// =====================================================
// TESTCASE - ADAFRUIT IO MQTT
// Requirement:
// Adafruit IO phai nhan duoc du lieu mau tu ESP32 qua MQTT
//
// Target:
// ESP32 publish du lieu mau len cac feed Adafruit IO
// =====================================================

#include <WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

// ================= ADAFRUIT IO =================
// Copy AIO_USER va AIO_KEY tu file project goc
// Khong nen public AIO_KEY len GitHub / bao cao
#define AIO_USER "YOUR_AIO_USERNAME"
#define AIO_KEY  "YOUR_AIO_KEY"

const char* MQTT_HOST = "io.adafruit.com";
const uint16_t MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// ================= MQTT TOPICS =================
// Cac topic nay dua theo file .ino goc
String tGasState;
String tRainState;
String tLocalTemp;
String tLocalHum;
String tRainProb;
String tWeatherTemp;
String tWeatherHum;

// ================= DU LIEU MAU =================
// Gia tri gia lap de gui len Adafruit IO
float localTemp   = 30.5;
float localHum    = 70.0;

bool rainDetected = false;   // false = DRY, true = WET
bool gasDanger    = false;   // false = SAFE, true = DANGER

int rainProb      = 25;
float weatherTemp = 31.0;
float weatherHum  = 65.0;

// =====================================================
// BUILD ADAFRUIT IO TOPICS
// Dua theo project goc
// =====================================================
void buildAioTopics() {
  String base = String(AIO_USER) + "/feeds/";

  tGasState    = base + "do-an.gas-state";
  tRainState   = base + "do-an.rain-state";
  tLocalTemp   = base + "do-an.local-temp";
  tLocalHum    = base + "do-an.local-hum";
  tRainProb    = base + "do-an.rain-prob";
  tWeatherTemp = base + "do-an.weather-temp";
  tWeatherHum  = base + "do-an.weather-hum";
}

// =====================================================
// CONNECT MQTT
// Dua theo project goc
// =====================================================
bool mqttConnect() {
  if (mqtt.connected()) {
    return true;
  }

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setKeepAlive(60);

  Serial.print("[MQTT] Connecting");

  String clientId = "esp32-test-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  if (mqtt.connect(clientId.c_str(), AIO_USER, AIO_KEY)) {
    Serial.println(" OK");
    return true;
  }

  Serial.print(" FAIL rc=");
  Serial.println(mqtt.state());
  return false;
}

// =====================================================
// PUBLISH FUNCTIONS
// =====================================================
void publishFloat(const String& topic, float value, int digits = 1) {
  if (!mqtt.connected()) return;

  char buf[20];
  dtostrf(value, 0, digits, buf);

  bool ok = mqtt.publish(topic.c_str(), buf, true);

  Serial.print("[PUBLISH] ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.print(buf);
  Serial.println(ok ? " OK" : " FAIL");
}

void publishInt(const String& topic, int value) {
  if (!mqtt.connected()) return;

  char buf[16];
  snprintf(buf, sizeof(buf), "%d", value);

  bool ok = mqtt.publish(topic.c_str(), buf, true);

  Serial.print("[PUBLISH] ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.print(buf);
  Serial.println(ok ? " OK" : " FAIL");
}

void publishText(const String& topic, const char* value) {
  if (!mqtt.connected()) return;

  bool ok = mqtt.publish(topic.c_str(), value, true);

  Serial.print("[PUBLISH] ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.print(value);
  Serial.println(ok ? " OK" : " FAIL");
}

// =====================================================
// PUBLISH TO ADAFRUIT IO
// Ham test chinh cua testcase
// =====================================================
void publishToAdafruitIO() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[MQTT] WiFi not connected, skip publish");
    return;
  }

  if (!mqtt.connected()) {
    if (!mqttConnect()) {
      Serial.println("[MQTT] connect failed, skip publish");
      return;
    }
  }

  mqtt.loop();

  publishText(tGasState, gasDanger ? "DANGER" : "SAFE");
  publishText(tRainState, rainDetected ? "WET" : "DRY");

  publishFloat(tLocalTemp, localTemp, 1);
  publishFloat(tLocalHum, localHum, 1);

  publishInt(tRainProb, rainProb);

  publishFloat(tWeatherTemp, weatherTemp, 1);
  publishFloat(tWeatherHum, weatherHum, 0);

  Serial.println("[MQTT] Published sample data to Adafruit IO");
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================");
  Serial.println("TESTCASE - ADAFRUIT IO MQTT");
  Serial.println("==================================");

  // Dung WiFiManager giong project goc
  // Neu ESP32 chua co WiFi, no se tao AP: ESP32-WIFI-SETUP
  WiFiManager wm;
  wm.autoConnect("ESP32-WIFI-SETUP");

  Serial.println("[WiFi] Connected");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());

  buildAioTopics();

  Serial.println("----------------------------------");
  Serial.println("Expected:");
  Serial.println("[MQTT] Connecting OK");
  Serial.println("Adafruit IO dashboard nhan du lieu mau");
  Serial.println("----------------------------------");

  // Publish lan dau ngay sau khi ket noi
  publishToAdafruitIO();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  if (mqtt.connected()) {
    mqtt.loop();
  }

  // Moi 20 giay gui du lieu mau 1 lan
  static unsigned long lastPublish = 0;

  if (millis() - lastPublish >= 20000) {
    lastPublish = millis();

    // Gia lap du lieu thay doi de de quan sat tren dashboard
    localTemp += 0.1;
    localHum += 0.5;
    rainProb += 5;

    if (localTemp > 35.0) localTemp = 30.0;
    if (localHum > 90.0) localHum = 60.0;
    if (rainProb > 100) rainProb = 0;

    // Dao trang thai SAFE/DANGER va DRY/WET moi lan publish
    gasDanger = !gasDanger;
    rainDetected = !rainDetected;

    publishToAdafruitIO();
  }
}
