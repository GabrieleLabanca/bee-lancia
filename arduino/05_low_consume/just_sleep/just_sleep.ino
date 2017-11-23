#define RST_PIN D0

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.println("READY");

  
  digitalWrite(RST_PIN,HIGH); // ATTEMPT does it fix the automatic reset?
  blink(2);
}

void loop() {
  delay(1000);
  blink(3);
  //digitalWrite(RST_PIN,LOW);
  ESP.deepSleep(10*1000000UL,WAKE_RF_DEFAULT);
}

/////////////////////////////////////////////////////////////////////
void blink(int n) {
  static bool first_time = true;
  if (first_time == true) {
    pinMode(LED_BUILTIN, OUTPUT);
    first_time = false;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < n; ++i) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }

}
