import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
data1 = [] # column 1
data2 = [] # column 2

with open('DSP/sigC.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

sample_rate = (len(data1))/(t[-1])

filter_list = []
size_x = 1000

for i in range(size_x):
    filter_list.append(0)

for i in range(len(t)):
    filter_list.append(data1[i])
    filter_list.pop(0)
    data2.append(sum(filter_list)/size_x)
    

plt.plot(t,data1,'b-*')
plt.plot(t,data2,'r-*')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title('Signal vs Time')
plt.show()

