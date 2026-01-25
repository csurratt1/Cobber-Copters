/*
 * DHT11 Temperature & Humidity Sensor Reader
 * 
 * This Arduino sketch reads temperature and humidity data from a DHT11 sensor
 * and displays the readings on your computer via the Serial Monitor.
 * 
 * About the DHT11 Sensor:
 * - Measures temperature: 0°C to 50°C (±2°C accuracy)
 * - Measures humidity: 20% to 90% relative humidity (±5% accuracy)
 * - Uses a single digital pin for communication
 * - Low cost and easy to use for basic environmental monitoring
 * - Good for room temperature/humidity monitoring, weather stations, etc.
 * 
 * Hardware Requirements:
 * - Arduino board (Teensy, Uno, Nano, etc.)
 * - DHT11 temperature/humidity sensor module
 * - 3 wires: Power (VCC/+), Ground (GND/-), and Data (Signal)
 * 
 * Wiring:
 * - VCC/+ → 3.3V or 5V on Arduino (check your module's voltage requirement)
 * - GND/- → GND on Arduino
 * - DATA/Signal → Digital pin 28 on Arduino (or change DHTPIN below)
 * 
 * Note: Some DHT11 modules have a built-in pull-up resistor, others require
 * an external 10kΩ resistor between DATA and VCC pins.
 */

#include "DHT.h"  // DHT sensor library - handles the complex timing needed to talk to the sensor

// ========================================
// Configuration Settings
// ========================================

// Define which Arduino pin is connected to the DHT11 data line
// GPIO = General Purpose Input/Output (the numbered pins on your Arduino)
#define DHTPIN 28

// Define which type of DHT sensor you're using
// DHT11 = Basic, less accurate but cheaper
// DHT22 = More accurate, slightly more expensive (also called AM2302)
#define DHTTYPE DHT11

// Create a DHT sensor object that we'll use to read temperature and humidity
// This tells the library: "I have a DHT11 sensor connected to pin 28"
DHT dht(DHTPIN, DHTTYPE);

/**
 * setup() - Runs once when Arduino powers on or resets
 * 
 * This function initializes the communication systems so we can
 * talk to both the computer and the DHT11 sensor.
/**
 * loop() - Runs continuously after setup() completes
 * 
 * This function repeatedly reads the sensor and displays the results.
 * It runs over and over, creating a continuous stream of readings.
 */
void loop() {
  // ========================================
  // Step 1: Read the sensor data
  // ========================================
  
  // Read humidity value from the sensor
  // 'float' means decimal number (e.g., 45.5%)
  // Humidity = amount of water vapor in the air (0-100%)
  float h = dht.readHumidity();
  
  // Read temperature value from the sensor
  // Returns temperature in Celsius (°C) by default
  // To get Fahrenheit, use: dht.readTemperature(true)
  float t = dht.readTemperature();

  // ========================================
  // Step 2: Check if the reading was successful
  // ========================================
  
  // Sometimes sensors fail to read (bad connection, timing issue, etc.)
  // 'isnan' = "is Not a Number" - checks if the reading is invalid
  // If either reading failed, display an error message
  if (isnan(h) || isnan(t)) {
    Serial.println("⚠ Sensor read failed - check wiring and connections!");
    // Possible causes:
    // - Loose wire connection
    // - Wrong pin number in DHTPIN
    // - Sensor not powered
    // - Faulty sensor
  } else {
    // ========================================
    // Step 3: Display the successful readings
    // ========================================
    
    // Print the humidity reading
    Serial.print("Humidity: ");
    Serial.print(h);            // The actual humidity value
    Serial.print(" %");         // Units: percentage
    
    Serial.print("  |  ");      // Separator for readability
    
    // Print the temperature reading
    Serial.print("Temp: ");
    Serial.print(t);            // The actual temperature value
    Serial.println(" °C");      // Units: degrees Celsius (println adds a new line)
    
    // Example output: "Humidity: 45.50 %  |  Temp: 23.20 °C"
  }

  // ========================================
  // Step 4: Wait before next reading
  // ========================================
  
  // Wait 2000 milliseconds (2 seconds) before taking another reading
  // Why wait?
  // - DHT11 sensor needs time between readings (minimum 1 second)
  // - Prevents flooding the Serial Monitor with too much data
  // - Readings don't change that quickly anyway
  delay(2000);
  
  // After this delay, the loop() function automatically starts over from the top

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

  delay(2000);
}

