import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#plt.style.use('fivethirtyeight')
fig = plt.figure()
ax1 = fig.add_subplot(411, title="first")
ax1.title.set_text("Live GSR")
ax2 = fig.add_subplot(412, sharex=ax1)
ax2.title.set_text("Live PPG")
bx1 = fig.add_subplot(413)
bx1.title.set_text("ALL GSR")
bx2 = fig.add_subplot(414, sharex=bx1)
bx2.title.set_text("ALL PPG")

def animate(i):
    data = pd.read_csv('GSR_PPG.csv')
    x = data['Timestamp']
    y1 = data['GSR']
    y2 = data['PPG']
    

    window_x=x[-500:]
    window_y1=y1[-500:]
    window_y2=y2[-500:]

    # wd, m = hp.process(window_y2, 50)

    ax1.cla()
    ax2.cla()
    bx1.cla()
    bx2.cla()

    ax1.plot(window_x, window_y1, label='GSR')
    ax1.title.set_text("Live GSR")
    ax2.plot(window_x, window_y2, label='PPG')
    ax2.title.set_text("Live PPG")
    bx1.plot(x[200:], y1[200:], label='ALL GSR')
    bx1.title.set_text("ALL GSR")
    bx2.plot(x[200:], y2[200:], label='ALL PPG')
    bx2.title.set_text("ALL PPG")
    # plt.text(0.8, 0.85, 'heart rate='+str(m['bpm']), 
    #      fontsize=10, fontfamily='Georgia', color='k',
    #      ha='left', va='bottom',
    #      transform=plt.gca().transAxes)


ani = FuncAnimation(plt.gcf(), animate, interval=1000)
plt.tight_layout()
plt.show()