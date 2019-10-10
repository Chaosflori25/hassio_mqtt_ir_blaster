/******************  LIBRARY SECTION *************************************/
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <PubSubClient.h>
#include <IRsend.h>
#include <Arduino.h>
#include <ArduinoJson.h>

IRsend irsend(5);

/*****************  WIFI AND CLOUDMQTT SETUP *****************************/
#define wifi_ssid "BVSchmitz intern1"
#define wifi_password "W3are54family"
#define mqtt_server "192.168.178.5"
#define mqtt_user "admin"
#define mqtt_password "superkrassesMqttpwd"
#define mqtt_client_name "sportraum_ir_remote"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

/*****************  GLOBAL VARIABLES  ************************************/


#define TV_onoff "button/tv_state"
#define TV_input "switch/tv_input"
#define TV_ok "ircode"
#define AV_Input "switch/av_input"
#define AV_reciever "button/av_off"
#define AV_Input "tv_input_tv"
bool boot = true;

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

void reconnect() {
  // Loop until we're reconnected
      if(WiFi.status() != WL_CONNECTED)
    {
      setup_wifi();
    }

      
      while (!client.connected()) {
       Serial.print("Attempting MQTT connection...");
         // Attempt to connect
      if (client.connect(mqtt_client_name, mqtt_user, mqtt_password)) {
/**********Test*****/        
       Serial.println("MQTT connected");
      if(boot == true)
      {
        client.publish("ADAFRUIT_USER/feeds/status","Rebooted");
        boot = false;
      }
      if(boot == false)
      {
        client.publish("ADAFRUIT_USER/feeds/status","Reconnected"); 
      }
      client.subscribe("ADAFRUIT_USER/feeds/bells");
    } 
  }

    } else {

      Serial.print("MQTT failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}


void callback(char* topic, byte * payload, unsigned int length) {
    
    if (strcmp(topic, "tv_state") == 0) {
      irsend.sendNEC(0x20DF10EF, 32);// LG Power
      Serial.println("tv_state = 1");
      delay(100);
    }

    if (strcmp(topic, "tv_input") == 0) {
      irsend.sendNEC(0x20DFD02F, 32); // LG Input
      delay(300);
      irsend.sendNEC(0x20DFD02F, 32);// LG Input
      delay(300);
      irsend.sendNEC(0x20DF22DD, 32);// LG OK
      Serial.println("tv_input = 1");
      delay(100);
    }

    if (strcmp(topic, "av_input") == 0) {
      Serial.println("av_input = 1");
      delay(200);
    }


    if (strcmp(topic, "av_turn_off") == 0) {
      irsend.sendNEC(0x10EF906, 32);// AV Power
      Serial.println("av_turn_off = 1");
      delay(100);
    }
  }
/****************   Setup  ***********************************************/
void setup() {
  Serial.begin(115200);
  pinMode(IRpin, OUTPUT);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
/****************   Loop   ***********************************************/
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  if (client.connected()) {
    client.subscribe("Sportraum/Tv_state");
    client.subscribe("Sportraum/Tv_input");
    client.subscribe("Sportraum/Av_input");
    client.subscribe("Sportraum/Av_turn_off");
    callback;
    }
}
