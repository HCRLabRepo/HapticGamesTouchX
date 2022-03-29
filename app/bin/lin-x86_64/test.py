import numpy as np
import pandas as pd
import neurokit2 as nk
import matplotlib.pyplot as plt
import time

data = pd.read_csv('GSR_PPG.csv')
ppg = data["GSR"]

signals, info = nk.eda_process(ppg, sampling_rate=10)

fig = nk.eda_plot(signals, sampling_rate=10)

