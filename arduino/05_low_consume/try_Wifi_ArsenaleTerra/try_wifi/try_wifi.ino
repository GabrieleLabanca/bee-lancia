// CONFIGURE WIFI
#include <ESP8266WiFi.h>
/*// CELL
  const char *ssid =  "gabgab";  
  const char *pass =  "12345678";
 */
/*
// TELECOM
const char *ssid =  "Telecom-58120441";     
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
 */
/*
// FASTWEB
const char *ssid =  "FASTWEB-1-0D2259";     /
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
 */
// Arsenale della Terra
const char *ssid =  "dlink_DWR-730_2F6E";    
const char *pass =  "arsenaleterra2017";

// CONFIGURE SERVER
const char* server = "api.thingspeak.com";
String apiKey = "AG5BH0BV8ITOCAUL";     //  Enter your Write API key from ThingSpeak
WiFiClient client;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("HX711_DHT11_wifi");

  //SETUP WIFI
  Serial.println("Connecting to ");
  Serial.println(ssid);  Serial.println("HX711_DHT11_wifi");
  WiFi.begin(ssid, pass);
  while (WiFi.localIP().toString() == "0.0.0.0") 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

}

void loop()
{
  // SEND TO THINGSPEAK
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field5=";
    postStr += String(0);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();

  // DELAY
  Serial.print('\n');
  delay(60000);
}








