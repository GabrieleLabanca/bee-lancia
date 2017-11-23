// CONFIGURE WIFI
#include <ESP8266WiFi.h>
/*// CELL
  const char *ssid =  "gabgab";     // replace with your wifi ssid and wpa2 key
  const char *pass =  "12345678";
*/
// TELECOM
const char *ssid =  "Telecom-58120441";     // replace with your wifi ssid and wpa2 key
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
/*
  // FASTWEB
  const char *ssid =  "FASTWEB-1-0D2259";     // replace with your wifi ssid and wpa2 key
  const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
*/
/*// Toolbox
  const char *ssid =  "toolbox";
  const char *pass =  "Toolbox.Torino";
*/


// CONFIGURE SERVER
const char* server = "api.thingspeak.com";
String apiKey = "AG5BH0BV8ITOCAUL";     //  Enter your Write API key from ThingSpeak
WiFiClient client;


void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("JUST_WIFI");

  //SETUP WIFI
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.localIP().toString() == "0.0.0.0") //while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop()
{
  
  float weight = 10;
  float t = 11;
  float h = 12;
  Serial.println("%. Send to Thingspeak.");
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(weight);
    postStr += "&field2=";
    postStr += String(t);
    postStr += "&field3=";
    postStr += String(h);
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
  delay(15000);
  
}

