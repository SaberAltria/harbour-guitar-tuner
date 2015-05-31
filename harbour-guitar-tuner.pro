# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-guitar-tuner

QT += multimedia
QT += qml

CONFIG += sailfishapp

SOURCES += src/harbour-guitar-tuner.cpp \
    src/fastfouriertransformer.cpp \
    src/fftpack.c \
    src/guitartuner.cpp \
    src/voiceanalyzer.cpp \
    src/voicegenerator.cpp

HEADERS += src/constants.h \
    src/fastfouriertransformer.h \
    src/guitartuner.h \
    src/voiceanalyzer.h \
    src/voicegenerator.h

OTHER_FILES += qml/harbour-guitar-tuner.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/GuitarTunerPanel.qml \
    qml/pages/IOControls.qml \
    qml/pages/Meter.qml \
    qml/pages/Slider.qml \
    qml/pages/StringIndicator.qml \
    qml/pages/StringSelector.qml \
    qml/pages/StringsModel.qml \
    rpm/harbour-guitar-tuner.changes.in \
    rpm/harbour-guitar-tuner.spec \
    rpm/harbour-guitar-tuner.yaml \
    translations/*.ts \
    harbour-guitar-tuner.desktop \
    qml/images/big_a.png \
    qml/images/big_b.png \
    qml/images/big_d.png \
    qml/images/big_e.png \
    qml/images/big_g.png \
    qml/images/glowing_a.png \
    qml/images/glowing_b.png \
    qml/images/glowing_d.png \
    qml/images/glowing_e.png \
    qml/images/glowing_g.png \
    qml/images/guitartuner_malli.png \
    qml/images/guitartuner_skin.png \
    qml/images/lcdFrame.png \
    qml/images/meter-background.png \
    qml/images/mute.png \
    qml/images/pointer-shadow.png \
    qml/images/pointer.png \
    qml/images/power.png \
    qml/images/quit.png \
    qml/images/sensitivity.png \
    qml/images/slider-background.png \
    qml/images/slider.png \
    qml/images/tuner_a_on.png \
    qml/images/tuner_a.png \
    qml/images/tuner_auto_on.png \
    qml/images/tuner_auto.png \
    qml/images/tuner_b_on.png \
    qml/images/tuner_b.png \
    qml/images/tuner_d_on.png \
    qml/images/tuner_d.png \
    qml/images/tuner_e_on.png \
    qml/images/tuner_e.png \
    qml/images/tuner_g_on.png \
    qml/images/tuner_g.png \
    qml/images/voicemode_off.png \
    qml/images/voicemode_on.png \
    qml/images/volume_off.png \
    qml/images/volume.png \
    qml/images/guitartab.svg \
    qml/images/inputMode.svg \
    qml/images/note.svg \
    qml/images/noteNo.svg

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-guitar-tuner-de.ts

