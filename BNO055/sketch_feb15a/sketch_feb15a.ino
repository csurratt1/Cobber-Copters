#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(9600);
  while (!Serial);
// Have to use GP4 and GP5 pins on Pico
  Wire.begin();

  if (!bno.begin()) {
    Serial.println("Failed to initialize BNO055!");
    while (1);
  }

  Serial.println("Accelerometer initialized!");
  Serial.println("Acceleration in m/s^2");
  Serial.println("X\tY\tZ");
}

void loop() {
  sensors_event_t accelEvent;
  bno.getEvent(&accelEvent, Adafruit_BNO055::VECTOR_ACCELEROMETER);

  float x = accelEvent.acceleration.x;
  float y = accelEvent.acceleration.y;
  float z = accelEvent.acceleration.z;

  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);
  Serial.print('\t');
  Serial.println(z);

  // Threshold checks (converted to ~1G ≈ 9.8 m/s^2)
  if (x > 9.0 || x < -9.0) {
    Serial.println("X threshold met");
    delay(1000);
  }
  if (y > 9.0 || y < -9.0) {
    Serial.println("Y threshold met");
    delay(1000);
  }
  if (z > 9.0 || z < -9.0) {
    Serial.println("Z threshold met");
    delay(1000);
  }

  delay(500);
}
