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


// CASA RINALDO
  const char *ssid =  "WIFIRINALDO";  
  const char *pass =  "12345678";


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
/*
// Arsenale della Terra
  const char *ssid =  "dlink_DWR-730_2F6E";    
  const char *pass =  "arsenaleterra2017";
*/
// CONFIGURE SERVER
const char* server = "api.thingspeak.com";
String apiKey = "AG5BH0BV8ITOCAUL";     //  Enter your Write API key from ThingSpeak
WiFiClient client;


void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("HX711_DHT11_wifi");


  // SETUP HX711
  scale.begin(HX711_DOUT_PIN,HX711_SCK_PIN);
  
  scale.power_up();
  delay(1000);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t ");
  Serial.println(scale.read());     // print a raw reading from the ADC
  float myscale = 114. / .005600966442953021;
  scale.set_scale(myscale); // this value is obtained by calibrating the scale with known weights;
  scale.tare();   // reset the scale to 0
  Serial.println("After setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());                        // print a raw reading from the ADC
  delay(15);
  // print the average of 20 readings from the ADC
  Serial.print("read average:\t\t ");
  delay(15);
  Serial.println(scale.read_average(20));
  delay(15);
  // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get value: \t\t ");
  Serial.println(scale.get_value(5));
  delay(15);
  Serial.print("get units: ");
  Serial.println(scale.get_units(5), 1);
  delay(15);
  // print the average of 5 readings from the ADC minus tare weight,
  //divided by the SCALE parameter set with set_scale

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
  // GET DATA  M
  // weight
  float weight = scale.get_units();//* .005600966442953021;
  Serial.print("Weight: ");
  Serial.print(weight, 3); //tara con 3.870kg
  float weight_raw = scale.read();
  Serial.print(" Raw weight: ");
  Serial.print(weight_raw, 3);
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
    postStr += "&field1=";
    postStr += String(weight);
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
  int n_min = 10;
  int delay_time = 1000*60*n_min;
  delay(delay_time);
}








