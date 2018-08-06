#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPtimeESP.h>

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

String data;
String key;
String value;
int isOnLed1;
int currentTimeStamp;
int led1TurnOnTimeStamp;
int led1TurnOffTimeStamp;

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
  dateTime = NTPch.getNTPtime(7.0, 0);
  led1TurnOnTimeStamp = receiveIntFirebase(KEY_LED1TURNONTIMESTAMP);
  led1TurnOffTimeStamp = receiveIntFirebase(KEY_LED1TURNOFFTIMESTAMP);
  if (dateTime.valid && led1TurnOnTimeStamp!=0 && led1TurnOffTimeStamp!=0) {
    currentTimeStamp = dateTime.hour * 10000 + dateTime.minute * 100 + dateTime.second;
    sendUNO("1", (String)currentTimeStamp);
    sendUNO("2", (String)led1TurnOnTimeStamp);
    sendUNO("3", (String)led1TurnOffTimeStamp);
  }
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

float receiveFloatFirebase(String key)
{
  return Firebase.getFloat(key);
}

int receiveIntFirebase(String key)
{
  return Firebase.getInt(key);
}

void sendUNO(String key, String data)
{
  Serial.print(key + data);
  delay(500);
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

