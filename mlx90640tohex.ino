#include <Wire.h>

#define MLX_ADDR 0x33

// Teensy Wire pins are SDA=18, SCL=19 by default
// If you're on Wire1, change Wire -> Wire1 everywhere and use SDA=17, SCL=16.

static bool readWords16(uint8_t addr7, uint16_t reg, uint16_t *out, uint16_t words) {
  Wire.beginTransmission(addr7);
  Wire.write((uint8_t)(reg >> 8));
  Wire.write((uint8_t)(reg & 0xFF));

  // repeated start
  uint8_t tx = Wire.endTransmission(false);
  if (tx != 0) return false;

  uint16_t bytes = words * 2;
  uint16_t got = Wire.requestFrom((int)addr7, (int)bytes, (int)true);
  if (got != bytes) return false;

  for (uint16_t i = 0; i < words; i++) {
    uint8_t hi = Wire.read();
    uint8_t lo = Wire.read();
    out[i] = ((uint16_t)hi << 8) | lo;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Wire.begin();
  Wire.setClock(400000);
  Wire.setTimeout(50);

  Serial.println("\nMLX90640 simple hex probe");

  // Quick ACK check
  Wire.beginTransmission(MLX_ADDR);
  uint8_t ack = Wire.endTransmission(true);
  Serial.print("ACK at 0x33? endTransmission = ");
  Serial.println(ack); // 0 = OK, nonzero = fail
}

void loop() {
  uint16_t w;

  // Status 0x8000
  bool okStatus = readWords16(MLX_ADDR, 0x8000, &w, 1);
  Serial.print("0x8000 STATUS: ");
  if (okStatus) {
    Serial.print("0x");
    Serial.println(w, HEX);
  } else {
    Serial.println("READ FAIL");
  }

  // Control1 0x800D
  bool okCtrl = readWords16(MLX_ADDR, 0x800D, &w, 1);
  Serial.print("0x800D CTRL1 : ");
  if (okCtrl) {
    Serial.print("0x");
    Serial.println(w, HEX);
  } else {
    Serial.println("READ FAIL");
  }

  // RAM sample 0x0400, 16 words
  uint16_t ram[16];
  bool okRam = readWords16(MLX_ADDR, 0x0400, ram, 16);
  Serial.print("0x0400 RAM[0..15]: ");
  if (okRam) {
    for (int i = 0; i < 16; i++) {
      if (i) Serial.print(' ');
      if (ram[i] < 0x1000) Serial.print('0');
      if (ram[i] < 0x0100) Serial.print('0');
      if (ram[i] < 0x0010) Serial.print('0');
      Serial.print(ram[i], HEX);
    }
    Serial.println();
  } else {
    Serial.println("READ FAIL");
  }

  Serial.println("----");
  delay(1000);
}
