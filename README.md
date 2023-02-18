# TouchX
This is a haptic game consisting on controlling a ball towards a target as efficiently as possible by avoiding obstacles.

# Prerequisites

1. Operating System: Windows 10 64-bit
2. 3D Systems: Touch or Touch X
3. Shimmer3 GSR+ Unit

# How To Install

1. OpenHaptics Software & Drivers<br>
[Download](https://support.3dsystems.com/s/article/OpenHaptics-for-Windows-Developer-Edition-v35?language=en_US)<br>

2. Visual Studio 2022
[Download](https://visualstudio.microsoft.com/vs/)

3. CMake
[Download](https://cmake.org/)

4. Python 3
[Download](https://www.python.org/downloads/)

Firstly, run the Touch Setup utility to configure your haptic device. The Touch is recommended, but the Touch X can also work. Make sure whichever one you are using is set as the default device. The Touch diagnostic can then be run to check that the device is properly set up.

Run the setup.bat script to compile the external libraries and the project. It must be ran using the Visual Studio developer console **OR** after adding devenv.exe to PATH.
Please ensure that the dll files required for running the project are present in the win_x64 folder: fuzzylite.dll, hdPhantom64.dll, Leap.dll, sixense_x64.dll, and tdLeap.dll. There should also be a folder named "resources" where the text files containing the coordinates for the waypoints should be present.

# Source Files
The source and header files for this implementation can be found in the app/tiltboard-win/tiltboard-win folder.


``` 
## Notes
- You can see the guidance sphere by clicking G, the negotiated sphere by clicking N, and the human control sphere by clicking S.
