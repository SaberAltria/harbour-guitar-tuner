/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "voiceanalyzer.h"

#include <QtCore/QDebug>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>

#include "constants.h"
#include "fastfouriertransformer.h"

// Constant used to scale the cut-off density for the fft helper.
const static float CutOffScaler(0.05);

// Force the precision to be "1/PrecisionPerNote" notes near the target
// frequency.
const static int PrecisionPerNote(4);

// TargetFrequencyParameter is a constant which implies the index at which
// corresponds to the target frequency. 0.5 * N * 1 / TargetFrequencyParameter
// is (about) the index which corresponds to the given target frequency.
// Effectively TargetFrequencyParameter = 2^z, and the z * TargetFrequency
// is the maximum frequency that can be noticed.
const static int TargetFrequencyParameter(4);


/*!
  \class VoiceAnalyzer
  \brief Analyzes the data received from the audio input device.
*/


/*!
  Constructor.
*/
VoiceAnalyzer::VoiceAnalyzer(const QAudioFormat &format, QObject *parent)
    : QIODevice(parent),
      m_fftHelper(0),
      m_format(format),
      m_totalSampleCount(0),
      m_maximumVoiceDifference(0),
      m_stepSize(0),
      m_frequency(0),
      m_position(0)
{
    Q_ASSERT(qFuzzyCompare(M_SAMPLE_COUNT_MULTIPLIER,
                           float(2) / (M_TWELTH_ROOT_OF_2 - 1.0)));

    m_fftHelper = new FastFourierTransformer(this);
    m_totalSampleCount = qRound(qreal(PrecisionPerNote)
                                * TargetFrequencyParameter
                                * M_SAMPLE_COUNT_MULTIPLIER);
    m_samples.reserve(m_totalSampleCount);

    int i(2);
    int j(1);

    for (; i < TargetFrequencyParameter; i *= 2) {
        j++;
    }

    m_maximumVoiceDifference = j * 12;
    setCutOffPercentage(CutOffScaler);
}


/*!
  Opens the parent QIODevice. Sets up the analysation parameters.
*/
void VoiceAnalyzer::start(qreal frequency)
{
    setSampleRate(frequency);
    open(QIODevice::WriteOnly);
}


/*!
  Closes the parent QIODevice, thus the voice is not analysed anymore.
  Resets the m_samples QList.
*/
void VoiceAnalyzer::stop()
{
    m_samples.clear();
    m_samples.reserve(m_totalSampleCount);
    close();
}


/*!
  Empty implementation for readData, since no data is provided by the
  VoiceAnalyzer class.
*/
qint64 VoiceAnalyzer::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);

    return 0;
}


/*!
  Called when data is obtained. Stores each m_stepSize sample into a QList
  to be analysed. Returns the amount of data written.
*/
qint64 VoiceAnalyzer::writeData(const char *data, qint64 maxlen)
{
    const int channelBytes = m_format.sampleSize() / 8;
    int sampleSize = m_format.channelCount() * channelBytes;
    int m_stepSizeInBytes = m_stepSize*sampleSize;

    // assert that each sample fits fully into the data
    Q_ASSERT((m_position % sampleSize) == 0);

    const uchar *ptr = reinterpret_cast<const uchar *>(data);

    while (m_position < maxlen) {
        if (m_samples.size() < m_totalSampleCount) {
            m_samples.append(getValueInt16(ptr+m_position));
        }
        else {
            analyzeVoice();
            m_samples.clear();
            m_samples.reserve(m_totalSampleCount);

            // fast forward position to the first position after maxlen or to the maxlen
            m_position += ((m_stepSizeInBytes - 1 + maxlen - m_position) /
                           m_stepSizeInBytes) * m_stepSizeInBytes;
            break;
        }

        m_position += m_stepSizeInBytes;
    }

    m_position -= maxlen;
    return maxlen;
}


/*!
  Returns the current target frequency.
*/
qreal VoiceAnalyzer::frequency()
{
    return m_frequency;
}


/*!
  Returns the maximum absolute value sent by the voiceDifference() signal.
*/
int VoiceAnalyzer::getMaximumVoiceDifference()
{
    return m_maximumVoiceDifference;
}


/*!
  Returns the maximum precision per note near the target frequency.
*/
int VoiceAnalyzer::getMaximumPrecisionPerNote()
{
    return PrecisionPerNote;
}


/*!
  Sets the target frequency to \a frequency.
*/
void VoiceAnalyzer::setSampleRate(qreal frequency)
{
    Q_ASSERT(frequency > 0); // Avoid division by zero
    qDebug() << "VoiceAnalyzer::setSampleRate():" << frequency;

    m_stepSize = (qreal)(1.0 * m_format.sampleRate()
                         / (TargetFrequencyParameter * 2 * frequency));
    m_frequency = frequency;
}


