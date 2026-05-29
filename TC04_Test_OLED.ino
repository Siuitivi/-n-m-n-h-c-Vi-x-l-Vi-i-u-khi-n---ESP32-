// =====================================================
// TC04 - TEST OLED SSD1306
// Requirement:
// OLED SSD1306 phai hien thi duoc du lieu cam bien local
//
// Target:
// OLED SDA -> GPIO21 ESP32
// OLED SCL -> GPIO22 ESP32
// OLED I2C Address: 0x3C
// =====================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= I2C PIN =================
#define I2C_SDA 21
#define I2C_SCL 22

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bien kiem tra OLED co khoi tao duoc khong
bool oledOK = false;

// ================= DU LIEU CAM BIEN LOCAL GIA LAP =================
// Cac bien nay giong voi project goc
float localTemp = 30.5;
float localHum  = 70.0;

int rainAO = 1234;
bool rainDetected = true;

int mq6Raw = 850;
bool gasDanger = false;

// =====================================================
// HAM HIEN THI OLED
// Dua theo logic hien thi trong project goc
// =====================================================
void showOLED() {
  if (!oledOK) {
    return;
  }

  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  oled.setCursor(0, 0);
  oled.println("LOCAL SENSORS");

  oled.setCursor(0, 12);
  oled.print("Temp: ");
  oled.print(localTemp, 1);
  oled.println(" C");

  oled.setCursor(0, 22);
  oled.print("Hum : ");
  oled.print(localHum, 1);
  oled.println(" %");

  oled.setCursor(0, 32);
  oled.print("RainAO: ");
  oled.println(rainAO);

  oled.setCursor(0, 42);
  oled.print("RainDO: ");
  oled.println(rainDetected ? "WET" : "DRY");

  oled.setCursor(0, 52);
  oled.print("MQ6:");
  oled.print(mq6Raw);

  oled.print(" Gas:");
  oled.print(gasDanger ? "DANGER" : "SAFE");

  oled.display();
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TC04 - TEST OLED SSD1306");
  Serial.println("==================================");

  // Khoi tao I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.print("OLED SDA Pin: GPIO");
  Serial.println(I2C_SDA);

  Serial.print("OLED SCL Pin: GPIO");
  Serial.println(I2C_SCL);

  Serial.print("OLED Address: 0x");
  Serial.println(OLED_ADDR, HEX);

  // Khoi tao OLED
  if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    oledOK = true;
    Serial.println("[OLED] init OK");
  } else {
    oledOK = false;
    Serial.println("[OLED] init FAIL");
    Serial.println("Check wiring: VCC, GND, SDA, SCL");
    Serial.println("Try OLED address 0x3C or 0x3D");
  }

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("OLED hien thi LOCAL SENSORS");
  Serial.println("OLED hien thi Temp, Hum, RainAO, RainDO, MQ6, Gas");
  Serial.println("----------------------------------");

  // Hien thi du lieu test lan dau
  showOLED();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Tao du lieu gia lap thay doi de kiem tra OLED co cap nhat hay khong
  localTemp += 0.1;
  localHum += 0.2;
  rainAO += 10;
  mq6Raw += 5;

  // Dao trang thai WET/DRY va SAFE/DANGER moi 5 giay
  static unsigned long lastToggle = 0;
  if (millis() - lastToggle >= 5000) {
    lastToggle = millis();

    rainDetected = !rainDetected;
    gasDanger = !gasDanger;
  }

  // Goi ham test OLED
  showOLED();

  // In Serial de doi chieu voi man hinh OLED
  Serial.print("[TC04] Temp = ");
  Serial.print(localTemp, 1);

  Serial.print(" C | Hum = ");
  Serial.print(localHum, 1);

  Serial.print(" % | RainAO = ");
  Serial.print(rainAO);

  Serial.print(" | RainDO = ");
  Serial.print(rainDetected ? "WET" : "DRY");

  Serial.print(" | MQ6 = ");
  Serial.print(mq6Raw);

  Serial.print(" | Gas = ");
  Serial.println(gasDanger ? "DANGER" : "SAFE");

  delay(1000);
}