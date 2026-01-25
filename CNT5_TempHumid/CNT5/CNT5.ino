#include "DHT.h"

// CNT5/DHT11 wiring
#define DHTPIN 28        // GPIO 28 (physical pin ~34–36 area)
#define DHTTYPE DHT11    // CNT5 behaves like a DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Give the sensor time to stabilize

  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor read failed");
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %  |  Temp: ");
    Serial.print(t);
    Serial.println(" °C");
  }

  delay(2000);  // DHT11 must be read slowly
}
