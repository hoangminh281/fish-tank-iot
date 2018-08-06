#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPtimeESP.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

#define FIREBASE_HOST "qlcn-3d083.firebaseio.com"
#define FIREBASE_AUTH "DqRKMGNqF5HjBQSVqDBN7FDFSA4OrcPBaGQL82RT"
#define WIFI_SSID "TMA20years"
#define WIFI_PASSWORD "TMA20years"

#define KEY_HUMI "humidity"
#define KEY_ENVTEMP "env_temperature"
#define KEY_WATERTEMP "water_temperature"
#define KEY_LED1TURNONTIMESTAMP "led1TurnOnTimeStamp"
#define KEY_LED1TURNOFFTIMESTAMP "led1TurnOffTimeStamp"

NTPtime NTPch("ch.pool.ntp.org");
strDateTime dateTime;

const int DHT_PIN = 4;
const int DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dltemp(&oneWire);

double envtemp = 0.0f;
double humi = 0.0f;
double watertemp = 0.0f;

#define led1 4

int currentTimeStamp;
int led1TurnOnTimeStamp;
int led1TurnOffTimeStamp;

void setup() {
  Serial.begin(115200);
  dltemp.begin();
  dht.begin();
  pinMode(led1, OUTPUT);
  u8g2.begin();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  delay(500);
}

void loop() {
  envtemp = dht.readTemperature();
  humi = dht.readHumidity();
  dltemp.requestTemperatures();
  watertemp = dltemp.getTempCByIndex(0);
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawUTF8(0, 20, "Humi: ");
  u8g2.sendBuffer();

  dateTime = NTPch.getNTPtime(7.0, 0);
  led1TurnOnTimeStamp = receiveIntFirebase(KEY_LED1TURNONTIMESTAMP);
  led1TurnOffTimeStamp = receiveIntFirebase(KEY_LED1TURNOFFTIMESTAMP);

  if (dateTime.valid)
  {
    if (currentTimeStamp!=0 && led1TurnOnTimeStamp!=0 && led1TurnOffTimeStamp!=0)
    {
      currentTimeStamp = dateTime.hour * 10000 + dateTime.minute * 100 + dateTime.second;
      if (led1TurnOnTimeStamp <= currentTimeStamp && currentTimeStamp <= led1TurnOffTimeStamp)
      {
        digitalWrite(led1, HIGH);
      }
      else
      {
        digitalWrite(led1, LOW);
      }
    }
  }
  delay(1000);
}

void setFloatFirebase(String key, float value)
{
  Firebase.setFloat(key, value);
}

float receiveFloatFirebase(String key)
{
  return Firebase.getFloat(key);
}

int receiveIntFirebase(String key)
{
  return Firebase.getInt(key);
}