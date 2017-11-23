

void turnOff(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
}

void setup() {
  Serial.begin(9600);

  // disable all output to save power
  turnOff(0);
  turnOff(2);
  turnOff(4);
  turnOff(5);
  turnOff(12);
  turnOff(13);
  turnOff(14);
  turnOff(15);

    delay(10);
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.println("closing connection. going to sleep...");
  delay(1000);
  // go to deepsleep for 10 minutes
  //system_deep_sleep_set_option(0);
  system_deep_sleep(10 * 60 * 1000000);
}
