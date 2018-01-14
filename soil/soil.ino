#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define pot A0 
#define WLAN_SSID       "Anu" // enter your WiFi SSID
#define WLAN_PASS       "00000000" // this is your WiFi password

/************************* Adafruit.io Setup *********************************/

#define MQTT_SERVER      "172.20.10.2" // change this to your Pi IP_address
#define MQTT_SERVERPORT  1883                   // use 8883 for SSL
#define MQTT_USERNAME    "" // empty
#define MQTT_KEY         "" // empty
 
/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** data stream publish/subscribe ***************************************/

Adafruit_MQTT_Publish pot_stream = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/esp/pot"); // change the topic

/*************************** Sketch Code ************************************/
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(pot,INPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

}
uint32_t x=0;
void loop() {
  MQTT_connect();

  int pot_value = analogRead(pot); 
  Serial.print("pot read: ");
  Serial.println(pot_value,DEC); 
  pot_stream.publish(pot_value);
  delay(1000);
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000); 
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
