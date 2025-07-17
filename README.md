# BeagleBone-Black

   Static hostname: BeagleBone
   
         Icon name: computer-embedded
         
           Chassis: embedded
           
        Machine ID: 454bbe9c893a4e109cb1317d7bde7228
        
           Boot ID: fe954291367040828aac4ce6702a8503
           
  Operating System: Debian GNU/Linux 11 (bullseye)
  
            Kernel: Linux 5.10.168-ti-r73
            
      Architecture: arm
      
With Grove Base Cape

<hr>

Install Libmosquitto. ( sudo apt-get install mosquitto mosquitto-dev libmosquitto* ) Include mosquitto service , but not use the service. Refenence Libmosquitto Functions from https://mosquitto.org/api/files/mosquitto-h.html 

git Clone from <a href="https://github.com/positronic57/MQTT_examples_with_libmosquito" target="_blank"> MQTT_examples_with_libmosquito </a>. And test mqtt_sub example. 

Base on this change the i2c/testbmp280 to send data using MQTT publish.

<hr>

![alt text][def1]

[def1]: images/IMG_4914.jpg

![alt text][def2]

[def2]: images/IMG_4915.jpg
