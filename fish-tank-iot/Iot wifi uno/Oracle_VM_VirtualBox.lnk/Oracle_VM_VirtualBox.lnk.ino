#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "qlcn-3d083.firebaseio.com"
#define FIREBASE_AUTH "DqRKMGNqF5HjBQSVqDBN7FDFSA4OrcPBaGQL82RT"
#define WIFI_SSID "TMA20years"
#define WIFI_PASSWORD "TMA20years"

#define KEY_HUMI "humidity"
#define KEY_ENVTEMP "env_temperature"
#define KEY_WATERTEMP "water_temperature"

const int DHT_PIN = 4;
const int DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dltemp(&oneWire);

double envtemp = 0.0f;
double humi = 0.0f;
double watertemp = 0.0f;

void setup() {
  Serial.begin(115200);
  dltemp.begin();
  dht.begin();
  delay(500);
}

void loop() {
  envtemp = dht.readTemperature();
  humi = dht.readHumidity();
  dltemp.requestTemperatures();
  watertemp = dltemp.getTempCByIndex(0);
  Serial.println((String)envtemp);
  Serial.println((String)humi);
  Serial.println((String)watertemp);
}
