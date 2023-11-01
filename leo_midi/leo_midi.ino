/*
  Based on Sketch built by Gustavo Silveira (aka Music Nerd)
  Modified by Dolce Wang
  Modified by Ad0mA5

  This code is only for Arduinos that use ATmega32u4 (like Micro, Pro Micro, Leonardo...)
  Remember to also assign the correct board in the IDE (like Tools / Boards / Sparkfun AVR / Pro Micro...)

*/

// Change any fields with //**


// LIBRARY
#include <CapacitiveSensor.h>

#include "MIDIUSB.h"  

// BUTTONS
const int NButtons = 1; //***  total number of buttons
const int buttonPin[NButtons] = {13};
//{0,1,2,4,5, 7,8,9,10,A0,A1, A2,A3,A4,A5}; //*** define Digital Pins connected from Buttons to Arduino; (ie {10, 16, 14, 15, 6, 7, 8, 9, 2, 3, 4, 5}; 12 buttons)
int buttonCState[NButtons] = {};        // stores the button current value
int buttonPState[NButtons] = {};        // stores the button previous value
      
// debounce
unsigned long lastDebounceTime[NButtons] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    //** the debounce time; increase if the output flickers


// POTENTIOMETERS
const int NPots = 4; //*** total number of pots (knobs and faders)

const int potPin[NPots] = {A5, A4, A3, A2}; //*** define Analog Pins connected from Pots to Arduino; Leave nothing in the array if 0 pots {}
const int potPinVals[NPots] = {1, 2, 7, 71};
int potCState[NPots] = {0}; // Current state of the pot; delete 0 if 0 pots
int potPState[NPots] = {0}; // Previous state of the pot; delete 0 if 0 pots
int potVar = 0; // Difference between the current and previous state of the pot

int midiCState[NPots] = {0}; // Current state of the midi value; delete 0 if 0 pots
int midiPState[NPots] = {0}; // Previous state of the midi value; delete 0 if 0 pots

const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[NPots] = {0}; // Previously stored time; delete 0 if 0 pots
unsigned long timer[NPots] = {0}; // Stores the time that has elapsed since the timer was reset; delete 0 if 0 pots


// MIDI Assignments 
byte midiCh = 1; //* MIDI channel to be used
byte note = 60; //* Lowest note to be used; 36 = C2; 60 = Middle C
byte cc = 1; //* Lowest MIDI CC to be used
//bool connected = false;


// CAPACCITIVE TOUCH
const int sendPin = A0;
const int Ncaps = 13; //*** total number of capacitive touchers

const int capPin[Ncaps] = {
0,1,2
,3,4,5,6,7,8,9,10,11,12
}; //*** define Digital Pins connected from Touchpads to Arduino; Leave nothing in the array if 0 pots {}
const CapacitiveSensor capSensors[Ncaps] = { 
  CapacitiveSensor(sendPin, capPin[0]),
  CapacitiveSensor(sendPin, capPin[1]),
  CapacitiveSensor(sendPin, capPin[2]),
  CapacitiveSensor(sendPin, capPin[3]),
  CapacitiveSensor(sendPin, capPin[4]),
  CapacitiveSensor(sendPin, capPin[5]),
  CapacitiveSensor(sendPin, capPin[6]),
  CapacitiveSensor(sendPin, capPin[7]),
  CapacitiveSensor(sendPin, capPin[8]),
  CapacitiveSensor(sendPin, capPin[9]),
  CapacitiveSensor(sendPin, capPin[10]),
  CapacitiveSensor(sendPin, capPin[11]),
  CapacitiveSensor(sendPin, capPin[12])
//*/
};
int capCState[Ncaps] = {0}; // Current state of the pad; delete 0 if 0 pads
int capPState[Ncaps] = {0}; // Previous state of the pad; delete 0 if 0 pads
unsigned long capSumVal[Ncaps] = {0};
//int capVar = 0; // Difference between the current and previous state of the pad

