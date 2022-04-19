import sys
import time
import os 

with open('ExperimentSettings.txt') as f:
   subject_num = f.readline()[:-1]
   subject_sex = f.readline()[:-1]
   subject_age = f.readline()[:-1]
   game_scene = f.readline()[:-1]
   control_mode = f.readline()

