#include "DHT.h"

// CNT5/DHT11 wiring
#define DHTPIN 28        // GPIO 28 (physical pin ~34–36 area)
#define DHTTYPE DHT11    // CNT5 behaves like a DHT11
#define TRIG_PIN 14
#define ECHO_PIN 15  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // Give the sensor time to stabilize
  delay(2000);
}

long readUltrasonic() {
  // Make sure trigger starts LOW
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send 10 µs HIGH pulse to trigger the sensor
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure how long ECHO stays HIGH (timeout = 30 ms)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // If no echo was received (timeout)
  if (duration == 0) {
    Serial.println("No echo received");
    return -1;
  }

  // Convert time to distance (cm)
  long distance_cm = duration / 58;
Serial.print("distance is ");
Serial.print(distance_cm);
Serial.println(" cm");

  return distance_cm;
}

void readDHT() {
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



void loop() {
  readDHT();
  readUltrasonic();

  delay(2000);  // DHT11 must be read slowly
}
