const int numButtons = 11;
const int buttonPins[numButtons] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
bool buttonStates[numButtons];       // Current button state
bool lastButtonStates[numButtons];   // Previous button state
unsigned long lastDebounceTimes[numButtons]; // Last debounce time
const unsigned long debounceDelay = 50;      // 50 ms debounce
const int ledPin = 13;
bool ledState = false;
char bufer[20];
void setup() {
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonStates[i] = HIGH;  // not pressed
    buttonStates[i] = HIGH;
    lastDebounceTimes[i] = 0;
  }
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttonPins[i]);
    // If button state changed
    if (reading != lastButtonStates[i]) {
      lastDebounceTimes[i] = millis();
    }
    // Check debounce delay
    if ((millis() - lastDebounceTimes[i]) > debounceDelay) {
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;
        // If button is pressed (goes LOW)
        if (buttonStates[i] == LOW) {
          ledState = HIGH;
          digitalWrite(ledPin, ledState);
          sprintf(bufer,"digital %d 0", buttonPins[i]);
          Serial.println(bufer);
        }else{
          ledState = LOW;
          sprintf(bufer,"digital %d 1", buttonPins[i]);
          Serial.println(bufer);
        }
      }
    }
    // Handle button release (rising edge) immediately, no debounce
    if (lastButtonStates[i] == LOW && reading == HIGH) {
      ledState = false;
      digitalWrite(ledPin, ledState);   // Turn LED off
    }
    lastButtonStates[i] = reading;
  }
}













