// =====================================================
// TC05 - TEST LCD 16x2 I2C
// Requirement:
// LCD 16x2 I2C phai hien thi duoc du lieu thoi tiet/API
//
// Target:
// LCD SDA -> GPIO21 ESP32
// LCD SCL -> GPIO22 ESP32
// LCD I2C Address: 0x27
// =====================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= I2C PIN =================
#define I2C_SDA 21
#define I2C_SCL 22

// ================= LCD =================
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// ================= DU LIEU API GIA LAP =================
// Cac bien nay giong logic trong project goc
float weatherTemp = 31.2;
float weatherHum  = 65.0;
int rainProb      = 40;

bool wifiOK = true;
bool weatherOK = true;

// =====================================================
// HAM HIEN THI LCD
// Dua theo logic hien thi trong project goc
// =====================================================
void showLCD() {
  lcd.clear();

  lcd.setCursor(0, 0);

  if (wifiOK && weatherOK) {
    // Dong 1: Nhiet do va do am tu API
    lcd.print("T:");
    lcd.print(weatherTemp, 1);
    lcd.print("C ");

    lcd.print("H:");
    lcd.print((int)weatherHum);
    lcd.print("%");

    // Dong 2: Xac suat mua va trang thai API
    lcd.setCursor(0, 1);
    lcd.print("Rain:");
    lcd.print(rainProb);
    lcd.print("% API OK");
  } 
  else if (!wifiOK) {
    lcd.print("WiFi ERROR");
    lcd.setCursor(0, 1);
    lcd.print("No connection");
  } 
  else if (!weatherOK) {
    lcd.print("API ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Fetch failed");
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TC05 - TEST LCD 16x2 I2C");
  Serial.println("==================================");

  // Khoi tao I2C cho ESP32
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.print("LCD SDA Pin: GPIO");
  Serial.println(I2C_SDA);

  Serial.print("LCD SCL Pin: GPIO");
  Serial.println(I2C_SCL);

  Serial.print("LCD Address: 0x");
  Serial.println(LCD_ADDR, HEX);

  // Khoi tao LCD
  lcd.init();
  lcd.backlight();

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("LCD dong 1: T:31.2C H:65%");
  Serial.println("LCD dong 2: Rain:40% API OK");
  Serial.println("----------------------------------");

  showLCD();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Gia lap du lieu API thay doi de kiem tra LCD co cap nhat khong
  weatherTemp += 0.1;
  weatherHum += 1;
  rainProb += 5;

  if (weatherHum > 90) {
    weatherHum = 60;
  }

  if (rainProb > 100) {
    rainProb = 0;
  }

  // Goi ham test LCD
  showLCD();

  // In Serial de doi chieu voi LCD
  Serial.print("[TC05] Weather Temp = ");
  Serial.print(weatherTemp, 1);

  Serial.print(" C | Weather Hum = ");
  Serial.print(weatherHum, 0);

  Serial.print(" % | Rain Prob = ");
  Serial.print(rainProb);

  Serial.print(" % | LCD Status = ");

  if (wifiOK && weatherOK) {
    Serial.println("API OK");
  } else if (!wifiOK) {
    Serial.println("WiFi ERROR");
  } else {
    Serial.println("API ERROR");
  }

  delay(2000);
}
