#include <iostream>
#include <unistd.h> //for usleep
#include "GPIO.h"
using namespace exploringBB;
using namespace std;

int main(){
   GPIO outGPIO(51), inGPIO(50);

   // Basic Output - Flash the LED 10 times, once per second
   outGPIO.setDirection(OUTPUT);
   //for (int i=0; i<10; i++)
   while(1){
       outGPIO.setValue(HIGH);
       usleep(1000000); //micro-second sleep 0.5 seconds
       outGPIO.setValue(LOW);
       usleep(1000000);
   }
   // Basic Input example
   inGPIO.setDirection(INPUT);
   cout << "The value of the input is: "<< inGPIO.getValue() << endl;

   // Fast write to GPIO 1 million times
   outGPIO.streamOpen();
   for (int i=0; i<1000000; i++){
       outGPIO.streamWrite(HIGH);
       outGPIO.streamWrite(LOW);
   }
   outGPIO.streamClose();
   return 0;
}
