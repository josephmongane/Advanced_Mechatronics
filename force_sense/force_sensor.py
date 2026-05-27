import matplotlib.pyplot as plt
import numpy as np
import serial

quit = False

while not quit:
    ser = serial.Serial('/dev/tty.usbmodem101')
    print('Opening port: ')
    print(ser.name)
    print('Begin\n')

    selection = input('\nENTER Number of Samples, q to quit: ')
    if selection == 'q':
        quit = True
        ser.close()
        break

    t = []
    data = []
    filtered = []

    selection_endline = selection + '\n'
    ser.write(selection_endline.encode())

    num_samples = int(selection)
    for i in range(num_samples):
        line = ser.readline().decode('utf-8').strip()
        parts = line.split()
        if len(parts) == 3:
            t.append(int(parts[0]))       # time in ms since program start
            data.append(int(parts[1]))
            filtered.append(float(parts[2]))

    # Calculate sample rate from actual timestamps
    t_sec = np.array(t) / 1000.0                        # convert ms -> seconds
    intervals = np.diff(t_sec)                           # time between each sample
    avg_interval = np.mean(intervals)                    # average interval in seconds
    sample_rate = 1.0 / avg_interval                     # Hz

    print(f"Calculated sample rate: {sample_rate:.2f} Hz")
    for i in range(len(t)):
        print(f"{t[i]}, {data[i]}, {filtered[i]}")

    # FFT using the filtered signal
    y = np.array(filtered)
    n = len(y)
    frq = np.fft.rfftfreq(n, d=avg_interval)            # frequency bins using real average interval
    Y = np.fft.rfft(y) / n                              # normalized FFT (rfft for real signals)

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t_sec, y, 'k')
    ax1.set_xlabel('Time (s) since program start')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'IIR Filtered Signal  |  Sample Rate: {sample_rate:.2f} Hz')

    ax2.loglog(frq[1:], abs(Y[1:]), 'k')               # skip DC bin (index 0) for loglog
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')

    plt.tight_layout()
    plt.show()

    ser.close()