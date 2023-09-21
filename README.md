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

4. Python 3.11
[Download](https://www.python.org/downloads/)
**Please ensure this is installed in the default location (C:\Python311)**

5. Pyserial package

Firstly, run the Touch Setup utility to configure your haptic device. The Touch is recommended, but the Touch X can also work. Make sure whichever one you are using is set as the default device. The Touch diagnostic can then be run to check that the device is properly set up.

Run the setup.bat script to compile the external libraries and the project. It must be ran using the Visual Studio developer console **OR** after adding devenv.exe to PATH.
Please ensure that the dll files required for running the project are present in the win_x64 folder: fuzzylite.dll, hdPhantom64.dll, Leap.dll, sixense_x64.dll, and tdLeap.dll. There should also be a folder named "resources" where the text files containing the coordinates for the waypoints should be present.

**NOTE:** Please ensure the correct COM port is set in line 20 of read_sensor.py!!

There are two ways of starting an experiment:
1. Modifying the ExperimentSettings.txt file. This should be formatted as such:  
Subject Number  
Scene Number  
Control System  
Fuzzy Parameters (if needed)  

2. Command Line
```
tiltboard-win.exe SUBJECT_NUMBER SCENE_NUMBER CONTROL_SYSTEM FUZZY_PARAMS(if needed)
```

# Source Files
The source and header files for this implementation can be found in the app/tiltboard-win/tiltboard-win folder.


``` 
## Notes
- You can see the guidance sphere by clicking G, the negotiated sphere by clicking N, and the human control sphere by clicking S.
```

# Updates
## How to run this project
1. Open solution

    Go to `\HapticGamesTouchX-Windows\app\tiltboard-win\tiltboard-win` to find the `tiltboard-win.sln`, and open this file in Visual Studio 2022.

3. Remember to run the ``setup. bat`` **within** Visual Studio 2022, before you build the project. 

    Go to ``Tools -> Command-Line`` to find the developer console, either one is okay to use. This .bat only needs to be run once. Run *setup.bat* in this console.

5. Build the project:

    Go to the menu bar on the top, change to release x64, and then click "run local debugger" to build the project. If this is successful, the prompt will show something as "press any key to exit". So, quit.
         
    The "setting files not found" could be ignored, as the author puts all .dll files in a different directory (the directory where ``tiltboard-win.exe`` is located). Maybe you can try to move everything to the current directory, but I am too scared to do so. (Project breakdown has ruined my confidence :p)

6. Run the .exe

    Run a new ``cmd``, go to the directory where the ``tiltboard-win.exe`` is located, and then run the command line mentioned above if you would like to specify the mode, such as tiltboard-win.exe 0 1 1, tiltboard-win.exe 0 2 5, etc. Now, you can play! Woah-ho!!

## Running on Windows 11 64-bit
1. Install Python 3.11:

    Install it either in ``C:/Python311`` or the default location (mine is ``C:/ ProgramFiles/Python311`` after selecting install for all users and adding to the environmental path), the ``setup.bat`` still has 1 failed configuration, but it doesn't affect the following project building and running.

    I pre-assume as long as ``Solution Explorer -> Properties -> Linker -> Additional Library Directories`` has the Python path such as ``C:/Python311/libs`` or ``C:/ProgramFiles/Python311/libs``, and the ``Advanced System Setting -> Environmental Variables -> System Variables -> Path`` indicates the path of Python, the project can detect ``python311.dll`` successfully.

    In ``Advanced System Setting -> Environmental Variables -> System Variables -> Path``, it should contain the path to Python and CMake.

    When it gives me an error like "can't find python311.dll", to make this project run, I put this .dll under the same directory where ``tiltboard-win.exe`` is. 

2. Pyserial Packages Installing:

    Go to the Pyserial GitHub page, zip the code, and download it to the local machine. Extract the zip folder and move the folder to your ideal location. I put it in the same directory as Python. Run a new ``cmd`` as administrator, go to the directory where the ``setup.py`` file in the Pyserial folder is stored, run *python setup.py install*, and then the Pyserial packages can be installed successfully.

4. CMake:

   Download ``cmake-3.27.6-windows-x86_64.msi`` from the official website and install it.

5. It may tell you an SDK error:
```
Open Visual Studio Installer.
Click on "Modify" or "More" -> "Modify" next to your Visual Studio 2022 installation.
Go to the "Individual components" tab.
Search for "SDK 10.0.22000.0" and check it.
Click on the "Install" or "Modify" button at the bottom right.
```

## Errors to be fixed
1. Sensor:

    When separating the sensor code from the project, to make the project run without a GSR sensor, the project can't run if I remove the print code *sys.stdout.write("-1")* around line 73 in ``read_sensor.py``. If you put sleeping code before this block, the project can only run the time you specify in the sleeping code. For example, sleep(10), then the project runs only 10 seconds. After that, it will crush.
   
    The idea is to set a global variable in ``read_sensor.py``, defining whether the sensor is connected or not. If not, skip all the code but return -1 to the SensorData.cpp file, where it converts Python code to C++ code.
   
    The interesting thing is when the *sys.stdout.write("-1")* is removed, it seems like the project can't receive -1 anymore, which shouldn't happen as the *sys.stdout.write("-1")* doesn't have the function to return any value.

    To make the project run, in the end, the current way is to comment the line around 683 *s->getData();* in ``tilt-board2.cpp``, to stop reading any data from the sensor, avoiding entering ``read_sensor.py`` and ``SensorData.cpp``.

2. Setup.bat has one failed configuration, but it can run. -- Win 11 only
   
    Not always happen tho. It only happens after I fix the issue of undetected pyserial packages.
