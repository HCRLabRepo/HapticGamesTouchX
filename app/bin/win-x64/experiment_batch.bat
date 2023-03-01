@ECHO off
cls

:BEGIN
SET /p userId= Enter user id: 
@ echo userID %userId%
mkdir results\S%userID%
mkdir results\S%userID%\control_1
mkdir results\S%userID%\control_5
mkdir results\S%userID%\control_8
mkdir results\S%userID%\control_9
mkdir results\S%userID%\control_10_coll-force
mkdir results\S%userID%\control_10_coll-SCL
mkdir results\S%userID%\control_10_force-SCL
mkdir results\S%userID%\control_10_coll-force-SCL

SET /p perm= Enter permutation id (1--8):

echo START THE EXPERIMENT
pause

if %perm% == 1 GOTO P1
@ if %perm% == 2 GOTO P2
@ if %perm% == 3 GOTO P3

echo No such permutation is defined, please enter again
GOTO BEGIN

:P1
echo Permutation 1
start /WAIT /B "Experiment 1" tiltboard-win.exe %userId% 2 1
pause
start /WAIT /B "Experiment 2" tiltboard-win.exe %userId% 2 5
pause
start /WAIT /B "Experiment 3" tiltboard-win.exe %userId% 2 8
pause
start /WAIT /B "Experiment 4" tiltboard-win.exe %userId% 2 9 
pause
start /WAIT /B "Experiment 5" tiltboard-win.exe %userId% 2 10 coll-force
pause
start /WAIT /B "Experiment 6" tiltboard-win.exe %userId% 2 10 coll-SCL
pause
start /WAIT /B "Experiment 7" tiltboard-win.exe %userId% 2 10 force-SCL
pause
start /WAIT /B "Experiment 8" tiltboard-win.exe %userId% 2 10 coll-force-SCL
echo STUDY COMPLETE.
pause

:P2
echo Permutation 2
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 1
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 5
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 8
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 9 
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-force
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-SCL
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 force-SCL
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-force-SCL

:P3
echo Permutation 3
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 1
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 5
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 8
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 9 
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-force
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-SCL
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 force-SCL
start /WAIT /B "Experiment" tiltboard-win.exe %userId% 2 10 coll-force-SCL