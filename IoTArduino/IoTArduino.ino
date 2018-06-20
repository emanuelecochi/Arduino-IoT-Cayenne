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
char username[] = "16831f80-745e-11e8-8fb7-c3a557c9635e";
char password[] = "7a1be4aa7b26b54f5b0f01e3ce781e1722d79f55";
char clientID[] = "bcb42d80-745f-11e8-8458-69a620124a87";

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
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  Cayenne.celsiusWrite(1, event.temperature);
  dht.humidity().getEvent(&event);
  Cayenne.virtualWrite(2, event.relative_humidity, "rel_hum", "p");
}

CAYENNE_IN(LIGHT_CHANNEL)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", LIGHT_CHANNEL, LED_PIN, value);
  // Write the value received to the digital pin.
  digitalWrite(LED_PIN, value);
}
