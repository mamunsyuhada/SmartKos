#include <ArduinoJson.h>  
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SimpleDHT.h>

#define FIREBASE_HOST "imam-syuhada.firebaseio.com/"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "ofghHbZO3m9RgVJlxP8uv8022Crs0JNwV1W7RHFB"       //Your Firebase Database Secret goes here
#define WIFI_SSID "MERANGSANG"                                               //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "juhpattu"                                      //Password of your wifi network 

#define Relay1 5 //D1
String val1;

#define Relay2 4 //D2 
String val2;

#define Relay3 0 //D3
String val3;

#define Relay4 2 //D4
String val4;

const int sensorPintu = 14; //D5
const int sensorJendela = 12; //D6
const int alarm = 13; //D7

int pinDHT22 = 10; //SD3
SimpleDHT22 dht22;
float temperature,humidity;
String tempStr, rhStr;

void setup()
{
  Serial.begin(115200);                                                   // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
    pinMode(Relay4, OUTPUT);  
  pinMode(alarm, OUTPUT);
  pinMode(sensorPintu, INPUT);
  pinMode(sensorJendela, INPUT);

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }
  /*Semua Lampu*/
  if (Firebase.getString("semualampu") == "on") {
    Firebase.setString("lampu1", "high");
    Firebase.setString("lampu2", "high");
    Firebase.setString("kipass", "high");
    Firebase.setString("lockkk", "high");
    Firebase.setString("semualampu", "netral");
  }
  else if (Firebase.getString("semualampu") == "off") {
    Firebase.setString("lampu1", "low");
    Firebase.setString("lampu2", "low");
    Firebase.setString("kipass", "low");
    Firebase.setString("lockkk", "low");
    Firebase.setString("semualampu", "netral");
  }
  /*Keamanan*/
  if (Firebase.getString("keamanan") == "on") {
    keamananOn();
  }
  /*Lampu--1*/
  val1 = Firebase.getString("lampu1");                                      //Reading the value of the varialble Status from the firebase
  if (val1 == "high")                                                          // If, the Status is 1, turn on the Relay1
  {
    digitalWrite(Relay1, LOW);
    Serial.println("light 1 ON");
  }
  else if (val1 == "low")                                                   // If, the Status is 0, turn Off the Relay1
  {
    digitalWrite(Relay1, HIGH);
    Serial.println("light 1 OFF");
  }
  /*Lampu--2*/
  val2 = Firebase.getString("lampu2");                                      //Reading the value of the varialble Status from the firebase

  if (val2 == "high")                                                          // If, the Status is 1, turn on the Relay2
  {
    digitalWrite(Relay2, LOW);
    Serial.println("light 2 ON");
  }
  else if (val2 == "low")                                                   // If, the Status is 0, turn Off the Relay2
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("light 2 OFF");
  }
  /*Lampu--3*/
  val3 = Firebase.getString("kipass");                                      //Reading the value of the varialble Status from the firebase

  if (val3 == "high")                                                          // If, the Status is 1, turn on the Relay3
  {
    digitalWrite(Relay3, LOW);
    Serial.println("light 3 ON");
  }
  else if (val3 == "low")                                                   // If, the Status is 0, turn Off the Relay3
  {
    digitalWrite(Relay3, HIGH);
    Serial.println("light 3 OFF");
  }
  /*Lampu--4*/
  val4 = Firebase.getString("lockkk");                                      //Reading the value of the varialble Status from the firebase

  if (val4 == "high")                                                          // If, the Status is 1, turn on the Relay4
  {
    digitalWrite(Relay4, LOW);
    Serial.println("light 4 ON");
  }
  else if (val4 == "low")                                                   // If, the Status is 0, turn Off the Relay4
  {
    digitalWrite(Relay4, HIGH);
    Serial.println("light 4 OFF");
  }

  //Suhu dan RH
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(pinDHT22, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err); delay(2000);
    return;
  }

  tempStr = (float)temperature;
  rhStr   = (float)humidity;
  Serial.print(tempStr); Serial.print(" *C, ");
  Serial.print(rhStr); Serial.println(" RH%");
  Firebase.setString("suhu", tempStr);
  Firebase.setString("rh", rhStr);
}
