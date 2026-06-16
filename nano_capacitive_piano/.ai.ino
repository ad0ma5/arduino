// 13 Piano Keys mapped directly to Nano Digital Pins
const int totalKeys = 13;
const int keyPins[totalKeys] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; 
const int sendPin = A1; // Common transmitter pin

// Tuning parameters for 1M resistors + Beer caps
const int chargeDelayMicros = 4;   // Pure speed: microsecond time window to sample mid-charge
const int debounceInterval = 8;    // 8 milliseconds debounce (Imperceptible lag, no double-triggers)

// Tracking states for loop speed
unsigned long lastDebounceTime[totalKeys] = {0};
bool currentKeyState[totalKeys] = {false};
bool lastRawState[totalKeys] = {false};

void setup() {
  Serial.begin(115200); // High baud rate to prevent serial buffer lag
  
  pinMode(sendPin, OUTPUT);
  digitalWrite(sendPin, LOW);
  
  for(int i = 0; i < totalKeys; i++) {
    pinMode(keyPins[i], INPUT);
  }
}

void loop() {
  unsigned long currentTime = millis();
  bool rawReads[totalKeys];

  // --- STEP 1: ULTRA-FAST PARALLEL CAPACITIVE SENSING ---
  // Discharge all caps
  digitalWrite(sendPin, LOW);
  for(int i = 0; i < totalKeys; i++) pinMode(keyPins[i], OUTPUT);
  delayMicroseconds(10); 
  
  // Set back to inputs and fire charge pulse
  for(int i = 0; i < totalKeys; i++) pinMode(keyPins[i], INPUT);
  digitalWrite(sendPin, HIGH);
  
  // Wait precisely for human capacitance to lag the voltage rise
  delayMicroseconds(chargeDelayMicros); 
  
  // Take snapshot of raw states immediately
  for(int i = 0; i < totalKeys; i++) {
    // If a cap is touched, its voltage rises SLOWER, reading LOW (0) at sample time
    rawReads[i] = (digitalRead(keyPins[i]) == LOW); 
  }

  // --- STEP 2: ZERO-LAG STATE-MACHINE DEBOUNCE ---
  for(int i = 0; i < totalKeys; i++) {
    // Check if raw data fluctuated
    if (rawReads[i] != lastRawState[i]) {
      lastDebounceTime[i] = currentTime; // Reset noise timer
    }

    // If the signal stabilizes longer than the 8ms window, update the register
    if ((currentTime - lastDebounceTime[i]) > debounceInterval) {
      if (rawReads[i] != currentKeyState[i]) {
        currentKeyState[i] = rawReads[i];

        // Trigger your sound engine cleanly without false positives
        if (currentKeyState[i] == true) {
          triggerNoteOn(i);
        } else {
          triggerNoteOff(i);
        }
      }
    }
    lastRawState[i] = rawReads[i];
  }
}

// Action functions - replace with your buzzer/MIDI code
void triggerNoteOn(int keyIndex) {
  Serial.print("NOTE_ON: "); 
  Serial.println(keyIndex);
}

void triggerNoteOff(int keyIndex) {
  Serial.print("NOTE_OFF: "); 
  Serial.println(keyIndex);
}

