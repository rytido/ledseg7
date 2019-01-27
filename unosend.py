from time import sleep
import serial

# sudo chmod a+rw /dev/ttyACM0
# equivalent to
# echo 345 > /dev/ttyACM0

ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    sleep(2)
    ser.write(b'345\n')
    sleep(2)
    ser.write(b'454556\n')