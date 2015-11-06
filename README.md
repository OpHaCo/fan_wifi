# Description
Make your low cost fan smarter, control it from anyway! 

Project has been done in Amiqual4Home Equipex Creativity Lab - https://amiqual4home.inria.fr/

# Prerequisities
## Hardware
 * Spark Core with latest firmware updates :
     
    ```
    sudo particle flash --usb cc3000
    sudo particle flash --usb deep_update_2014_06
    ``` 
 * 3 relays
 * A fan

<img src="https://github.com/OpHaCo/fan_wifi/blob/master/img/Fan.jpg" width="500">

## software 
 * MQTT broker - (do not use mosquitto 1.4.3 - https://github.com/hirotakaster/MQTT/issues/13 ) 

# Setup
## hardware 
* For this model, there are four buttons for three speed and one stop. They are connected to the input pullup sparkcore in D0, D1 , D2 associated to 3 relays to control motor speed.

<img src="https://github.com/OpHaCo/fan_wifi/blob/master/img/Sparkcore.jpg" width="500">
* The motor speed are controlled by three wire :
  * Blue = speed 1
  * White = speed 2
  * Red = speed 3

<img src="https://github.com/OpHaCo/fan_wifi/blob/master/img/Motor.jpg" width="500">
* connect each relay on a three wire and on live wire (brown 220v),The relay are connected to the output sparkcore in D3, D4 , D5 and Vin(5v), GND.

<img src="https://github.com/OpHaCo/fan_wifi/blob/master/img/Relay.jpg" width="500">
* power supply @5V using an old phone charger.
 
# Commands

Commands can be sent either using MQTT protocol or using Particle API

## Control commands - as string
 
 * CMD_NAME =
   * POWEROFF
   * SPEED1
   * SPEED2
   * SPEED3

## Fan control over particle API

Command line syntax :

    curl https://api.spark.io/v1/devices/'SPARK_CORE_ID'/fanAPI -d access_token='YOUR_TOKEN' -d "params=CMD_NAME"

  e.g. :     
  
    curl https://api.spark.io/v1/devices/'SPARK_CORE_ID'/fanAPI -d access_token='YOUR_TOKEN' -d "params=SPEED2"

## Fan control over MQTT
 * a valid MQTT broker must be defined in Photon code mqttserver variable 

### topics
#### "fan/fanCmds" : fan control commands
In this topic, payload containing command name must be sent, 

e.g :

    mosquitto_pub -h BROKER_IP -t fan/fanCmds -m CMD_NAME
 
#### "fan/particleCloud" : fan cloud connection
To enable cloud connection
    
    mosquitto_pub -h BROKER_IP -t fan/particleCloud -m ENABLE
    
To disable cloud connection
    
    mosquitto_pub -h BROKER_IP -t fan/particleCloud -m DISABLE

## Input commands

 * To get status :

    curl https://api.spark.io/v1/devices/'SPARK_CORE_ID'/state\?access_token\='YOUR_TOKEN'

   * state = 0 : OFF
   * state = 1 : SPEED1
   * state = 2 : SPEED2
   * state = 3 : SPEED3

     
# References
 * https://community.particle.io/t/getting-mac-address-of-unconnected-core/8473/5
 * http://blog.particle.io/2014/08/06/control-the-connection/
