import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
data1 = [] # column 1
data2 = [0] # column 2

with open('DSP/sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

sample_rate = (len(data1))/(t[-1])

filter_list = []
A_val = 0.96
B_val  = 1 - A_val

data2 = [data1[0]] # column 2

for i in range(1, len(t)):
    data2.append( A_val * data2[i-1] + B_val * data1[i] )


for i in range(len(t)):
    # print the data to verify it was read
    print(str(t[i]) + ", " + str(data1[i]) + ", ")

print(str(sample_rate))

Fs = sample_rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data1 # the data to make the fft from
y2 = data2
n = len(y) # length of the signal
n2 = len(y2) # length of the signal

k = np.arange(n)
k2 = np.arange(n2)

T = n/Fs
T_2 = n2/Fs

frq = k/T # two sides frequency range
frq2 = k/T_2 # two sides frequency range

frq = frq[range(int(n/2))] # one side frequency range
frq2 = frq2[range(int(n2/2))] # one side frequency range

Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

Y_2 = np.fft.fft(y2)/n2 # fft computing and normalization
Y_2 = Y_2[range(int(n2/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,y,'k')
ax1.plot(t,y2,'r')

ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Signal D IRR A: ' + str(A_val) + ' B:' + str(B_val))

ax2.loglog(frq,abs(Y),'k') # plotting the fft
ax2.loglog(frq,abs(Y_2),'r') # plotting the fft

ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()