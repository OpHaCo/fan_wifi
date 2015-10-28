# Description
 
# Prerequisities
 * Spark Core with latest firmware updates :
     
    ```
    sudo particle flash --usb cc3000
    sudo particle flash --usb deep_update_2014_06
    ``` 
 * 3 Relay tinkerkit
 * A fan

# Setup
## hardware 
* For this model, there are four buttons for three speed and one stop. They are connected to the input pullup sparkcore in D0, D1 , D2 which are assosié three relays control the speed.
* 

# Commands
    curl https://api.spark.io/v1/devices/SPARK_CORE_ID/fanAPI -d access_token='YOUR_TOKEN' -d "params=CMD_NAME"
 
 * CMD_NAME =
   * POWEROFF
   * SPEED1
   * SPEED2
   * SPEED3

    curl https://api.spark.io/v1/devices/SPARK_CORE_ID/fanAPI -d access_token='YOUR_TOKEN' -d "params=SPEED1"
     
# References
 * https://community.particle.io/t/getting-mac-address-of-unconnected-core/8473/5
 * http://blog.particle.io/2014/08/06/control-the-connection/
