#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

const int DHT_PIN = 4;            //DHT11 is connected to pin D2
const int DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dltemp(&oneWire);

SoftwareSerial mySerial(2, 3); //Rx = 2, Tx = 3

double envtemp = 0.0f;
double humi = 0.0f;
double watertemp = 0.0f;

void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
  dltemp.begin();
  dht.begin();
  delay(500);
}

void loop()
{
  envtemp = dht.readTemperature();
  humi = dht.readHumidity();
  dltemp.requestTemperatures();
  watertemp = dltemp.getTempCByIndex(0);
  sendESP("1", (String)humi);
  sendESP("2", (String)envtemp);
  sendESP("3", (String)watertemp);
}

void sendESP(String key, String value)
{
  mySerial.print(key + value);
  delay(500);
}

String receiveESP()
{
  String buff = "";
  if (mySerial.available()) {
    while (mySerial.available()) {
      char data = (char)mySerial.read();
      buff += data;
    }
  }
  return buff;
}

