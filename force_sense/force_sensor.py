import matplotlib.pyplot as plt
import numpy as np
import serial

quit = False

# while loop begin
while not quit:
    ser = serial.Serial('/dev/tty.usbmodem101')
    print('Opening port: ')
    print(ser.name)
    print('Begin\n') 

    # Read the user's choice
    selection = input('\nENTER Number of Samples, q to quit: ')
    if selection == 'q':
        quit = True
        ser.close()
        break

    # Read data from the serial port 
    t = []
    data = []
    filtered = []
    sample_rate = 0 

    selection_endline = selection + '\n'

    # Send the command to the Pico
    ser.write(selection_endline.encode())

    # Read N lines back from the Pico
    num_samples = int(selection)
    for i in range(num_samples):
        line = ser.readline().decode('utf-8').strip()
        parts = line.split()
        if len(parts) == 3:
            t.append(int(parts[0]))
            data.append(int(parts[1]))
            filtered.append(float(parts[2]))

    # Derive sample rate from timestamps (ms -> Hz)
    if len(t) > 1:
        avg_interval_ms = (t[-1] - t[0]) / (len(t) - 1)
        sample_rate = 1000.0 / avg_interval_ms

    # Print the data to verify it was read
    for i in range(len(t)):
        print(str(t[i]) + ", " + str(data[i]) + ", " + str(filtered[i]))
    print("Sample rate: " + str(sample_rate))

    # Convert to numpy arrays
    t_sec = np.array(t) / 1000.0   # ms -> seconds
    y = np.array(filtered)          # use filtered data for FFT

    # FFT computation
    Fs = sample_rate
    Ts = 1.0 / Fs                       # sampling interval
    n = len(y)                          # length of the signal
    k = np.arange(n)
    T = n / Fs
    frq = k / T                         # two sided frequency range
    frq = frq[range(int(n/2))]          # one sided frequency range
    Y = np.fft.fft(y) / n              # fft computing and normalization
    Y = Y[range(int(n/2))]

    # Plot
    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t_sec, y, 'k')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Signal IIR Filtered')
    ax2.loglog(frq, abs(Y), 'k')       # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.tight_layout()
    plt.show()

    ser.close()