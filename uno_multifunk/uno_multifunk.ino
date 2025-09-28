/* Define shift register pins used for seven segment display */
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

#define Pot1 0
const int buzzerPin = 3;
int PotValue;
int lastPotValue = -1;
int freq = 0;

/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

const int buttonPins[3] = {A1, A2, A3};  // Buttons
const int ledPins[3]    = {10, 11, 12}; // LEDs
    int butpress = 0;                                     //
    bool toned = false;
// Debounce
const unsigned long DEBOUNCE_MS = 100;
unsigned long lastDebounceTime[3] = {0,0,0};
bool buttonState[3] = {false,false,false};     // stable state
bool lastButtonState[3] = {false,false,false}; // for change detection
    bool pressed = false; // threshold for shield buttons

void setup ()
{
  /* Set DIO pins to outputs */
  pinMode(LATCH_DIO,OUTPUT);
  pinMode(CLK_DIO,OUTPUT);
  pinMode(DATA_DIO,OUTPUT);
  for (int i = 0; i < 3; i++) {
    //pinMode(buttonPins[i], INPUT_PULLUP); // Buttons (pressed = LOW)
    pinMode(ledPins[i], OUTPUT);          // LEDs
    digitalWrite(ledPins[i], HIGH);        // All LEDs OFF
  }
  pinMode(13,OUTPUT);
    //pinMode(A1, INPUT_PULLUP); // Buttons (pressed = LOW)
}

/* Main program */
void loop()
{
      tone(buzzerPin, 0);
  delay(10);
  unsigned long now = millis();
  PotValue = analogRead(Pot1);
  // Map to audible frequency (say 100 Hz – 2000 Hz)
  freq = map(PotValue, 0, 1023, 20, 10000);

  // Play tone on buzzer

  /* Update the display with the current counter value */

  //tone(buzzerPin, freq);

      butpress = 0;
bool anyPressed = false;
  for (int i = 0; i < 3; i++) {

    int raw = analogRead(buttonPins[i]);
    bool pressed = (raw > 10); // threshold for shield buttons
    if (pressed != lastButtonState[i]) {
      lastDebounceTime[i] = now;
      lastButtonState[i] = pressed;
    }

    if ((now - lastDebounceTime[i]) > DEBOUNCE_MS) {
      if (buttonState[i] != pressed) {
        buttonState[i] = pressed;

        // LED reflects state
        if(!pressed) anyPressed = true ;
        digitalWrite(ledPins[i], pressed ? HIGH : LOW);
        freq += freq*(i+1);
        tone(buzzerPin, freq);
        delay(10);
      }
    }
    /*
    if (analogRead(buttonPins[i]) > 0) {
      digitalWrite(ledPins[i], HIGH); // Turn OFF if pressed
    } else {
      digitalWrite(ledPins[i], LOW);  // on otherwise
      //butpress = 0;
      //tone(buzzerPin, 40*i);
      freq += 40*i;
      butpress = 1;
    }
    */
  }

  if (anyPressed) {
     // tone(buzzerPin, freq);
      digitalWrite(13, HIGH); // Turn ON if pressed
  } else {
      tone(buzzerPin, 0);
      digitalWrite(13, LOW); // Turn ON if pressed
  }
  // --- Tone only if *any* button pressed ---
  //bool anyPressed = buttonState[0] || buttonState[1] || buttonState[2];
  // --- Update screen only if something changed ---
  if (PotValue/10 != lastPotValue/10){// || anyPressed != (lastButtonState[0] || lastButtonState[1] || lastButtonState[2])) {
    lastPotValue = PotValue;

     // tone(buzzerPin, freq);
    WriteNumberToSegment(0 , freq / 1000);
    WriteNumberToSegment(1 , (freq  / 100) % 10);
    WriteNumberToSegment(2 , (freq / 10) % 10);
    WriteNumberToSegment(3 , freq % 10);
    delay(10);
  } else {
    WriteNumberToSegment(0 , PotValue / 1000);
    WriteNumberToSegment(1 , (PotValue / 100) % 10);
    WriteNumberToSegment(2 , (PotValue / 10) % 10);
    WriteNumberToSegment(3 , PotValue % 10);
    delay(10);
    // Example: display pot value across 4 digits
  }
/*
  if (butpress == 1){
      digitalWrite(13, HIGH); // Turn ON if pressed
    if(toned){
      toned = false;
    }
      noTone(buzzerPin);
    WriteNumberToSegment(0 , PotValue / 1000);
    WriteNumberToSegment(1 , (PotValue / 100) % 10);
    WriteNumberToSegment(2 , (PotValue / 10) % 10);
    WriteNumberToSegment(3 , PotValue % 10);
  }else{
      digitalWrite(13, LOW); // Turn ON if pressed

    if(!toned){
      toned = true;
      tone(buzzerPin, freq);
    }
    WriteNumberToSegment(0 , freq / 1000);
    WriteNumberToSegment(1 , (freq / 100) % 10);
    WriteNumberToSegment(2 , (freq / 10) % 10);
    WriteNumberToSegment(3 , freq % 10);

  }
  */

  /*
  int val = analogRead(A1);
if(val > 15) {
      digitalWrite(10, LOW); // Turn ON if pressed
  // button pressed
}else{
      digitalWrite(10, HIGH); // Turn ON if pressed

}*/
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value] & 0x7F);// & 0x7F to add '.'
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO,HIGH);
}
