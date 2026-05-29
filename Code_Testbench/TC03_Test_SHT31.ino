// =====================================================
// TC03 - TEST SHT31 TEMPERATURE & HUMIDITY SENSOR
// Target:
// SHT31 SDA -> GPIO21 ESP32
// SHT31 SCL -> GPIO22 ESP32
// =====================================================

#include <Wire.h>
#include <Adafruit_SHT31.h>

// ================= I2C PIN =================
#define I2C_SDA 21
#define I2C_SCL 22

// ================= SHT31 =================
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Bien kiem tra cam bien co khoi tao duoc khong
bool shtOK = false;

// Bien luu gia tri nhiet do, do am
float localTemp = NAN;
float localHum  = NAN;

// =====================================================
// HAM DOC SHT31
// Dua theo ham trong project goc
// =====================================================
void readSHT31() {
  if (!shtOK) {
    localTemp = NAN;
    localHum = NAN;
    return;
  }

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t)) localTemp = t;
  if (!isnan(h)) localHum = h;
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TC03 - TEST SHT31 SENSOR");
  Serial.println("==================================");

  // Khoi tao I2C cho ESP32
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.print("SHT31 SDA Pin: GPIO");
  Serial.println(I2C_SDA);

  Serial.print("SHT31 SCL Pin: GPIO");
  Serial.println(I2C_SCL);

  // Thu khoi tao SHT31 voi dia chi 0x44 truoc
  if (sht31.begin(0x44)) {
    shtOK = true;
    Serial.println("[SHT31] OK @0x44");
  } 
  // Neu khong duoc thi thu dia chi 0x45
  else if (sht31.begin(0x45)) {
    shtOK = true;
    Serial.println("[SHT31] OK @0x45");
  } 
  // Neu ca 2 dia chi deu fail
  else {
    shtOK = false;
    Serial.println("[SHT31] FAIL");
    Serial.println("Check wiring: VCC, GND, SDA, SCL");
  }

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("shtOK = true");
  Serial.println("localTemp is a number, not NaN");
  Serial.println("localHum  is a number, not NaN");
  Serial.println("----------------------------------");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Goi ham test SHT31
  readSHT31();

  Serial.print("[TC03] SHT31 Status = ");
  Serial.print(shtOK ? "OK" : "FAIL");

  Serial.print(" | Temp = ");
  if (isnan(localTemp)) {
    Serial.print("NaN");
  } else {
    Serial.print(localTemp);
    Serial.print(" C");
  }

  Serial.print(" | Hum = ");
  if (isnan(localHum)) {
    Serial.println("NaN");
  } else {
    Serial.print(localHum);
    Serial.println(" %");
  }

  delay(1000);
}
