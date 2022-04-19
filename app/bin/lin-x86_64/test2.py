import numpy as np
import pandas as pd
import neurokit2 as nk
import matplotlib.pyplot as plt
import time

data = pd.read_csv('easy_GSR_PPG.csv')
ppg = data["PPG"]

signals, info = nk.ppg_process(ppg, sampling_rate=128)

fig = nk.ppg_plot(signals, sampling_rate=128)

plt.show()