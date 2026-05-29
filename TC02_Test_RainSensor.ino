// =====================================================
// TC02 - TEST RAIN SENSOR
// Target:
// Rain Sensor AO -> GPIO34 ESP32
// Rain Sensor DO -> GPIO27 ESP32
// =====================================================

// ================= RAIN SENSOR =================
#define RAIN_AO_PIN 34
#define RAIN_DO_PIN 27

// Theo code project goc:
// Neu cam bien DO active LOW:
// LOW  = co mua / WET
// HIGH = kho / DRY
const bool RAIN_DO_ACTIVE_LOW = true;

// Bien luu gia tri analog cua cam bien mua
int rainAO = 0;

// Bien trang thai mua
// false = DRY
// true  = WET
bool rainDetected = false;

// =====================================================
// HAM DOC CAM BIEN MUA
// Ham nay dua theo logic trong project goc
// =====================================================
void readRainSensor() {
  // Doc gia tri analog
  rainAO = analogRead(RAIN_AO_PIN);

  // Doc gia tri digital
  int rainDO = digitalRead(RAIN_DO_PIN);

  // Xac dinh trang thai kho/uot
  if (RAIN_DO_ACTIVE_LOW) {
    rainDetected = (rainDO == LOW);
  } else {
    rainDetected = (rainDO == HIGH);
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TC02 - TEST RAIN SENSOR");
  Serial.println("==================================");

  // Cau hinh chan cam bien mua
  pinMode(RAIN_DO_PIN, INPUT);

  // Cau hinh ADC ESP32
  analogReadResolution(12);                       // ADC 12-bit: 0 -> 4095
  analogSetPinAttenuation(RAIN_AO_PIN, ADC_11db); // Doc dai dien ap rong hon

  Serial.print("Rain AO Pin: GPIO");
  Serial.println(RAIN_AO_PIN);

  Serial.print("Rain DO Pin: GPIO");
  Serial.println(RAIN_DO_PIN);

  Serial.print("RAIN_DO_ACTIVE_LOW = ");
  Serial.println(RAIN_DO_ACTIVE_LOW ? "true" : "false");

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("Dry condition  => rainDetected = false, Status = DRY");
  Serial.println("Wet condition  => rainDetected = true,  Status = WET");
  Serial.println("Analog value should change when water is added.");
  Serial.println("----------------------------------");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Goi ham test cam bien mua
  readRainSensor();

  // Doc truc tiep gia tri DO de quan sat them
  int rainDO = digitalRead(RAIN_DO_PIN);

  // In ket qua testcase
  Serial.print("[TC02] Rain AO = ");
  Serial.print(rainAO);

  Serial.print(" | Rain DO = ");
  Serial.print(rainDO == HIGH ? "HIGH" : "LOW");

  Serial.print(" | Rain Status = ");
  if (rainDetected) {
    Serial.println("WET");
  } else {
    Serial.println("DRY");
  }

  delay(1000);
}