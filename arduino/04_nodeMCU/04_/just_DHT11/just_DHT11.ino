///////////////////
// CONFIGURE DHT11
#include "DHT.h" // https://github.com/adafruit/DHT-sensor-library  NEEDS https://github.com/adafruit/Adafruit_Sensor
#define DHTTYPE DHT11   // DHT 11
#define DHT11_PIN D2 //signal pin (has to be digital)
DHT dht(DHT11_PIN, DHTTYPE);
//DHT dht(DHTPIN, DHT11);

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("just_DHT11");
}

void loop()
{
  // temperature
  float t = dht.readTemperature();
  // humidity
  float h = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius, Humidity: ");
  Serial.print(h);
  Serial.print('\n');

  delay(1500);
}
