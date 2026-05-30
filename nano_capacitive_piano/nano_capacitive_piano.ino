#include "CapacitiveSensor.h"



// CAPACCITIVE TOUCH
const int sendPin = A1;
const int Ncaps = 13; //*** total number of capacitive touchers
const int ledPin = 13;

const int capPin[Ncaps] = {
  0,2,1,3,4,5,6,7,8,9,10,11,12
}; //*** define Digital Pins connected from Touchpads to Arduino; Leave nothing in the array if 0 pots {}
   //initialize capacitive sense 
const CapacitiveSensor capSensors[Ncaps] = { 
  CapacitiveSensor(sendPin, capPin[0]),
  CapacitiveSensor(sendPin, capPin[2]),
  CapacitiveSensor(sendPin, capPin[1]),
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
unsigned long capLow[Ncaps] = {0};
unsigned long capHigh[Ncaps] = {0};
unsigned long cs_vals[Ncaps] = {0};
// debounce
unsigned long lastDebounceTimeCap[Ncaps] = {0};  // the last time the output pin was toggled
unsigned long debounceDelayCap = 10;    //** the debounce time; increase if the output flickers

// SETUP
void setup() {
  //Serial.begin(9600);
  // Baud Rate
  // 31250 for MIDI class compliant | 115200 for Hairless MIDI
  pinMode(ledPin, OUTPUT);
  // Init capacitive touch pins
  for (int i = 0; i < Ncaps; i++) {
    capSensors[i].set_CS_AutocaL_Millis(0xFFFFFFFF);  //Calibrate the sensor... pinMode(led, OUTPUT);
  }

}
////
// LOOP
void loop() {
  capacitiveButtons();
}


void capacitiveButtons(){
  for (int i = 0; i < Ncaps; i++) {
    //if ((millis() - lastDebounceTimeCap[i]) < debounceDelayCap * 10 ) continue;
    //if(potCState[3] != debounceDelayCap) debounceDelayCap = potCState[3];;
    //get single cap button reading
    unsigned long cs_val = capSensors[i].capacitiveSensor(debounceDelayCap); //a: Sensor resolution is set to 80
    if(cs_val < 5*debounceDelayCap) continue;
    digitalWrite(ledPin, LOW);
    long percent = 1;//init small but non zero
    if(capHigh[i] == 0){
      capHigh[i] = 1000;
    }
    //calculate score 0 to 127 by mapping current reading val to min < val < max
    int score = map(cs_val, capLow[i], capHigh[i], 0, 127);

    /*
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
    */
    if (
        !capCState[i]
        &&
        //percent > 50
        score > 50
        //&&
       ) //c: This value is the threshold, a High value means it takes longer to trigger
    {
      capCState[i] = true;
      if (capPState[i] != capCState[i]) {

        digitalWrite(ledPin, HIGH);
        lastDebounceTimeCap[i] = millis();
        // use if using with ATmega32U4 (micro, pro micro, leonardo...)
        int extra = 0;
        //if(startPress13 != 0) extra = 12;
        //noteOn(midiCh, note + i + extra, 127);  // channel, note, velocity
        //MidiUSB.flush();
        //if(potCState[2] == 0){
          //int times = 1;
          //if(startPress13 != 0) times = 2;

          //if(toned == 1) 
            //tone(speakerPin, speakerNotes[i]*times, 10);
          //if(toned == 2) 
            //tone(speakerPin, speakerNotes[i]*times, 500);
        //}
        capPState[i] = capCState[i];
      }
      capSensors[i].reset_CS_AutoCal(); //Stops readings
                                        //new hight average
                                        //if(90 < score < 110)
                                        //capHigh[i] = capHigh[i] + cs_val / 2;

    }else 
      //if(
      //capCState[i] && score < 25 
      //)
      //if(percent < 0 && capCState[i])i
    {
      capCState[i] = false;

      if (capPState[i] != capCState[i]) {
        int extra = 0;
        //if(startPress13 != 0) extra = 12;
        //noteOn(midiCh, note + i + extra, 0);  // channel, note, velocity
        //MidiUSB.flush();

        capPState[i] = capCState[i];
      }
      //if(-10 < score < 10)
      //capLow[i] = capLow[i] + cs_val / 2;
    }  
    cs_vals[i] = cs_val;
  }//end for

}
