import Adafruit_BBIO.PWM as PWM
import time

time.sleep(2)

while True:
    for i in range(0,101,1):
        PWM.start("P9_14",i)
        time.sleep(.0)
