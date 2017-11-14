// CONFIGURE WIFI
#include "ESP8266WiFi.h"
const char *ssid =  "Telecom-58120441";     
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
//WiFiClient client;
//
void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("BEGIN CONNECTION");

  //SETUP WIFI
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop()
{
  //Serial.println("Enter loop");
}

