// by Gabriele Labanca
// credits: Robo India Tutorial

///////////////////
// CONFIGURE DHT11
#include "DHT.h" // https://github.com/adafruit/DHT-sensor-library  NEEDS https://github.com/adafruit/Adafruit_Sensor
#define DHTTYPE DHT11   // DHT 11
#define DHT11_PIN D3 //signal pin (has to be digital)
DHT dht(DHT11_PIN, DHTTYPE);
//DHT dht(DHTPIN, DHT11);

///////////////////
// CONFIGURE HX711
#include "HX711.h"//The library used for arduino  https://github.com/bogde/HX711
// HX711.DOUT   - D2  (Arduino: 8 (BEFORE: pin 10))
// HX711.PD_SCK - D1 (Arduino: 7 (BEFORE: pin 11))
#define HX711_SCK_PIN  D1
#define HX711_DOUT_PIN D2
//scale(DOUT,SCK)
HX711 scale(HX711_DOUT_PIN,HX711_SCK_PIN); // parameter "gain" omitted; default value 128

//////////////////
// CONFIGURE WIFI
#include <ESP8266WiFi.h>
const char *ssid =  "Telecom-58120441";     // replace with your wifi ssid and wpa2 key
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
const char* server = "api.thingspeak.com";
String apiKey = "AG5BH0BV8ITOCAUL";     //  Enter your Write API key from ThingSpeak
WiFiClient client;

///////////////
// SMOOTHING VARIABLES
float m_w[100];
float m_t[100];
int s = 0;
int f = 0;
int counter = 0;
int m_counter = 0;
float sigma = 0.01; // from fit: error
float lin_mult  = 1.17; // from fit: linear multiplicative constant

void setup() 
{
  Serial.begin(38400);
  delay(10);
  Serial.println("BEGIN CONNECTION");

  delay(10);
  //dht.begin();

  //SETUP WIFI
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  for(int i=0; i<100; ++i){
    m_w[i] = 0;
    m_t[i] = 0;
  }
}

void loop() 
{
  // weight
  float weight = scale.get_units();//* .005600966442953021;
  // temperature
  float t = dht.readTemperature();
  // humidity
  float h = dht.readHumidity();

  Serial.print("Weight: ");
  Serial.print(weight, 3); //tara con 3.870kg 
  Serial.print(' ');
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius, Humidity: ");
  Serial.print(h);
  Serial.println("%. Send to Thingspeak.");

  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {  
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(weight);
    postStr +="&field2=";
    postStr += String(t);
    postStr +="&field3=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();

  // MEAN 
  if(m_counter++ < 10){ // exits until 10 values in mean arrays
    m_w[f] += weight;
    m_t[f] += t;
    continue;  
  }
  m_counter = 0;
  m_w[f] /= 10;
  m_t[f] /= 10;

  // SMOOTHING
  if(counter < 10){ // at least 10 m_* points
    ++counter;
    ++f; 
    continue;
  }

  if( (m_w[f]-m_w[f-1])*(m_w[f]-m_w[f-1]) > sigma*sigma ){ // if data are too different, mark it
    ++counter;
  }


  // persistence usually means a new tare of weight
  if(counter > 15){ // 15 = 10(minimum) + 5(more than sigma)
    f -= 5; // if new tare, f-5 is the last point in the old tare
    for(int j=s+1; j<f; ++j){
      m_w[j] -= m_w[s] + lin_mult*m_t[j]; // TODO CHECK IF IT IS RIGHT
      //[....... TODO SEND DATA
    }

    // re-set parameters for new cycle
    s = f; 
    counter = 0;
  }

  Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates
  delay(60000);
}

