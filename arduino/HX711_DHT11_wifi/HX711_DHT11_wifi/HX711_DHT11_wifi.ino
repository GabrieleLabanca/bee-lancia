///////////////////
// CONFIGURE HX711
#include "HX711.h"//The library used for arduino  https://github.com/bogde/HX711
// HX711.DOUT   - D1  (Arduino: 8 (BEFORE: pin 10))
// HX711.PD_SCK - D0 (Arduino: 7 (BEFORE: pin 11))
#define HX711_SCK_PIN  D1
#define HX711_DOUT_PIN D2
//scale(DOUT,SCK)
HX711 scale;//(HX711_DOUT_PIN, HX711_SCK_PIN); // parameter "gain" omitted; default value 128

///////////////////
// CONFIGURE DHT11
#include "DHT.h" // https://github.com/adafruit/DHT-sensor-library  NEEDS https://github.com/adafruit/Adafruit_Sensor
#define DHTTYPE DHT11   // DHT 11
#define DHT11_PIN D3 //signal pin (has to be digital)
DHT dht(DHT11_PIN, DHTTYPE);
//DHT dht(DHTPIN, DHT11);

// CONFIGURE WIFI
#include <ESP8266WiFi.h>


// WIFI CREDENTIALS
const char *ssid = "";
const char *pass = "";

// CONFIGURE SERVER
const char* server = "api.thingspeak.com";
String apiKey = "";     //  Enter your Write API key from ThingSpeak
WiFiClient client;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("HX711_DHT11_wifi");
  
  // SETUP HX711
  scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);

  //SETUP WIFI
  Serial.println("Connecting to ");
  Serial.println(ssid);  Serial.println("HX711_DHT11_wifi");
  WiFi.begin(ssid, pass);
  while (WiFi.localIP().toString() == "0.0.0.0") //while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  //Serial.println("HX711_DHT11_wifi READY");

}
void loop()
{
  // GET DATA
  // weight
  scale.power_up();
  float weight_raw = scale.read();
  Serial.print(" Raw weight: ");
  Serial.print(weight_raw, 3);
  scale.power_down(); 
  
  // temperature
  float t = dht.readTemperature();
  // humidity
  float h = dht.readHumidity();
  Serial.print(" Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius, Humidity: ");
  Serial.print(h);
  Serial.print('\n');

  // SEND TO THINGSPEAK
  Serial.println("%. Send to Thingspeak.");
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    //postStr += "&field1=";
    //postStr += String(weight);
    postStr += "&field2=";
    postStr += String(t);
    postStr += "&field3=";
    postStr += String(h);
    postStr += "&field4=";
    postStr += String(weight_raw);
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
  int n_min = 1;
  int delay_time = 1000 * 60 * n_min;
  delay(delay_time);
}








