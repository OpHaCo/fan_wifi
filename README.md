# Description
Resurrect your old fan! Add Wifi connectivity and control it from anyway! 

Project has been done in Amiqual4Home Equipex Creativity Lab - https://amiqual4home.inria.fr/

# Prerequisities
 * Spark Core with latest firmware updates :
     
    ```
    sudo particle flash --usb cc3000
    sudo particle flash --usb deep_update_2014_06
    ``` 
 * 3 Relay tinkerkit
 * A fan

<img src="https://github.com/OpHaCo/fan_wifi.git/img/Fan.jpg" width="500">

# Setup
## hardware 
* For this model, there are four buttons for three speed and one stop. They are connected to the input pullup sparkcore in D0, D1 , D2 which are assosié three relays control the motor speed.
<img src="https://github.com/OpHaCo/fan_wifi.git/img/Sparkcore.jpg" width="500">
* The motor speed are control by three wire :
  * Blue = speed 1
  * White = speed 2
  * Red = speed 3
<img src="https://github.com/OpHaCo/fan_wifi.git/img/Motor.jpg" width="500">
* connect each relay on a three wire and on live wire (brown 220v),The relay are connected to the output sparkcore in D3, D4 , D5 and Vin(5v), GND.
<img src="https://github.com/OpHaCo/fan_wifi.git/img/Relay.jpg" width="500">
* for the sparkcore and the relay is supplied with 5V, install a portable charger transformer.

# Commands
    curl https://api.spark.io/v1/devices/'SPARK_CORE_ID'/fanAPI -d access_token='YOUR_TOKEN' -d "params=CMD_NAME"
 
 * CMD_NAME =
   * POWEROFF
   * SPEED1
   * SPEED2
   * SPEED3

 * For get the status :
    curl https://api.spark.io/v1/devices/'SPARK_CORE_ID'/state\?access_token\='YOUR_TOKEN'
   * state = 0 : OFF
   * state = 1 : SPEED1
   * state = 2 : SPEED2
   * state = 3 : SPEED3

     
# References
 * https://community.particle.io/t/getting-mac-address-of-unconnected-core/8473/5
 * http://blog.particle.io/2014/08/06/control-the-connection/
