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

Run the setup.bat script to compile one of the external libraries. It must be ran using the Visual Studio developer console **OR** after adding devenv.exe to PATH.

Open the visual studio solution file located in app/tiltboard-win/tiltboard-win. Build all the projects and the resulting .exe file should be in app/bin/win-x64. Please ensure that the dll files required for running the project are present: fuzzylite.dll, hdPhantom64.dll, Leap.dll, sixense_x64.dll, and tdLeap.dll. There should also be a folder named "resources" where the text files containing the coordinates for the waypoints should be present.

# Source Files
The source and header files for this implementation can be found in the app/tiltboard-win/tiltboard-win folder.


``` 
## Notes
- You can see the guidance sphere by clicking G, the negotiated sphere by clicking N, and the human control sphere by clicking S.