# TouchX
This is a haptic game that control a ball to destination without touching the obstacles.

# Prerequisite

1. Operating System: Ubuntu 18.04.6 LTS
2. 3D Systems: Touch or Touch X
3. Shimmer3 GSR+ Unit

# How To Install

1. CHAI3D <br>
[Download](https://www.chai3d.org/download/releases)


2. Haptic Device Driver<br>
[Official Guide for Linux Installation](https://support.3dsystems.com/s/article/OpenHaptics-for-Linux-Developer-Edition-v34?language=en_US)<br>
[Instruction](Installation+Instructions.pdf) : Available from the official link.

# Files Structure
Here I explain how I arrange the files and dependencies.

1. The codes for single program is store as a folder just like tiltboard and tiltboard2.

2. You compile the codes by running the makefile in the project folder. 

3. You can find your project as executable program in bin/lin-x86_64 folder. The naming could be different depends on your operating systems and hardware.

```
📦app
 ┣ 📂.vscode
 ┣ 📂bin                # Executable file after compilation.
 ┃ ┗ 📂lin-x86_64       # Executable files on Linux.
 ┃
 ┣ 📂external           # I put the chai3d library here.
 ┃ ┗ 📂chai3d-3.2.0
 ┃
 ┣ 📂lib                # Compiled chai3d.
 ┣ 📂tiltboard          # The first version of the game.
 ┗ 📂tiltboard2         # Final version with physics engine.
```

This is where I program the game, the workspace.

```
📦tilt-board2
 ┣ 📂obj                    # Generated after compile.
 ┃ ┗ 📂release
 ┃   ┗ 📂lin-x86_64-cc
 ┃     ┣ 📜CGenericScene.o
 ┃     ┣ 📜CScene1.o
 ┃     ┣ 📜CScene2.o
 ┃     ┣ 📜CScene3.o
 ┃     ┗ 📜tilt-board2.o
 ┃
 ┣ 📜CGenericScene.cpp      # A base code for all scene.
 ┣ 📜CGenericScene.h
 ┣ 📜CScene1.cpp            # Customised scene1.
 ┣ 📜CScene1.h
 ┣ 📜CScene2.cpp
 ┣ 📜CScene2.h              # Customised scene2.
 ┣ 📜CScene3.cpp
 ┣ 📜CScene3.h              # Customised scene3.
 ┣ 📜Makefile               # The Makefile to compile.
 ┗ 📜tilt-board2.cpp        # The main.
```
This is where I execute my file and run experiment.

The important file is Experiment Settings.
```
📦bin
 ┣ 📂lin-x86_64
 ┃ ┣ 📂S0                       # Data of a subject.
 ┃ ┃ ┣ 📂CIP                    # Data of the CIP.
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_3_4.csv
 ┃ ┃ ┃ ┗ 📜S0_R0_3_4_force.csv
 ┃ ┃ ┣ 📂GSR_PPG                # Skin conductance data.
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2.csv
 ┃ ┃ ┃ ┗ 📜S0_M22_3_3.csv
 ┃ ┃ ┣ 📂HIP                    # HIP data
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2_force.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4_force.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_3_4.csv
 ┃ ┃ ┃ ┗ 📜S0_R0_3_4_force.csv
 ┃ ┃ ┣ 📂NIP                    # NIP data
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4.csv
 ┃ ┃ ┃ ┗ 📜S0_R0_3_4.csv
 ┃ ┃ ┣ 📂ball                   # Ball data
 ┃ ┃ ┃ ┣ 📜S0_M22_0_0.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_1_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_2_3.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_1.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_2.csv
 ┃ ┃ ┃ ┣ 📜S0_M22_3_3.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_1_4.csv
 ┃ ┃ ┃ ┣ 📜S0_R0_2_4.csv
 ┃ ┃ ┃ ┗ 📜S0_R0_3_4.csv
 ┃ ┃ ┗ 📂prediction             # POSM predictions.
 ┃ ┃   ┣ 📜S0_M22_1_3.csv
 ┃ ┃   ┣ 📜S0_M22_1_3_live.csv
 ┃ ┃   ┣ 📜S0_M22_2_3.csv
 ┃ ┃   ┣ 📜S0_M22_2_3_live.csv
 ┃ ┃   ┣ 📜S0_M22_3_3.csv
 ┃ ┃   ┗ 📜S0_M22_3_3_live.csv
 ┃ ┣ 📜ExperimentSettings.txt   
 ┃ ┣ 📜GSR_PPG.csv      
 ┃ ┣ 📜POSM.py                  # Make prediction based on GSR data.
 ┃ ┣ 📜btGSR_PPG.py             # Run to collect EDA in real-rime.
 ┃ ┣ 📜cfont
 ┃ ┣ 📜cimage
 ┃ ┣ 📜cshader
 ┃ ┣ 📜data_gen.py
 ┃ ┣ 📜distanceTravelled.py
 ┃ ┣ 📜example.csv
 ┃ ┣ 📜plot_live.py             # Plot EDA in real-time.
 ┃ ┣ 📜test.py
 ┃ ┣ 📜test2.py
 ┃ ┣ 📜tilt-board
 ┃ ┣ 📜tilt-board2              # Run the game
 ┃ ┗ 📜trial.csv
 ┗ 📂resources                  # chai3d resources.
```

# How To Run
1. Connect the Haptic Device

Open terminal
```bash
Touch_Setup ## Open a program to setup connection.
Touch_Diagnostic ## Open a program to test the hardware.
```

2. Ready the shimmer sensors for recording physiology data.

There are some python2 scripts to connects the shimmers hardware in the from the official Github [Python Scripts](https://github.com/ShimmerResearch/shimmer3/tree/master/LogAndStream/python_scripts). I already downloaded the btGSR_PPG.py and modified it for my own usage.
```bash
python btGSR_PPG.py ##My python default is python2.
```
3. Run the game.

Open your ExperimentSettings.txt
```
0   # Subject number, i reserved 0 as admin.
M   # Sex
0   # age
1   # Game Scene
1   # Control mode, known as shared control mechanism in dissertation. 1 is full control, 2 is shared control, 3 is variable control.
```
Open terminal, go to the bin/lin_x86_64 and run
```bash
./tiltboard2
```

You can change your scene by clicking 1, 2, 3 on the keyboards. But you can only change the control mode by modifying the ExperimentSettings and re-run the program.

You should see your data in your the S0 folder now.

Noted*: You should always create the data folders before running the experiment. Some data would not save if the folders are not existed, I have lost data from 2 subjects by this issue. Of course you can always debug it when you have time.

4. Predict the stress cognitive level by POSM algorithm.

Open new terminal window.
```bash
python3 POSM.py 
``` 
## Notes
- You can see guidance sphere by clicking G, negotiated sphere by clicking N, human control sphere by clicking S.