//initialize capacitive sense 
//unsigned long csSum;
//unsigned long csSum12;
//unsigned long csSum6;

//CapacitiveSensor   cs_6_11 = CapacitiveSensor(6,11);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
//CapacitiveSensor   cs_6_12 = CapacitiveSensor(6,12);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// To add more sensors...


//speaker
const int speakerPin = A1;    
const int speakerNotes[Ncaps] = {
440,466,494,523,554 , 
587,622,659,698,740, 
784,831,880
//,932,988, 1047
//,1109,1175
//,1245
};
int toned = 0;
int startPress13 = 0;

// SETUP
void setup() {
  Serial.begin(9600);
  // Baud Rate
  // 31250 for MIDI class compliant | 115200 for Hairless MIDI

  // Buttons
  // Initialize buttons with pull up resistors
  for (int i = 0; i < NButtons; i++) {
    //pinMode(buttonPin[i], INPUT_PULLUP);
  }
    //pinMode(A0, INPUT_PULLUP);
  // Init capacitive touch pins
  for (int i = 0; i < Ncaps; i++) {
 //   CapacitiveSensor   cs = CapacitiveSensor(sendPin, capPin[i]);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
   capSensors[i].set_CS_AutocaL_Millis(0xFFFFFFFF);  //Calibrate the sensor... pinMode(led, OUTPUT);
  // capSensors[i] = cs;
  }

}

////
// LOOP
void loop() {
/*
	if(MidiUSB.isAvailable()){
		connected = true;
	}else{
		connected = false;
	}
*/
  //buttons();
  potentiometers();
  capacitiveButtons();

}

////
// BUTTONS
void buttons() {

  for (int i = 0; i < NButtons; i++) {

    buttonCState[i] = digitalRead(buttonPin[i]);  // read pins from arduino

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

        if (buttonCState[i] == LOW) {
          if(buttonPin[i] == 13){
						//toned = !toned;
						startPress13 = millis();
						//tone(speakerPin, 880, 500);
					}
	          if(toned == 1) 
              tone(speakerPin, speakerNotes[i], 3);
	          if(toned == 2) 
              tone(speakerPin, speakerNotes[i]);
            //toned = true;
          // Sends the MIDI note ON 
          
         // use if using with ATmega32U4 (micro, pro micro, leonardo...)
          noteOn(midiCh, note + i, 127);  // channel, note, velocity
          MidiUSB.flush();
        }
        else {

	        if(toned == 2) 
              noTone(speakerPin);
          if(buttonPin[i] == 13){
              //tone(speakerPin, speakerNotes[i], millis() - startPress13);
						startPress13 = 0;
						//toned = false;
					}
          // Sends the MIDI note OFF accordingly to the chosen board

          // use if using with ATmega32U4 (micro, pro micro, leonardo...)
          noteOn(midiCh, note + i, 0);  // channel, note, velocity
          MidiUSB.flush();
          


        }
        buttonPState[i] = buttonCState[i];
				//if(millis() - startPress13 > 2500) startPress13 = 0;
      }
    }
		if(buttonPin[i] == 13 && startPress13 >= 1){
			int dt = millis() - startPress13;
			if(dt  >= 5000){
			//tone(speakerPin, 440, dt);
			//delay(1000);
			tone(speakerPin, 440, 10);
			tone(speakerPin, 440, 10);
			toned = toned+1;
			if(toned >= 3) toned = 0;
			startPress13 = 0;
			}
		}
  }
}