/*!
  Takes \a cutoff, a number between 0 and 1, scales it with CutOffScaler,
  multiplies it with maximum density, and then gives it to the fft helper.
*/
void VoiceAnalyzer::setCutOffPercentage(qreal cutoff)
{
    qDebug() << "VoiceAnalyzer::setCutOffPercentage():" << cutoff;
    cutoff = CutOffScaler * cutoff;

    if (m_format.sampleSize() == 8) {
        float t = cutoff * m_totalSampleCount * M_MAX_AMPLITUDE_8BIT_SIGNED;
        m_fftHelper->setCutOffForDensity(t);
    }
    else if (m_format.sampleSize() == 16) {
        float t = cutoff * m_totalSampleCount * M_MAX_AMPLITUDE_16BIT_SIGNED;
        m_fftHelper->setCutOffForDensity(t);
    }
}


/*!
  Interprets \a ptr as a pointer to int value and returns it.
*/
qint16 VoiceAnalyzer::getValueInt16(const uchar *ptr)
{
    qint16 realValue(0);

    if (m_format.sampleSize() == 8)
    {
        const qint16 value = *reinterpret_cast<const quint8*>(ptr);

        if (m_format.sampleType() == QAudioFormat::UnSignedInt) {
            realValue = value - M_MAX_AMPLITUDE_8BIT_SIGNED - 1;
        }
        else if (m_format.sampleType() == QAudioFormat::SignedInt) {
            realValue = value;
        }
    }
    else if (m_format.sampleSize() == 16) {
        qint16 value(0);

        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
            value = qFromLittleEndian<quint16>(ptr);
        else
            value = qFromBigEndian<quint16>(ptr);

        if (m_format.sampleType() == QAudioFormat::UnSignedInt) {
            realValue = value - M_MAX_AMPLITUDE_16BIT_SIGNED;
        }
        else if (m_format.sampleType() == QAudioFormat::SignedInt) {
            realValue = value;
        }
    }

    return realValue;
}


/*!
  Analyzes the voice frequency and emits appropriate signals.
*/
void VoiceAnalyzer::analyzeVoice()
{
    m_fftHelper->calculateFFT(m_samples);
    int index = m_fftHelper->getMaximumDensityIndex();

    // If index == -1
    if (index == -1) {
        // The voice is to be filtered away.
        // Emit the lowVoice signal and return.
        qDebug() << "VoiceAnalyzer::analyzeVoice(): Low voice";
        emit lowVoice();
        return;
    }

    // Else, continue

    // Let the correctIndex to be the nearest index corresponding to the
    // correct frequency.
    qreal stepSizeInFrequency = (qreal)m_format.sampleRate()
            / (m_totalSampleCount * m_stepSize);
    qreal newFrequency = qreal(index) * stepSizeInFrequency;

    // Calculate the nearest index corresponding to the correct frequency.
    int correctIndex = qRound(m_frequency / stepSizeInFrequency);
    qreal value = 0;

    // If the obtained frequency is more than
    // log_2(TargetFrequencyParameter) octaves less than the m_frequency:

    // Note:
    // Instead of m_frequency/TargetFrequencyParameter > newFrequency,
    // the comparison is done without a div instructions by
    // m_frequency > newFrequency * TargetFrequencyParameter.

    if (m_frequency > newFrequency * TargetFrequencyParameter) {
        // Set the difference value to be -m_maximumVoiceDifference.
        qDebug() << "VoiceAnalyzer::analyzeVoice(): Comparing (low):"
                 << newFrequency  << m_frequency - stepSizeInFrequency * correctIndex
                 << (m_frequency - stepSizeInFrequency * correctIndex) / stepSizeInFrequency;
        value = -m_maximumVoiceDifference;
    }
    // Else, if the obtained frequency is more than
    // log_2(TargetFrequencyParameter) octaves more than the m_frequency:
    else if (m_frequency * TargetFrequencyParameter < newFrequency) {
        // Set the difference value to be m_maximumVoiceDifference.
        qDebug() << "VoiceAnalyzer::analyzeVoice(): Comparing (high):"
                 << newFrequency << m_frequency - stepSizeInFrequency * correctIndex
                 << (m_frequency - stepSizeInFrequency * correctIndex) / stepSizeInFrequency;
        value = m_maximumVoiceDifference;
    }
    // Else:
    else {
        // Calculate the difference between the obtained and the correct
        // frequency in tones.
        // Use stepSizeInFrequency * correctIndex instead of
        // m_frequency so that the value is zero when there is correct
        // voice obtained. Set the difference value to be
        // log(frequency / target frequency) * 12 / log(2).
        value = log(newFrequency / (stepSizeInFrequency * correctIndex)) * 12 / M_LN2;
        qDebug() << "VoiceAnalyzer::analyzeVoice(): Comparing:"
                 << newFrequency << m_frequency - stepSizeInFrequency * correctIndex
                 << (m_frequency - stepSizeInFrequency * correctIndex) / stepSizeInFrequency;
    }

    // Emit voiceDifferenceChanged signal.
    qDebug() << "VoiceAnalyzer::analyzeVoice(): Voice difference changed:" << value;
    emit voiceDifferenceChanged(value);

    // If the correctIndex is index, emit the correctFrequency signal.
    if (correctIndex == index) {
        emit correctFrequency();
    }
}
