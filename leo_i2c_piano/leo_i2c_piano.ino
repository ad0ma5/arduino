#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MIDIUSB.h"  

#define DEBUG 0

#if DEBUG
  #define DBG(...) Serial.print(__VA_ARGS__)
  #define DBGLN(...) Serial.println(__VA_ARGS__)
#else
  #define DBG(...)
  #define DBGLN(...)
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define TRIG_PIN 12
#define ECHO_PIN 13 
#define ECHO_TIMEOUT 20 

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

struct ControllerData
{
  uint16_t pots[4];
  uint16_t touchBits;
};

ControllerData controllerData;
ControllerData previousData;

const byte NANO_ADDR = 0x10;

uint16_t receivedValue = 0;
uint8_t current_note = 13;

// Pot pins
const uint8_t potPins[6] =
{
A0, A1, A2, A3, A4, A5
};
uint16_t pots[6];

//buttons 
const uint8_t buttonPins[6] =
{
6,7,8,9,10,11
};
 uint8_t buttonVals[6] =
{
0,0,0,0,0,0
};
 uint8_t buttonValsOld[6] =
{
  1,1,1,1,1,1
};
uint8_t actualNotes[13] =
{
  1,2,0,3,4,5,6,7,8,9,10,11,12
};
// MIDI Assignments 
byte midiCh = 2; //* MIDI channel to be used
byte note = 60; //* Lowest note to be used; 36 = C2; 60 = Middle C
byte cc = 1; //* Lowest MIDI CC to be used
uint8_t cc_val = 0;
uint16_t pot_val = 0;
int trigPin1 = 12;
int echoPin1 = 13;
//
// Arduino MIDI functions MIDIUSB Library
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
float current_distance = 1;
int distance = 0;
int cdist = 0;
int cdistold = 0;

/////////////////////////////////////////////////////////////////////
void setup()
{
#if DEBUG
  //Serial.begin(115200);
#endif

  Wire.begin();

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();

  printLCD();
  //DBGLN("Leonardo Master Ready");
  for(int i=0;i<6;i++)
  {
    
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
}
/////////////////////////////////////////////////////////////////////
bool readButtons()
{

  bool changed = false;
  for(int i=0;i<6;i++)
  {
    buttonVals[i] = digitalRead(buttonPins[i]);
      if(buttonValsOld[i] != buttonVals[i])
      {
        //if(buttonVals[i] == HIGH)
          sendMIDI(10, i+1); 

        changed = true;
      }
  }

  return changed;
}

void printLCD()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("midi controlieour loading...");
    display.display();

    delay(5000);
    display.clearDisplay();
    //display.setTextSize(1);
    //display.setTextColor(SSD1306_WHITE);
}

void printTouches()
{
    bool foundNote = false;
    int newNote = 13;

    for(int ii=0; ii<13; ii++)
    {
        int i = actualNotes[ii];

        if(controllerData.touchBits & (1 << i))
        {
            foundNote = true;
            newNote = ii;
            break;  // first active note wins
        }
    }

    if(newNote != current_note)
    {
        if(current_note != 13)
        {
            noteOff(midiCh, note + current_note, 127);
        }

        if(newNote != 13)
        {
            noteOn(midiCh, note + newNote, 127);
        }

        current_note = newNote;
    }

    display.setCursor(40,0);
    display.print(current_note);
    display.print(":");
    for(int i=0;i<6;i++)
    {
        display.print(buttonVals[i]);
    }
}

uint16_t readPots()
{
  uint16_t changed = 0;
  for(int i = 0; i < 6; i++)
  {
    uint16_t v = analogRead(potPins[i]);

    // Simple jitter suppression
    if(abs((int)v - (int)pots[i]) > 2)
    {
      cc_val = map(v, 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi

      sendMIDI(i+4, cc_val); 
      pots[i] = v;
      changed = v;
    }
  }
  return changed;
}

void sendMIDI(uint8_t cc_no, uint8_t cc_val){
        // Sends  MIDI CC 
        // Use if using with ATmega32U4 (micro, pro micro, leonardo...)
        controlChange(midiCh, cc_no, cc_val); //  (channel, CC number,  CC value)
        MidiUSB.flush();
}

float ultrasonicMeasure() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  //float duration_us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT);
  float duration_us = pulseIn(ECHO_PIN, HIGH );

  // calculate the distance
  float distance_cm = 0.017 * duration_us;

  return distance_cm;
}
int getDistance(){

    // Trigger the current sensor
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);

    // Read the echo pin
    long duration = pulseIn(echoPin1, HIGH, 20000); // Timeout after 30ms

    // Calculate the distance
    distance = duration * 0.034 / 2; // Calculate distance
    return distance;                                     // 
}

void loop()
{
  //delay(100);
  Wire.requestFrom(
    NANO_ADDR,
    sizeof(ControllerData)
  );

  if (Wire.available() == sizeof(ControllerData))
  {
    Wire.readBytes(
      (char*)&controllerData,
      sizeof(ControllerData)
    );



    bool changed = false;

    if(readButtons()){
      changed = true;
    }
    if(readPots() != 0){
      changed = true;
    }
    //current_distance = ultrasonicMeasure();
    //if(current_distance > 0){
    //  changed = true;
    //}
    cdist = getDistance();
    if(cdist != cdistold){
        changed = true;
        cdistold = cdist;
        cc_val = map(cdist, 0, 165, 0, 127);
        sendMIDI(11, cc_val); 

    }
    for(int i=0;i<4;i++)
    {
      if(controllerData.pots[i] != previousData.pots[i])
      {

        cc_val = map(controllerData.pots[i], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi

        sendMIDI(i, cc_val); 
        changed = true;
        break;
      }
    }    
    if(controllerData.touchBits != previousData.touchBits)
    {
      changed = true;
    }
    if(changed)
    {
      display.clearDisplay();
      display.setTextSize(1);
      printTouches();
      display.setCursor(0, 20);
      display.print("P:");
      for(int i=0;i<4;i++)
      {
        display.print(controllerData.pots[i]);
        display.print(" ");
      }
      display.setCursor(0, 40);
      display.setTextSize(1);
      display.print("P:");
      for(int i=0;i<6;i++)
      {
        display.print(pots[i]);
        display.print(" ");
      }
        display.print("D:");
      display.print(cdist);
      display.display();
/*
      Serial.print(" TouchBits: ");

      Serial.println(
        controllerData.touchBits,
        BIN
      );
      */
    }




#if DEBUG
#endif

    memcpy(
      &previousData,
      &controllerData,
      sizeof(ControllerData)
    );
  }

}
