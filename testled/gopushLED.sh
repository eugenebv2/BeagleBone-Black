config-pin P9_16 gpio
config-pin P9_14 gpio
gcc -o pushLED pushLED.c -lgpiod
./pushLED
