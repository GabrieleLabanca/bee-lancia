/*
   Deep sleep test

   Pins RST and GPIO #16 shorted

*/

#define SECONDS_DS(seconds)  ((seconds)*1000000UL)
#define SERIAL_DEBUG (1)

void setup() {
#if SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println(F("\nGood morning, ESP"));
#endif
  // Give user a chance to get control before going into deep sleep
  //delay(5000);
#if SERIAL_DEBUG
  Serial.println(F("Good night"));
#endif
  //ESP.deepSleep(SECONDS_DS(10), WAKE_RF_DEFAULT);
  // Execution resumes at the top of this function.
  delay(100);
}

void loop() {
#if SERIAL_DEBUG
  Serial.println(F("Loop"));
#endif
  delay(5000);
  ESP.deepSleep(SECONDS_DS(10), WAKE_RF_DEFAULT);
  // Execution resumes at the top of this function.
  delay(100);
}
