# hassio_mqtt_ir_blaster
# This Project could replace some of your current ir remotes.
I've replaced 2 remotes(Lg TV & Harman/Kardon AVR 300 RDS.
The code is very specific for my use case but could be modified to fit your needs.
I've added the mqtt commands to hassio and created buttons to control it via App. The code for Home assistant is in the Hassio.yaml
# Hardware
First connect the NodeMCU to the Pins like I did, or change them in the CODE.

<img src="https://github.com/Chaosflori25/hassio_mqtt_ir_blaster/blob/master/SC1.jpg" alt="Bord Manager" width=500> 

# Code
Then install the Librarys that you haven't installed and the ESP8266 boardmanager:

*Esp8266 Board Manager Github*

*https://github.com/esp8266/Arduino*

*Esp Boardmanager Link*

*https://arduino.esp8266.com/stable/package_esp8266com_index.json*

*IRremoteESP8266.h*

*PubSubClient.h*

Seach PubSubClient.h in your System and change the entry in line 26(#define MQTT_MAX_PACKET_SIZE) to 512

*IRsend.h*

Edit the entries for wifi mqtt server, etc.

Then upload the code to the ESP

To add this to home assitant add

https://www.home-assistant.io/addons/configurator

to your addons. To open the configuration.yaml press the folder icon in the top left corner and click on the configuration.yaml.

Edit the Ip adress and username aswell as the topics switch names, etc.

You can change the icon that Home Assistant shows to any icon you like in the MDI Icon Library

https://materialdesignicons.com/

<img src="https://github.com/Chaosflori25/hassio_mqtt_ir_blaster/blob/master/SC2.jpg" alt="Bord Manager" width=800>

# Questions
Please create an issue if you have any questions/suggestions for improvements or if you find any bugs!

Hope you like the program as much as I've enjoyed creating it!

Many thanks,

Chaosflori25


