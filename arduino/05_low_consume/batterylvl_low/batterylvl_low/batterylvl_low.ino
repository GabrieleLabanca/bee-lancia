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
String apiKey = "HVFDP8XKJSFERVTV";     //  Enter your Write API key from ThingSpeak
WiFiClient client;

// LOW POWER
#define SECONDS_DS(seconds)  ((seconds)*1000000UL)
#define SERIAL_DEBUG (1)

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("batterylvl_low");
  delay(1000);

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

  // ALERT!! to deep sleep
  //Serial.println("\n\nCONNECT D0 WITH RST pin!!!!\n(ten seconds left!)\n");
  //delay(10000);

}
void loop()
{
  // GET DATA  
  float blvl = analogRead(0);

  // SEND TO THINGSPEAK
  Serial.println("%. Send to Thingspeak.");
  if (isnan(blvl))
  {
    Serial.println("Failed to read battery level!");
    return;
  }
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field5=";
    postStr += String(blvl);
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
  //delay(1000);
  delay(1000);
  // TO USE SLEEP: RST connected to D0
  ESP.deepSleep(SECONDS_DS(60*60*2), WAKE_RF_DEFAULT);  
  // Execution resumes at the top of this function.
  delay(100);
}









