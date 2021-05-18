#include "WiFi.h"
#include <PubSubClient.h> //Open-source library that enables MQTT communication between board and RabbitMQ
#include <WiFiClient.h>

const char* SSID = "********";
const char* WiFiPassword = "*******";

const char* mqtt_server = "192.168.0.51";
const char* mqtt_user = "smart-car5";
const char* mqtt_pass = "admin";


WiFiClient espClient;
PubSubClient client(espClient);

void ConnectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting to "); Serial.println(SSID);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}




void setup() {
  Serial.begin(115200);
  ConnectToWiFi();
  client.setServer(mqtt_server, 1883); // Port 1883 = MQTT
}

void loop() {
  if (!client.connected()) { //If disconnected from RabbitMQ, reconnect - constantly check this UNCOMMENT FOR LAB
    reconnect();
  }
  mqtt();
}

void mqtt() {
  client.loop();
  if(client.publish("car_mqtt", "Speed = 30mph", 10000)){ //Publishing to RabbitMQ under topic 'car_mqtt'
    Serial.println("Published messages succcessfully");
  }
  else {
    Serial.println("Could not send message :(");
  }
  delay(4000);
}

void reconnect() {
   // Loop until we're reconnected
   Serial.println("In reconnect...");
   while (!client.connected()) {
     Serial.print("Attempting MQTT connection...");
	 
     // Attempt to connect
     if (client.connect("smart_car5", mqtt_user, mqtt_pass)) { //User credentials per smart vehicle
      Serial.println("connected");
     } else {
       Serial.print("failed, rc=");
       Serial.print(client.state());
       Serial.println(" try again in 5 seconds");
       Serial.println("Debug");
       delay(5000);
     }
   }
 }
 
 