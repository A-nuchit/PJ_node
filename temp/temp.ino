#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>

#define DHTPIN1 D1
#define DHTPIN2 D2
#define DHTTYPE DHT22
 
#define WLAN_SSID       "Anu" // enter your WiFi SSID
#define WLAN_PASS       "00000000" // this is your WiFi password

/************************* Adafruit.io Setup *********************************/

#define MQTT_SERVER      "172.20.10.2" // change this to your Pi IP_address
#define MQTT_SERVERPORT  1883                   // use 8883 for SSL
#define MQTT_USERNAME    "" // empty
#define MQTT_KEY         "" // empty

DHT dht_1(DHTPIN1, DHTTYPE);
DHT dht_2(DHTPIN2, DHTTYPE);
 
/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** data stream publish/subscribe ***************************************/

Adafruit_MQTT_Publish pot_stream_1= Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/esp/hum_1");
Adafruit_MQTT_Publish pot_stream_2= Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/esp/temp_1");// change the topic
Adafruit_MQTT_Publish pot_stream_3= Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/esp/hum_2");
Adafruit_MQTT_Publish pot_stream_4= Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/esp/temp_2");// change the topic

/*************************** Sketch Code ************************************/
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
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

 float hum_1 = dht_1.readHumidity();
 float temp_1= dht_1.readTemperature();
 float hum_2 = dht_2.readHumidity();
 float temp_2= dht_2.readTemperature();
  Serial.print("pot hum_1: ");
  Serial.println(hum_1,DEC); 
  Serial.print("pot tmp_1: ");
  Serial.println(temp_1,DEC);
  Serial.print("pot hum_2: ");
  Serial.println(hum_2,DEC); 
  Serial.print("pot tmp_2: ");
  Serial.println(temp_2,DEC);
  
  pot_stream_1.publish(hum_1);
  pot_stream_2.publish(temp_1);
  pot_stream_3.publish(hum_2);
  pot_stream_4.publish(temp_2);
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
