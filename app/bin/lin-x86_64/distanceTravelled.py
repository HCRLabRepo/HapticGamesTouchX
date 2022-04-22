import csv
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from numpy import genfromtxt

with open('ExperimentSettings.txt') as f:
   subject_num = f.readline()[:-1]
   subject_sex = f.readline()[:-1]
   subject_age = f.readline()[:-1]
   game_scene = f.readline()[:-1]
   control_mode = f.readline()

gamefilename = "S" + subject_num + "/ball" + "/S" + subject_num + "_" + subject_sex + subject_age + "_" + game_scene + "_" + control_mode + ".csv"
best_ball_file_name = "S0/ball/S0_R0_" + game_scene + "_4.csv"

game_data = np.array([])
data = pd.read_csv(gamefilename)
before_data = data.to_numpy()
data.dropna(inplace=True)
data = data.to_numpy()
distance = 0
distances = []

num_collision = before_data.shape[0] - data.shape[0]
print(num_collision)
for i in range(1,data.shape[0]):
    p1 = data[i-1][1:]
    p2 = data[i][1:]
    squared_dist = np.sum((p1-p2)**2, axis=0)
    dist = np.sqrt(squared_dist)
    distance += dist
    distances.append(distance)

game_data = np.array([])
data = pd.read_csv(best_ball_file_name)
before_data = data.to_numpy()
data.dropna(inplace=True)
data = data.to_numpy()
best_distance = 0
best_distances = []

num_collision = before_data.shape[0] - data.shape[0]
print(num_collision)
for i in range(1,data.shape[0]):
    p1 = data[i-1][1:]
    p2 = data[i][1:]
    squared_dist = np.sum((p1-p2)**2, axis=0)
    dist = np.sqrt(squared_dist)
    best_distance += dist
    best_distances.append(best_distance)

plt.plot(distances,'g',best_distances,'r')
plt.show()
print(distance)