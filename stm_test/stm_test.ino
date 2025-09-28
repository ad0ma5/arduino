// Blink_STM32.ino
void setup() {
  pinMode(PC13, OUTPUT); // PC13 is the user LED on many Blue Pill boards
}

void loop() {
  digitalWrite(PC13, LOW);   // LED on (on Blue Pill PC13 is typically active-low)
  delay(500);
  digitalWrite(PC13, HIGH);  // LED off
  delay(500);
}

