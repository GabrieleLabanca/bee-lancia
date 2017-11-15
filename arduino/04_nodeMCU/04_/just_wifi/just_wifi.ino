// CONFIGURE WIFI
#include "ESP8266WiFi.h"
// Toolbox
const char *ssid =  "toolbox";
const char *pass =  "Toolbox.Torino";

// CONFIGURE SERVER
const char* server = "api.thingspeak.com";
String apiKey = "AG5BH0BV8ITOCAUL";     //  Enter your Write API key from ThingSpeak
WiFiClient client;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("BEGIN CONNECTION");

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
  // weight
  float weight = scale.get_units();//* .005600966442953021;
  // temperature
  float t = dht.readTemperature();
  // humidity
  float h = dht.readHumidity();
  if (isnan(weight)) {
    Serial.println("ERROR READING from HX711!");
    blink(10);
    return;
  }

  blink(1);
  Serial.print("Weight: ");
  Serial.print(weight, 3); //tara con 3.870kg
  Serial.print(' ');
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius, Humidity: ");
  Serial.print(h);
  Serial.print('\n');
*/

  /*
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

