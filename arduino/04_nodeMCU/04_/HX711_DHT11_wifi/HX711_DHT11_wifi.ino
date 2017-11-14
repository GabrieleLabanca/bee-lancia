///////////////////
// CONFIGURE HX711
#include "HX711.h"//The library used for arduino  https://github.com/bogde/HX711
// HX711.DOUT   - D2  (Arduino: 8 (BEFORE: pin 10))
// HX711.PD_SCK - D1 (Arduino: 7 (BEFORE: pin 11))
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
/*// CELL
const char *ssid =  "gabgab";     // replace with your wifi ssid and wpa2 key
const char *pass =  "12345678";
*/
/*
// TELECOM
const char *ssid =  "Telecom-58120441";     // replace with your wifi ssid and wpa2 key
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";
*/
// FASTWEB
const char *ssid =  "FASTWEB-1-0D2259";     // replace with your wifi ssid and wpa2 key
const char *pass =  "CviK6uXSQLucijkT1XT8BlFP";




void setup()
{
  // SETUP SERIAL
  delay(1000);
  Serial.begin(9600);
  Serial.println("BEGIN CONNECTION");

  // SETUP HX711
  scale.begin(D2, D1);
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
  // weight
  float weight = scale.get_units();//* .005600966442953021;
  Serial.print("Weight: ");
  Serial.print(weight, 3); //tara con 3.870kg

  // temperature
  float t = dht.readTemperature();
  // humidity
  float h = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius, Humidity: ");
  Serial.print(h);
  Serial.print('\n');

  delay(15000);
}
