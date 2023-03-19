@ECHO off
cls

:BEGIN
SET /p userId= Enter user id: 
@ echo userID %userId%
mkdir results\S%userID%\scene_0\
mkdir results\S%userID%\scene_0\control_1
mkdir results\S%userID%\scene_0\control_5
mkdir results\S%userID%\scene_0\control_9
mkdir results\S%userID%\scene_0\control_10_force-SCL

mkdir results\S%userID%
mkdir results\S%userID%\scene_2\control_1
mkdir results\S%userID%\scene_2\control_5
mkdir results\S%userID%\scene_2\control_9
mkdir results\S%userID%\scene_2\control_10_force-SCL

SET /p perm= Enter permutation id (1-4):

echo START THE EXPERIMENT
pause

if %perm% == 1 GOTO P1
@ if %perm% == 2 GOTO P2
@ if %perm% == 3 GOTO P3
@ if %perm% == 4 GOTO P4

echo No such permutation is defined, please enter again
GOTO BEGIN

:P1
echo Permutation 1
echo PRACTICE 1
pause
start /WAIT /B "Practice 1" tiltboard-win.exe %userId% 0 1
pause
echo PRACTICE 2
pause
start /WAIT /B "Practice 2" tiltboard-win.exe %userId% 0 4
pause
echo BASELINE MEASUREMENT
python resources/baseline.py %userID%
echo BASELINE FINISHED
pause
start /WAIT /B "Experiment 1" tiltboard-win.exe %userId% 2 1
pause
start /WAIT /B "Experiment 2" tiltboard-win.exe %userId% 2 5
pause
start /WAIT /B "Experiment 3" tiltboard-win.exe %userId% 2 9 
pause
start /WAIT /B "Experiment 4" tiltboard-win.exe %userId% 2 10 force-SCL
echo STUDY COMPLETE.
pause
GOTO END

:P2
echo Permutation 2
echo PRACTICE 1
start /WAIT /B "Practice 1" tiltboard-win.exe %userId% 0 1
echo PRACTICE 2
start /WAIT /B "Practice 2" tiltboard-win.exe %userId% 0 4
pause
echo BASELINE MEASUREMENT
python resources/baseline.py %userID%
echo BASELINE FINISHED
pause
start /WAIT /B "Experiment 1" tiltboard-win.exe %userId% 2 5
pause
start /WAIT /B "Experiment 2" tiltboard-win.exe %userId% 2 9
pause
start /WAIT /B "Experiment 3" tiltboard-win.exe %userId% 2 10 force-SCL 
pause
start /WAIT /B "Experiment 4" tiltboard-win.exe %userId% 2 1
echo STUDY COMPLETE.
pause
GOTO END
GOTO END

:P3
echo Permutation 3
echo PRACTICE 1
start /WAIT /B "Practice 1" tiltboard-win.exe %userId% 0 1
echo PRACTICE 2
start /WAIT /B "Practice 2" tiltboard-win.exe %userId% 0 4
pause
echo BASELINE MEASUREMENT
python resources/baseline.py %userID%
echo BASELINE FINISHED
pause
start /WAIT /B "Experiment 1" tiltboard-win.exe %userId% 2 9
pause
start /WAIT /B "Experiment 2" tiltboard-win.exe %userId% 2 10 force-SCL
pause
start /WAIT /B "Experiment 3" tiltboard-win.exe %userId% 2 1 
pause
start /WAIT /B "Experiment 4" tiltboard-win.exe %userId% 2 5
echo STUDY COMPLETE.
pause
GOTO END

:P4
echo Permutation 4
echo PRACTICE 1
start /WAIT /B "Practice 1" tiltboard-win.exe %userId% 0 1
echo PRACTICE 2
start /WAIT /B "Practice 2" tiltboard-win.exe %userId% 0 4
pause
echo BASELINE MEASUREMENT
python resources/baseline.py %userID%
echo BASELINE FINISHED
pause
start /WAIT /B "Experiment 1" tiltboard-win.exe %userId% 2 10 force-SCL
pause
start /WAIT /B "Experiment 2" tiltboard-win.exe %userId% 2 1
pause
start /WAIT /B "Experiment 3" tiltboard-win.exe %userId% 2 5
pause
start /WAIT /B "Experiment 4" tiltboard-win.exe %userId% 2 9
echo STUDY COMPLETE.
pause
GOTO END

:END