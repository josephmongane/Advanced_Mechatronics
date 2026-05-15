
import time
import serial
ser = serial.Serial('/dev/tty.usbmodem1101') # the name of your port here

print("Test 1...")

while(True):
    print("Test 1...\n")
    time.sleep(1)