////
// POTENTIOMETERS
void potentiometers() {


  for (int i = 0; i < NPots; i++) { // Loops through all the potentiometers

    potCState[i] = analogRead(potPin[i]); // reads the pins from arduino

    midiCState[i] = map(potCState[i], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi

    potVar = abs(potCState[i] - potPState[i]); // Calculates the absolute value between the difference between the current and previous state of the pot

    if (potVar > varThreshold) { // Opens the gate if the potentiometer variation is greater than the threshold
      PTime[i] = millis(); // Stores the previous time
    }

    timer[i] = millis() - PTime[i]; // Resets the timer 11000 - 11000 = 0ms

    if (timer[i] < TIMEOUT) { // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
      potMoving = true;
    }
    else {
      potMoving = false;
    }

    if (potMoving == true) { // If the potentiometer is still moving, send the change control
      if (midiPState[i] != midiCState[i]) {

        // Sends  MIDI CC 
        // Use if using with ATmega32U4 (micro, pro micro, leonardo...)
        controlChange(midiCh, potPinVals[i], midiCState[i]); //  (channel, CC number,  CC value)
        MidiUSB.flush();

        potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
        midiPState[i] = midiCState[i];
      }
    }
  }
}


// if using with ATmega32U4 (micro, pro micro, leonardo...)


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


bool trash = false;
unsigned long cs_vals[Ncaps] = {0};
void capacitiveButtons(){
  for (int i = 0; i < Ncaps; i++) {
    unsigned long cs_val = capSensors[i].capacitiveSensor(50); //a: Sensor resolution is set to 80
    long percent = 1;
    if(cs_vals[i] != 0 && cs_val > 1000){
      //percent of +dt comparing to previous value
      if (cs_val > cs_vals[i] *10){
        percent = 100;
      }else{
        percent = cs_val * 100 / (cs_vals[i]);
      }
    }
    if (
      cs_vals[i] == 0
      &&
      cs_val > 1000
    ){
      percent = 51;
    }
/*
if(Serial){
Serial.print(i);
Serial.print(": ");
Serial.print(cs_val);
Serial.print(" / ");
Serial.print(cs_vals[i]);
Serial.print(" = ");
Serial.println(percent);
if(percent > 50){
Serial.println(" more ");
}else{
Serial.println(" less ");
}
delay(500);
}
// */
		if (
      //cs_val > 100 
      //percent > 50
      true
    ) { //b: Arbitrary number
			//csSum += cs;
			//capCState[i] += cs_val;  // read pins from arduino
			//capSumVal[i] += cs_val;  // read pins from arduino
			//Serial.println(cs); 
			if (
        //capSumVal[i] >= 1500
        percent > 50
      ) //c: This value is the threshold, a High value means it takes longer to trigger
			{

				capCState[i] = true;
        if (capPState[i] != capCState[i]) {

//Serial.println(capSumVal[i]);
//if(Serial){Serial.println('Y');}
         // use if using with ATmega32U4 (micro, pro micro, leonardo...)
          noteOn(midiCh, note + i, 127);  // channel, note, velocity
          MidiUSB.flush();

	          if(toned == 1) 
              tone(speakerPin, speakerNotes[i], 10);
	          if(toned == 2) 
              tone(speakerPin, speakerNotes[i], 500);

          capPState[i] = capCState[i];
        }
				//trash = true;//!trash;
				//if (trash) digitalWrite(13, HIGH);
				//else digitalWrite(13, LOW);
				//Serial.print("Trigger: ");
				//Serial.println(csSum);
				//if (csSum > 0) { csSum = 0; } //Reset
				if(capSumVal[i] > 0 ) { capSumVal[i] = 0; } //reset		
				capSensors[i].reset_CS_AutoCal(); //Stops readings

			}else{
				capCState[i] = false;

        if (capPState[i] != capCState[i]) {
/*
Serial.print(i);
Serial.print(' ');
Serial.println(cs_val);
Serial.println(capSumVal[i]);
*/
//if(Serial){Serial.println('N');}
         // use if using with ATmega32U4 (micro, pro micro, leonardo...)
          // use if using with ATmega32U4 (micro, pro micro, leonardo...)
          noteOn(midiCh, note + i, 0);  // channel, note, velocity
          MidiUSB.flush();

          capPState[i] = capCState[i];
        }
      }  
		} else {
			 capSumVal[i] = 0; //Timeout caused by bad readings
		}
    cs_vals[i] = cs_val;
  }//end for

}
