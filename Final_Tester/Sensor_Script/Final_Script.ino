#include "DHT.h"
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SD.h>
#include <SPI.h>

// -------------------- PIN DEFINITIONS --------------------
#define DHTPIN 28
#define DHTTYPE DHT11

#define TRIG_PIN 14
<<<<<<< HEAD
#define ECHO_PIN 15
=======
#define ECHO_PIN 15  
#include  <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C Interface
>>>>>>> 0a186858604e32a9607e31eebb1c63a26185fd7d

#define SD_CS 17   // SD card chip select

// -------------------- SENSOR OBJECTS --------------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55);

File logfile;

// -------------------- DATA STRUCTURES --------------------
struct DHTData {
  float humidity;
  float temperature;
};

<<<<<<< HEAD
struct BMPData {
  float temperature;
  float pressure;
  float altitude;
};

struct BNOData {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float magX, magY, magZ;
  float heading, roll, pitch;
};

// -------------------- SENSOR FUNCTIONS --------------------
DHTData readDHT() {
  DHTData d;
  d.humidity = dht.readHumidity();
  d.temperature = dht.readTemperature();

  if (isnan(d.humidity) || isnan(d.temperature)) {
    d.humidity = NAN;
    d.temperature = NAN;
  }
  return d;
=======
  if  (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor,  check wiring!"));
    while (1);
  }

  /* Default settings from datasheet.  */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);  /* Standby time. */

  // Give the sensor time to stabilize
  delay(2000);
>>>>>>> 0a186858604e32a9607e31eebb1c63a26185fd7d
}

long readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return NAN;

  return duration / 58;
}

<<<<<<< HEAD
BMPData readBMP280() {
  BMPData b;
  b.temperature = bmp.readTemperature();
  b.pressure = bmp.readPressure() / 100.0;  // hPa
  b.altitude = bmp.readAltitude(1019.66);   // adjust for your local pressure
  return b;
=======
void readBMP280(){
  Serial.print(F("Temperature  = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure()/100);  //displaying the Pressure in hPa, you can change the unit
    Serial.println("  hPa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1019.66));  //The "1019.66" is the pressure(hPa) at sea level in day in your region
    Serial.println("  m");                    //If you don't know it, modify it until you get your current  altitude

    Serial.println();
    delay(2000);
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
>>>>>>> 0a186858604e32a9607e31eebb1c63a26185fd7d
}

BNOData readBNO() {
  BNOData d;

  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyro  = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag   = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  d.accelX = accel.x(); d.accelY = accel.y(); d.accelZ = accel.z();
  d.gyroX  = gyro.x();  d.gyroY  = gyro.y();  d.gyroZ  = gyro.z();
  d.magX   = mag.x();   d.magY   = mag.y();   d.magZ   = mag.z();
  d.heading = euler.x(); d.roll = euler.y(); d.pitch = euler.z();

  return d;
}

// -------------------- SETUP --------------------
void setup() {

  Serial.begin(115200);
  Serial.println("got here");

  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // BMP280 init
  if (!bmp.begin()) {
    Serial.println("BMP280 not found!");
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  // BNO055 init
  if (!bno.begin()) {
    Serial.println("BNO055 not found!");
    while (1);
  }

  // SD card init
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }

  logfile = SD.open("flight.csv", FILE_WRITE);
  if (!logfile) {
    Serial.println("Failed to open log file!");
    while (1);
  }

  logfile.println(
    "time_ms,humidity,temp_dht,temp_bmp,pressure,altitude,"
    "accel_x,accel_y,accel_z,"
    "gyro_x,gyro_y,gyro_z,"
    "mag_x,mag_y,mag_z,"
    "heading,roll,pitch,"
    "distance_cm"
  );
  logfile.flush();

  delay(2000);
}

// -------------------- MAIN LOOP --------------------
void loop() {
<<<<<<< HEAD
  unsigned long t = millis();
=======
  readDHT();
  readUltrasonic();
  readBMP280();
>>>>>>> 0a186858604e32a9607e31eebb1c63a26185fd7d

  DHTData dhtData = readDHT();
  long distance = readUltrasonic();
  BMPData bmpData = readBMP280();
  BNOData bnoData = readBNO();

  // Write CSV safely (no String class)
  logfile.print(t); logfile.print(",");
  logfile.print(dhtData.humidity); logfile.print(",");
  logfile.print(dhtData.temperature); logfile.print(",");
  logfile.print(bmpData.temperature); logfile.print(",");
  logfile.print(bmpData.pressure); logfile.print(",");
  logfile.print(bmpData.altitude); logfile.print(",");

  logfile.print(bnoData.accelX); logfile.print(",");
  logfile.print(bnoData.accelY); logfile.print(",");
  logfile.print(bnoData.accelZ); logfile.print(",");

  logfile.print(bnoData.gyroX); logfile.print(",");
  logfile.print(bnoData.gyroY); logfile.print(",");
  logfile.print(bnoData.gyroZ); logfile.print(",");

  logfile.print(bnoData.magX); logfile.print(",");
  logfile.print(bnoData.magY); logfile.print(",");
  logfile.print(bnoData.magZ); logfile.print(",");

  logfile.print(bnoData.heading); logfile.print(",");
  logfile.print(bnoData.roll); logfile.print(",");
  logfile.print(bnoData.pitch); logfile.print(",");

  logfile.println(distance);
  logfile.flush();

  // Optional debug
  Serial.println("Logged");

  delay(200);
}
