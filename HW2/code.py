import board
import time
import pwmio

motor = pwmio.PWMOut(board.GP16, variable_frequency=True)
motor.frequency = 50

while True:
    for i in range(5, 175, 1):
        motor.duty_cycle = (int)((0.02 + i*0.11/180.0)*65535)# 32768 is 50% duty cycle
        print('Running')
        time.sleep(0.01)
    for i in range(175, 5, -1):
        motor.duty_cycle = (int)((0.02 + i*0.11/180.0)*65535)# 32768 is 50% duty cycle
        print('Running')
        time.sleep(0.01)


