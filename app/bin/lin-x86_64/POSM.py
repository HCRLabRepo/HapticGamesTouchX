import sys
import time
import os 
import pandas as pd
import numpy as np


# Getting the subject information.
with open('ExperimentSettings.txt') as f:
   subject_num = f.readline()[:-1]
   subject_sex = f.readline()[:-1]
   subject_age = f.readline()[:-1]
   game_scene = f.readline()[:-1]
   control_mode = f.readline()

# Getting the baseline data from the subject.
baseline_filename = "S" + subject_num + "/GSR_PPG" + "/S" + subject_num + "_" + subject_sex + subject_age + "_0_0.csv"
baseline_data = pd.read_csv(baseline_filename)
baseline_gsr = baseline_data['GSR']
baseline_gsr = baseline_gsr[1280:]
baseline_gsr_mean = baseline_gsr.mean()

# Initalize the POSM variables.
confidence_vector = np.array([1,1,1])
beta_decay = 0.9

# Getting the live data from the subject.(btGSR_PPG.py must be running), and do the prediction.
gsr_file_name = "S" + subject_num + "/GSR_PPG" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
prediction_file_name = "S" + subject_num + "/prediction" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
prediction_live_file_name = "S" + subject_num + "/prediction" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + "_live" + ".csv"
if control_mode != "3":
   print("Not physiology control mode")
else:
   with open(prediction_file_name, 'w+') as writer:
      writer.write("0\n")
   starttime = time.time()
   while True:
      # Making prediction with confidence vector
      prediction_score = np.array([])
      for i in range(confidence_vector.size):
         A = np.array([confidence_vector[k] for k in range(0,i+1)])
         B = np.array([confidence_vector[k] for k in range(i,confidence_vector.size)])
         prediction_score = np.append(prediction_score,min(A.sum(),B.sum()))
      prediction = np.argmax(prediction_score)
      with open(prediction_file_name, 'a') as writer:
         writer.write(str(prediction)+"\n")
      with open(prediction_live_file_name, 'w+') as writer:
         writer.write(str(prediction)+"\n")
      
      # PFA
      data = pd.read_csv(gsr_file_name)
      gsr = data['GSR']
      gsr = gsr[-1280:]
      gsr_mean = gsr.mean()
      pfa = gsr_mean / baseline_gsr_mean
      if pfa > 1.7:
         observation = 1
      elif pfa < 1.3:
         observation = -1
      else:
         observation = 0

      print(pfa)
      # SFA
      # 

      # CFA
      #

      # Update confidence vector by observation from CFA
      if observation == 1:
         for i in range(prediction+1):
            confidence_vector[prediction] *= beta_decay
      elif observation == -1:
         for i in range(prediction,confidence_vector.size):
            confidence_vector[prediction] *= beta_decay

      

      time.sleep(10.0 - ((time.time() - starttime) % 10.0))
