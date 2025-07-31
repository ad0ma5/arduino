const int numButtons = 17;
const int buttonPins[numButtons] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0, A1, A2, A3, A4, A5};
bool buttonStates[numButtons];       // Current button state
bool lastButtonStates[numButtons];   // Previous button state
unsigned long lastDebounceTimes[numButtons]; // Last debounce time
const unsigned long debounceDelay = 5;      // 50 ms debounce
const int ledPin1 = 13;
const int ledPin2 = 13;
int ledPin = 13;
bool ledState[2] = {LOW, LOW};
char bufer[20];
int pinState = 0;
void setup() {
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonStates[i] = HIGH;  // not pressed
    buttonStates[i] = HIGH;
    lastDebounceTimes[i] = 0;
  }
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  //pinMode(A7, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  for (int i = 0; i < numButtons; i++) {
    if (i < 11){
      ledPin = ledPin1;
      pinState = 0; 
    }else{
      ledPin = ledPin2;
      pinState = 1;
    }
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
          ledState[pinState] = HIGH;
          digitalWrite(ledPin, ledState[pinState]);
          sprintf(bufer,"digital %d 0", buttonPins[i]);
          Serial.println(bufer);
        }else{
          ledState[pinState] = LOW;
          digitalWrite(ledPin, ledState[pinState]);   // Turn LED off
          sprintf(bufer,"digital %d 1", buttonPins[i]);
          Serial.println(bufer);
        }
      }
    }
    // Handle button release (rising edge) immediately, no debounce
    if (lastButtonStates[i] == LOW && reading == HIGH) {
      ledState[pinState] = LOW;
      digitalWrite(ledPin, ledState[pinState]);   // Turn LED off
      sprintf(bufer,"digital %d 1", buttonPins[i]);
    }
    lastButtonStates[i] = reading;
  }
}













