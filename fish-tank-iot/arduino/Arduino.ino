#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

const int DHT_PIN = 2;
const int DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dltemp(&oneWire);

#define led1 4

double envtemp = 0.0f;
double humi = 0.0f;
double watertemp = 0.0f;
String data;
String key;
String value;
int currentTimeStamp;
int led1TurnOnTimeStamp;
int led1TurnOffTimeStamp;

void setup()
{
  Serial.begin(115200);
  dltemp.begin();
  dht.begin();
  pinMode(led1, OUTPUT);
  delay(500);
}

void loop()
{
  data = receiveESP();
  if (data!="")
  {
    key = data.substring(0,1);
    value = data.substring(1);
    if (key == "1")
    {
      currentTimeStamp = value.toInt();
    }
    else if (key == "2")
    {
      led1TurnOnTimeStamp = value.toInt();
    }
    else if (key == "3")
    {
      led1TurnOffTimeStamp = value.toInt();
    }
  }
  if (currentTimeStamp!=0 && led1TurnOnTimeStamp!=0 && led1TurnOffTimeStamp!=0)
  {
    if (led1TurnOnTimeStamp <= currentTimeStamp && currentTimeStamp <= led1TurnOffTimeStamp)
    {
      digitalWrite(led1, HIGH);
    }
    else
    {
      digitalWrite(led1, LOW);
    }
  }
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
  Serial.print(key + value);
  delay(500);
}

String receiveESP()
{
  String buff = "";
  if (Serial.available())
  {
    while(Serial.available())
    {
      buff += (char)Serial.read();
    }   
  }
  return buff;
}
