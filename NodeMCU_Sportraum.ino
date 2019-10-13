/******************  LIBRARY SECTION *************************************/
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <IRsend.h>

const uint16_t kIrLed = 14;  // ESP GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);

/*****************  WIFI AND CLOUDMQTT SETUP *****************************/
#define wifi_ssid "ssid"
#define wifi_password "pwd"
#define mqtt_server "Ip"
#define mqtt_user "user"
#define mqtt_password "pwd"
#define mqtt_client_name "name"
#define Relais Relais Pin // I wanted an Relay to control an other thing


WiFiClient espClient;
PubSubClient client(espClient);


/*****************  SYSTEM FUNCTIONS  *************************************/

void setup_wifi() {
  delay(10);
  // Connecting to Wifi:
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/*****************  RECONNECT  ********************************************/
void reconnect() {
  // Loop until we're reconnected
      if(WiFi.status() != WL_CONNECTED)
    {
      setup_wifi();
    }

      
      while (!client.connected()) {
       Serial.print("Attempting MQTT connection...");
         // Attempt to connect
      if (client.connect(mqtt_client_name, mqtt_user, mqtt_password)) 
    {    
       Serial.println("MQTT connected");
       client.subscribe("Sportraum/#");
    } 
    else {

      Serial.print("MQTT failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}



/*****************  TOPICS  ********************************************/

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 /****TOPIC REQUEST THROUGH STRCMP****/
 if ((strcmp(topic, "Sportraum/Tv_state") == 0) && (payload[0] == '1')) {
    irsend.sendNEC(0x20DF10EF, 32);// LG Power ON
    Serial.println("1_On");
   }
  if ((strcmp(topic, "Sportraum/Tv_state") == 0) && (payload[0] == '0')) {
    irsend.sendNEC(0x20DF10EF, 32);// LG Power OFF
    Serial.println("1_Off");
   }
  if ((strcmp(topic, "Sportraum/Tv_input") == 0) && (payload[0] == '1')) {
    Serial.println("2_On");
    irsend.sendNEC(0x20DFD02F, 32); // LG Input Select
    delay(500);
    irsend.sendNEC(0x20DFD02F, 32);// LG Input Select
    delay(500);
    irsend.sendNEC(0x20DF22DD, 32);// LG OK
    Serial.println("tv_input = 1");
    delay(100);   
   }
  if ((strcmp(topic, "Sportraum/Tv_input") == 0) && (payload[0] == '0')) {
    Serial.println("2_0ff");
    irsend.sendNEC(0x20DFD02F, 32); // LG Input Select
    delay(500);
    irsend.sendNEC(0x20DFD02F, 32);// LG Input Select
    delay(500);
    irsend.sendNEC(0x20DF22DD, 32);// LG OK
    delay(100);   
   }
   if ((strcmp(topic, "Sportraum/Av_input") == 0) && (payload[0] == '1')) {  //Input Tape
    Serial.println("3_Inp.");
      irsend.sendNEC(0x10E53AC, 32); // HK Video 1
      delay(750);
      irsend.sendNEC(0x414E2AD5, 32);// HK DIGI.Sel.
      delay(500);
      irsend.sendNEC(0x414E59A6, 32);// HK DOWN
      delay(500);
      irsend.sendNEC(0x414E59A6, 32);// HK DOWN
      delay(500);
      irsend.sendNEC(0x414E21DE, 32);// HK OK
      irsend.sendNEC(0x414E21DE, 32);// HK OK
      irsend.sendNEC(0x414E21DE, 32);// HK OK
      delay(400);
      irsend.sendNEC(0x10E33CC, 32); // HK Tape
      delay(750);   
   }
  if ((strcmp(topic, "Sportraum/Av_input") == 0) && (payload[0] == '2')) {   //Input Video
    Serial.println("3_Inp.2");
    irsend.sendNEC(0x10E53AC, 32); // HK Video 1
    delay(750);
    irsend.sendNEC(0x414E2AD5, 32);// HK DIGI.Sel.
    delay(400);
    irsend.sendNEC(0x414E718E, 32);// HK Up
    delay(400);
    irsend.sendNEC(0x414E718E, 32);// HK UP
    delay(400);
    irsend.sendNEC(0x414E21DE, 32);// HK OK
    irsend.sendNEC(0x414E21DE, 32);// HK OK
    irsend.sendNEC(0x414E21DE, 32);// HK OK
    delay(750);
   }
   if ((strcmp(topic, "Sportraum/Av_turnoff") == 0) && (payload[0] == '1')) {
    Serial.println("4_On");
    irsend.sendNEC(0x10E53AC, 32); // HK Video 1
    delay(500);   
   }
  if ((strcmp(topic, "Sportraum/Av_turnoff") == 0) && (payload[0] == '0')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10EF906, 32);// AV Power
   }
   if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '+')) {
    Serial.println("4_On");
    irsend.sendNEC(0x10EE31C, 32); // HK Video 1 to Wake up Reciever
    delay(500);   
   }
  if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '-')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(500);
  }
  if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '5')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +5
    delay(500);
  }
    if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '+')) {
    Serial.println("4_On");
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(500);   
   }
   if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '6')) {
    Serial.println("4_On");
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(100);
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -5
    delay(500);   
   }
  if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '2')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +1
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +1
    delay(500);
   }
   if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '3')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +1
    delay(100);
    irsend.sendNEC(0x10E13EC, 32);// AV Vol +1
    delay(500);
   }
  if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '0')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -1
    delay(100);
    irsend.sendNEC(0x10EE31C, 32);// AV Vol -1
    delay(500);
  }
    if ((strcmp(topic, "Sportraum/Av_volume") == 0) && (payload[0] == '1')) {
    Serial.println("4_Off");
    irsend.sendNEC(0x10EE31C, 32); // AV Vol -1
    delay(100);
    irsend.sendNEC(0x10EE31C, 32);// AV Vol -1
    delay(500);
 }
 if ((strcmp(topic, "Sportraum/Av_speakers") == 0) && (payload[0] == '0')) {
    Serial.println("5_Off");
    digitalWrite(Relais, HIGH);
    delay(100);
  }
    if ((strcmp(topic, "Sportraum/Av_speakers") == 0) && (payload[0] == '1')) {
    Serial.println("5_On");
    digitalWrite(Relais, LOW);
    delay(100);
 }
}


/****************   Setup  ***********************************************/
void setup() {
  Serial.begin(115200);
  pinMode(Relais, OUTPUT); 
  irsend.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}
/****************   Loop   ***********************************************/
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
