import Adafruit_BBIO.PWM as PWM
import time

PWM.start("P9_14",50)
PWM.set_duty_cycle("P9_14",25.5)
PWM.set_frequency("P9_14",10)
time.sleep(10)

PWM.stop("P9_14")
PWM.cleanup()
