from time import sleep
import serial

ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    sleep(3)
    ser.write(b'345\n')
    sleep(2)
    ser.write(b'454556\n')