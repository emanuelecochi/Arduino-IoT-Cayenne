// This example shows how to connect to Cayenne using an Ethernet W5100 shield and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernet.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 3
#define LED_PIN 4
#define LIGHT_CHANNEL 0
#define DHT22_CHANNEL 0
#define DHTTYPE DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Cayenne.begin(username, password, clientID);
  dht.begin();
}

void loop() {
  Cayenne.loop(); 
}

CAYENNE_OUT_DEFAULT()
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if(millis() - lastMillis > 5000) {
    lastMillis = millis();
    sensors_event_t event;  
    dht.temperature().getEvent(&event);
    Cayenne.celsiusWrite(1, event.temperature);
    dht.humidity().getEvent(&event);
    Cayenne.virtualWrite(2, event.relative_humidity, "rel_hum", "p");
  }
}

CAYENNE_IN(LIGHT_CHANNEL)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", LIGHT_CHANNEL, LED_PIN, value);
  // Write the value received to the digital pin.
  digitalWrite(LED_PIN, value);
}
