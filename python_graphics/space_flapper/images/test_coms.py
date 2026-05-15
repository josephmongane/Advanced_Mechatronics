
import time
import serial

ser = serial.Serial(
    '/dev/tty.usbmodem101',
    baudrate=115200,   # must match your device
    timeout=1
)

# Clear old buffered data
ser.reset_input_buffer()

while True:

    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        print(f"Returned number = {line}")

    time.sleep(0.01)
