Guitar Tuner
============

The Guitar Tuner application can be used to tune guitar strings by analysing
the audio recorded by the device microphone. Guitar Tuner can also be used in
listening mode, where it will play the audio of a given frequency, and the 
user can tune the guitar by ear. 

This example application demonstrates:

* The audio-in and audio-out interfaces of QtMobility::QtMultimediaKit.
* Integrating Qt code to the Qt Quick UI.

This example is hosted in GitHub:
https://github.com/nokia-developer/guitar-tuner

For more information on the implementation, visit the wiki pages:
https://github.com/nokia-developer/guitar-tuner/wiki


What's new
----------

The project structure has been fully refactored in version 2.0. The C++
implementation is now wrapped in the QML plug-in "GuitarTuner 1.0". In addition,
the communication model has been improved; the QML UI is logically controlling
the application engine.


1. Running the application
-------------------------------------------------------------------------------

Guitar Tuner is used to tune the strings of a guitar. Two modes are provided:
input mode and voice mode.

In input mode, the microphone is used to analyse the frequency of the recorded
audio and show the frequency via a graph. The desired string can be chosen with
the note buttons. The selected note will also be highlighted under the graph.
When the recorded frequency is very close to the right tuning, the needle will
be positioned exactly in the centre of the graph, and the note under the graph
will be highlighted with a glow. When the needle is all the way to the left or
right side of the graph, the note is 12 halfnotes (or more) too low or too high.
If Auto-note is selected, the application will try to determine the note by
itself. The recording sensitivity can be adjusted with the volume slider at the
bottom of the view.

In voice mode, the application will play the selected note via the speakers 
of the device. The output volume can be adjusted with the volume slider at the 
bottom of the view. The output volume can also be muted by tapping the 
microphone icon located on the left side of the volume slider.


2. Project structure
-------------------------------------------------------------------------------

 |                      The root folder contains the project file, the license
 |                      information, and this file (release notes).
 |
 |- bin                 Contains the pre-compiled binaries.
 |
 |- doc                 Contains the documentation.
 |
 |- guitartunermodule   Contains the application engine plug-in.
 |
 |- icons               Contains application icons.
 |
 |- images              Contains application graphics.
 |
 |- qml                 Contains the QML and JavaScript files.
 |
 |- qtc_packaging       Contains the Harmattan (Debian) packaging files.
 |
 |- rsc                 Contains the platform-specific resource files.
 |
 |- src                 Contains the main.cpp file.
 |


3. Compatibility
-------------------------------------------------------------------------------

 - Symbian devices with Qt 4.7.4 and Qt Mobility 1.2.1.
 - Nokia N9 (MeeGo 1.2 Harmattan)

Tested to work on:
 - Nokia 701
 - Nokia N8-00
 - Nokia E7-00
 - Nokia C6-01
 - Nokia N9

Developed with Qt SDK 1.2.1.

3.1 Required capabilities
-------------------------

UserEnvironment capability in Symbian. The application can be self signed.

3.2 Known issues
----------------

- In Harmattan the position of the slider knob is not initialised properly when
  the application is launched.

- In listening mode, the audio of the lowest strings cannot be heard properly 
  via device speakers. The device speakers are not capable of playing such low 
  frequency audio.


4. Building, installing, and running the application
-------------------------------------------------------------------------------

4.1 Preparations
----------------

Check that you have the latest Qt SDK installed in the development environment
and the latest Qt version on the device.

4.2 Using the Qt SDK
--------------------

You can install and run the application on the device by using the Qt SDK.
Open the project in the SDK, set up the correct target (depending on the device
platform), and click the Run button. For more details about this approach,
visit the Qt Getting Started section at Nokia Developer
(http://www.developer.nokia.com/Develop/Qt/Getting_started/).

4.3 Symbian device
------------------

Make sure your device is connected to your computer. Locate the .sis
installation file and open it with Nokia Suite. Accept all requests from Nokia
Suite and the device. Note that you can also install the application by copying
the installation file onto your device and opening it with the Symbian File
Manager application.

After the application is installed, locate the application icon from the
application menu and launch the application by tapping the icon.

4.4 Nokia N9 and Nokia N950
---------------------------

Copy the application Debian package onto the device. Locate the file with the
device and run it; this will install the application. Note that you can also
use the terminal application and install the application by typing the command
'dpkg -i <package name>.deb' on the command line. To install the application
using the terminal application, make sure you have the right privileges 
to do so (e.g., root access).

Once the application is installed, locate the application icon from the
application menu and launch the application by tapping the icon.


5. License
-------------------------------------------------------------------------------

See the license text file delivered with this project. The license file is also
available online at
https://github.com/nokia-developer/guitar-tuner/blob/master/Licence.txt


6. Version history
-------------------------------------------------------------------------------

v2.0 Complete refactoring, application engine code isolation in QML plug-in.
v1.3.1 Fixed build issues for Windows and Simulator.
v1.3 Added support for MeeGo 1.2 Harmattan.
v1.2 Fixed a crash bug. Added Windows support. Compatible with Qt 4.7.2.
v1.1 Initial version published in Forum Nokia Projects only.