// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

/******************************************************************************
 * @file    roomba spark firmware
 * @author  Pierre Barralon - INRIA
 * @date    28 oct. 2015   
 *
 * @brief Spark core firmware controlling a fan over WIFI
 *
 * Project : fan_wifi - https://github.com/OpHaCo/fan_wifi
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 *
 * Revision History:
 * Refer https://github.com/OpHaCo/fan_wifi
 *
 * LICENSING
 * fan_wifi (c) by Pierre Barralon
 *
 * fan_wifi is licensed under a
 * Creative Commons Attribution 3.0 Unported License.
 *
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by/3.0/>.
 *****************************************************************************/


int vit1 = D3;      //
int vit2 = D4;      // commande relay
int vit3 = D5;      //
int push1 = D0;             //
int push2 = D1;             //  buton
int push3 = D2;             //

typedef enum{
    OFF = 0,
    SPEED1 = 1,
    SPEED2 = 2,
    SPEED3 = 3
}EFanState;

void connectMQTT(void);
int fanControl(String command);
void stopFan(void);
void setFanSpeed1(void);
void setFanSpeed2(void);
void setFanSpeed3(void);
void returnstate(void);
bool isStateOk(bool state, int pin, int nbChecks);
void mqttCallback(char* topic, byte* payload, unsigned int length);

int statu = OFF;

//iot.eclipse.org
//byte mqttserver[] = { 198,41,30,241 }; 
// glados
byte mqttserver[] = { 192,168,130,22 };
MQTT mqttClient(mqttserver, 1883, mqttCallback);

char* mqttFanCmdsTopic = "fan/fanCmds";
char* mqttFanCloudTopic = "fan/particleCloud";

SYSTEM_MODE(MANUAL);

void connectMQTT(void)
{
   Serial.println("connect mqtt");
   mqttClient.connect("fan_spark"); 
   if (mqttClient.isConnected()) 
   {
       // Now a single roomba can be connected at a time
       mqttClient.subscribe(mqttFanCmdsTopic);
       mqttClient.subscribe(mqttFanCloudTopic);
       Serial.println("connected to mqtt");
   }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start fan control");
    Particle.function("fanAPI", fanControl); // Expose the fan function to the Spark API
    
    pinMode(vit1, OUTPUT);                  // sets the pins as output
    pinMode(vit2, OUTPUT);                  // sets the pins as output
    pinMode(vit3, OUTPUT);                  // sets the pins as output
    pinMode(push1, INPUT_PULLUP);           // sets the pins as input
    pinMode(push2, INPUT_PULLUP);           // sets the pins as input
    pinMode(push3, INPUT_PULLUP);           // sets the pins as input
     
    Particle.variable("state", statu);
    connectMQTT();
    Particle.connect();
    attachPushIt();
}

void loop()
{
    static int loopIndex;
    loopIndex++;
    if (mqttClient.isConnected()) {
        mqttClient.loop(); 
    }
    else
    {
        Serial.print("mqtt client not connected - id=");
        Serial.println(loopIndex);
        connectMQTT();
     }  
     if(Particle.connected())
       Particle.process();
}

bool isStateOk(bool state, int pin, int nbChecks)               //check the input is stable
{
    int compteur=0;
    for (compteur=0 ; compteur < nbChecks ; compteur++){
       if(digitalRead(pin) != state){
           return false;
       }
       delay(1);
    }
    return true;
}

void push1Change(void)                          // interupt buton1 change
{
    if(!isStateOk(digitalRead(push1), push1, 10))       //check push1 is stable
       return;
    
    if ((digitalRead(push1)==0) & (digitalRead(push2)==1) & (digitalRead(push3)==1)){
        setFanSpeed1();
    }
    else if( (digitalRead(push1)==1) & (digitalRead(push2)==1) & (digitalRead(push3)==1) ){
        stopFan();
    }
}

void push2Change(void)                          // interupt buton2 change
{
    if(!isStateOk(digitalRead(push2), push2, 10))       //check push2 is stable
       return;
    
    if ((digitalRead(push1)==1) & (digitalRead(push2)==0) & (digitalRead(push3)==1)){
        setFanSpeed2();
    }
    else if( (digitalRead(push1)==1) & (digitalRead(push2)==1) & (digitalRead(push3)==1) ){
        stopFan();
    }
}

void push3Change(void)                          // interupt buton3 change
{
    if(!isStateOk(digitalRead(push3), push3, 10))       //check push3 is stable
       return;
    
    if ((digitalRead(push1)==1) & (digitalRead(push2)==1) & (digitalRead(push3)==0)){
        setFanSpeed3();
    }
    else if( (digitalRead(push1)==1) & (digitalRead(push2)==1) & (digitalRead(push3)==1) ){
        stopFan();
    }
}

void detachPushIt(void)             //disable interupt function
{
    detachInterrupt(push1);
    detachInterrupt(push2);
    detachInterrupt(push3);
}
void attachPushIt(void)             //enable interupt function
{
    attachInterrupt(push1, push1Change, CHANGE);
    attachInterrupt(push2, push2Change, CHANGE);
    attachInterrupt(push3, push3Change, CHANGE);
}

void stopFan(void){                 //poweroff function
    digitalWrite(vit1, LOW);
    digitalWrite(vit2, LOW);
    digitalWrite(vit3, LOW);
    statu = OFF;
}
void setFanSpeed1(void){            //set Speed1 function
    detachPushIt();             //disable interupt
    digitalWrite(vit1, HIGH);
    digitalWrite(vit2, LOW);
    digitalWrite(vit3, LOW);
    statu = SPEED1;
    delay(50);
    attachPushIt();             //enable interupt
}
void setFanSpeed2(void){            //set Speed1 function
    detachPushIt();             //disable interupt
    digitalWrite(vit1, LOW);
    digitalWrite(vit2, HIGH);
    digitalWrite(vit3, LOW);
    statu = SPEED2;
    delay(50);
    attachPushIt();             //enable interupt
}
void setFanSpeed3(void){            //set Speed1 function
    detachPushIt();             //disable interupt
    digitalWrite(vit1, LOW);
    digitalWrite(vit2, LOW);
    digitalWrite(vit3, HIGH);
    statu = SPEED3;
    delay(50);
    attachPushIt();             //enable interupt
}

int fanControl(String command)                      //command wifi
{
    if(command.substring(0,8) == "POWEROFF")        //poweroff command
    {
        detachPushIt();             //disable interupt
        stopFan();
        delay(50);
        attachPushIt();             //enable interupt
        return 1;
    }
    
     if(command.substring(0,6) == "SPEED1")        //set speed1 command
    {
        setFanSpeed1();
        return 1;
    }
    if(command.substring(0,6) == "SPEED2")        //set speed2 command
    {
        setFanSpeed2();
        return 1;
    }
    if(command.substring(0,6) == "SPEED3")        //set speed3 command
    {
        setFanSpeed3();
        return 1;
    }
    
    else
    {
        return -1;
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    Serial.print("received MQTT payload ");
    Serial.print(message);
    Serial.print(" on topic ");
    Serial.println(topic);
    if(strcmp(topic, mqttFanCloudTopic) == 0)
    {
        if (message.equals("ENABLE") && !Particle.connected()){
            Particle.connect();
        }
        else if(message.equals("DISABLE") && Particle.connected())      
        {
            Particle.disconnect();
        }
    }
    else if(strcmp(topic, mqttFanCmdsTopic) == 0)
    {
        fanControl(message);
    }
}