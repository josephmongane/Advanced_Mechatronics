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
            t.append(int(parts[0]))
            data.append(int(parts[1]))
            filtered.append(float(parts[2]))

    # Calculate sample rate from actual timestamps
    t_sec = np.array(t) / 1000.0
    intervals = np.diff(t_sec)
    avg_interval = np.mean(intervals)
    sample_rate = 1.0 / avg_interval

    print(f"Calculated sample rate: {sample_rate:.2f} Hz")
    for i in range(len(t)):
        print(f"{t[i]}, {data[i]}, {filtered[i]}")

    # FFT for both signals
    y_raw = np.array(data)
    y_filt = np.array(filtered)
    n = len(y_raw)
    frq = np.fft.rfftfreq(n, d=avg_interval)

    Y_raw  = np.fft.rfft(y_raw) / n
    Y_filt = np.fft.rfft(y_filt) / n

    # 2x2 grid: top row = time domain, bottom row = frequency domain
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 7))

    # Raw time domain
    ax1.plot(t_sec, y_raw, 'k')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Raw Signal')

    # Filtered time domain
    ax2.plot(t_sec, y_filt, 'b')
    ax2.set_xlabel('Time (s)')
    ax2.set_ylabel('Amplitude')
    ax2.set_title('IIR Filtered Signal')

    # Raw FFT
    ax3.loglog(frq[1:], abs(Y_raw[1:]), 'k')
    ax3.set_xlabel('Freq (Hz)')
    ax3.set_ylabel('|Y(freq)|')
    ax3.set_title('FFT - Raw Signal')

    # Filtered FFT
    ax4.loglog(frq[1:], abs(Y_filt[1:]), 'b')
    ax4.set_xlabel('Freq (Hz)')
    ax4.set_ylabel('|Y(freq)|')
    ax4.set_title('FFT - Filtered Signal')

    fig.suptitle(f'Sample Rate: {sample_rate:.2f} Hz  |  N = {num_samples} samples', fontsize=13)
    plt.tight_layout()
    plt.show()

    ser.close()