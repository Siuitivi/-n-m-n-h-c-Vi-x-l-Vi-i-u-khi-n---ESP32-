// =====================================================
// TC01 - TEST MQ6 GAS SENSOR
// Target: MQ6 AO pin -> GPIO35 ESP32
// =====================================================

// ================= MQ6 =================
#define MQ6_AO_PIN 35

// Nguong canh bao gas
// Theo code project goc, nguong dang dung la 1000
int GAS_THRESHOLD = 1000;

// Bien luu gia tri doc tu MQ6
int mq6Raw = 0;

// Bien trang thai gas
// false = SAFE
// true  = DANGER
bool gasDanger = false;

// =====================================================
// HAM DOC MQ6
// Day la ham lay tu project goc
// =====================================================
void readMQ6() {
  mq6Raw = analogRead(MQ6_AO_PIN);
  gasDanger = (mq6Raw >= GAS_THRESHOLD);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TC01 - TEST MQ6 GAS SENSOR");
  Serial.println("==================================");

  // Cau hinh ADC ESP32
  analogReadResolution(12);                    // ADC 12-bit, gia tri 0 -> 4095
  analogSetPinAttenuation(MQ6_AO_PIN, ADC_11db); // Cho phep doc dai dien ap rong hon

  Serial.print("MQ6 AO Pin: GPIO");
  Serial.println(MQ6_AO_PIN);

  Serial.print("Gas Threshold: ");
  Serial.println(GAS_THRESHOLD);

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("mq6Raw < threshold  => SAFE");
  Serial.println("mq6Raw >= threshold => DANGER");
  Serial.println("----------------------------------");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Goi ham test MQ6
  readMQ6();

  // In ket qua testcase
  Serial.print("[TC01] MQ6 Raw = ");
  Serial.print(mq6Raw);

  Serial.print(" | Threshold = ");
  Serial.print(GAS_THRESHOLD);

  Serial.print(" | Gas Status = ");
  if (gasDanger) {
    Serial.println("DANGER");
  } else {
    Serial.println("SAFE");
  }

  delay(1000);
}