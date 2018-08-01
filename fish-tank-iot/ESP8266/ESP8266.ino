#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "qlcn-3d083.firebaseio.com"
#define FIREBASE_AUTH "DqRKMGNqF5HjBQSVqDBN7FDFSA4OrcPBaGQL82RT"
#define WIFI_SSID "TMA20years"
#define WIFI_PASSWORD "TMA20years"

#define KEY_HUMI "humidity"
#define KEY_ENVTEMP "env_temperature"
#define KEY_WATERTEMP "water_temperature"

int isOnLed1;
String data;
String key;
String value;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(500);
}

void loop()
{
  data = receiveUNO();
  if (data!="")
  {
    key = data.substring(0,1);
    value = data.substring(1);
    if (key == "1")
    {
      setFirebase(KEY_HUMI, value.toFloat());
    }
    else if (key == "2")
    {
      setFirebase(KEY_ENVTEMP, value.toFloat());
    }
    else if (key == "3")
    {
      setFirebase(KEY_WATERTEMP, value.toFloat());
    }
  }
}

void setFirebase(String key, float value)
{
  Firebase.setFloat(key, value);
}

int receiveFirebase(String key)
{
  return Firebase.getFloat(key);
}

void sendUNO(String data)
{
  Serial.println(data);
}

String receiveUNO()
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

