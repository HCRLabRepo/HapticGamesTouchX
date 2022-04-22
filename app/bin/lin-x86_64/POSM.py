import sys
import time
import os 
import pandas as pd
import numpy as np
import math

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
confidence_vector = np.array([1.0,1.0,1.0])
beta_decay = 0.7

# Getting the live data from the subject.(btGSR_PPG.py must be running), and do the prediction.
gsr_file_name = "S" + subject_num + "/GSR_PPG" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
prediction_file_name = "S" + subject_num + "/prediction" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
prediction_live_file_name = "S" + subject_num + "/prediction" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + "_live" + ".csv"

# Getting the live data from the game for SFA (Score Feedback Adjustment.)
ball_file_name  = "S" + subject_num + "/ball" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
best_ball_file_name = "S0/ball/S0_R0_" + game_scene + "_4.csv"

if control_mode != "3":
   print("Not physiology control mode")
else:
   with open(prediction_file_name, 'w+') as writer:
      writer.write("1\n")
   starttime = time.time()
   while True:
      # Making prediction with confidence vector
      prediction_score = np.array([])
      for i in range(confidence_vector.size):
         A = np.array([confidence_vector[k] for k in range(0,i+1)])
         B = np.array([confidence_vector[k] for k in range(i,confidence_vector.size)])
         prediction_score = np.append(prediction_score, min(A.sum(),B.sum()))
      prediction = np.argmax(prediction_score)
      print("prediction",prediction)
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
      print("live conductance mean: ",gsr_mean)
      print("baseline conductance mean: ", baseline_gsr_mean)
      print("pfa: ", pfa)

      #Update the baseline on the go.
      baseline_gsr_mean = baseline_gsr_mean*0.9 + gsr_mean*0.1

      # if pfa > 1.7:
      #    observation = -1
      # elif pfa < 1.3:
      #    observation = 1
      # else:
      #    observation = 0

      # SFA
      # 
      game_data = np.array([])
      data = pd.read_csv(ball_file_name)
      before_data = data.to_numpy()
      data.dropna(inplace=True)
      data = data.to_numpy()
      size_live = len(data)

      num_collision = before_data.shape[0] - data.shape[0] + 1
      score_collision = 1/(np.log(num_collision/1000)+math.e)

      game_data = np.array([])
      data = pd.read_csv(best_ball_file_name)
      data = data.head(size_live)
      before_data = data.to_numpy()
      data.dropna(inplace=True)
      data = data.to_numpy()

      # Because the num_collision_best do not grow with long time. we add size_live/100 so the user can get higher score in later time.
      num_collision_best = before_data.shape[0] - data.shape[0] + size_live/1000
      score_collision_best = 1/(np.log(num_collision_best/1000)+math.e)
      sfa = score_collision/score_collision_best
      print("sfa: ",sfa)
      # if sfa < 0.3:
      #    observation = -1
      # elif sfa >0.7:
      #    observation = 1
      # else:
      #    observation = 0
      
      # CFA Tables RULES
      #
      observation = 0
      if sfa <0.2 and pfa > 1.7:
         observation = -1

      elif sfa> 0.5 and pfa<1.3:
         observation = 1

      elif 0.2<=sfa<=0.5 and 1.3<= pfa <=1.7:
         observation = 0

      elif sfa < 0.2 and 1.3<= pfa <=1.7:
         if sfa-0.2 >= 1.7-pfa:
            observation = -1
         else:
            observation = 0

      elif sfa > 0.5 and 1.3 <= pfa <=1.7:
         if sfa-0.5 >= 1.7-pfa:
            observation = 1
         else:
            observation = 0

      elif 0.2<=sfa<=0.5 and pfa < 1.3:
         if 0.5-sfa < 1.3-pfa:
            observation = -1
         elif 0.5-sfa > 1.3-pfa:
            observation = 1
         else:
            observation = 0

      elif 0.2<=sfa<=0.5 and pfa > 1.7:
         if sfa-0.2 > pfa-1.7:
            observation = 0
         else:
            observation = -1

      elif sfa < 0.2 and pfa < 1.3:
         if 0.2 - sfa > 1.3 - pfa:
            observation = -1
         elif  0.2 -sfa < 1.3-pfa:
            observation = 1
         else:
            observation = 0

      elif sfa > 0.7 and pfa > 1.7:
         if sfa-0.7 > pfa - 1.7:
            observation = +1
         elif sfa-0.7 < pfa - 1.7:
            observation = -1
         else:
            observation = 0

      # Update confidence vector by observation from CFA
      print("observation:" , observation)
      if observation == -1:
         for i in range(prediction+1):
            confidence_vector[i] *= beta_decay
      elif observation == 1:
         for i in range(prediction,confidence_vector.size):
            confidence_vector[i] *= beta_decay

      time.sleep(10.0 - ((time.time() - starttime) % 10.0))
   