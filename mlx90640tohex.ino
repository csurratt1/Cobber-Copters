#include <Wire.h>
#include <SPI.h>
#include <ILI9341_t3n.h>
#include <Adafruit_MLX90640.h>

// ================= DISPLAY =================
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

ILI9341_t3n tft(TFT_CS, TFT_DC, TFT_RST);

// ================= MLX90640 =================
Adafruit_MLX90640 mlx;
float frame[32 * 24];   // 768 pixels

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== MLX90640 + ILI9341 TEST ===");

  // ---- TFT INIT ----
  tft.begin();
  tft.setRotation(3);        // Landscape
  tft.useFrameBuffer(true);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.println("Starting...");
  tft.updateScreen();

  // ---- I2C INIT ----
  Wire.begin();
  Wire.setClock(100000);   // 1 MHz REQUIRED for MLX90640

  // ---- MLX INIT ----
  if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("❌ MLX90640 NOT FOUND");
    tft.fillScreen(ILI9341_RED);
    tft.setCursor(20, 100);
    tft.println("MLX NOT FOUND");
    tft.updateScreen();
    while (1);
  }

  Serial.println("✅ MLX90640 detected");

  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_8_HZ);

  tft.fillScreen(ILI9341_BLACK);
  tft.updateScreen();
}

// ================= LOOP =================
void loop() {

  if (!mlx.getFrame(frame)) {
    Serial.println("❌ Frame read failed");
    return;
  }

  // ---- FIND MIN / MAX ----
  float minT = 1000, maxT = -1000;
  for (int i = 0; i < 768; i++) {
    float t = frame[i];
    if (!isnan(t)) {
      if (t < minT) minT = t;
      if (t > maxT) maxT = t;
    }
  }

  // ---- SERIAL DEBUG ----
  Serial.print("Min: ");
  Serial.print(minT);
  Serial.print("  Max: ");
  Serial.print(maxT);
  Serial.print("  Center: ");
  Serial.println(frame[12 * 32 + 16]);

  // ---- DRAW IMAGE ----
  int pixelW = 10;
  int pixelH = 10;

  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 32; x++) {

      float t = frame[y * 32 + x];
      float norm = (t - minT) / (maxT - minT);
      norm = constrain(norm, 0.0, 1.0);

      // BLUE -> RED gradient
      uint8_t r = norm * 255;
      uint8_t g = 0;
      uint8_t b = 255 - r;

      uint16_t color = tft.color565(r, g, b);
      tft.fillRect(x * pixelW, y * pixelH, pixelW, pixelH, color);
    }
  }

  // ---- OVERLAY TEXT ----
  tft.fillRect(0, 220, 320, 20, ILI9341_BLACK);
  tft.setCursor(4, 222);
  tft.setTextSize(1);
  tft.print("Min:");
  tft.print(minT, 1);
  tft.print("  Max:");
  tft.print(maxT, 1);

  tft.updateScreen();
}
