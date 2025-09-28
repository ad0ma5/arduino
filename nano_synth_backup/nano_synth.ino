#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------ Pins ------------------
const int buttonPins[] = {2,9,4,5,6,7};
const int numButtons = 6;

const int potFreq = A6;
const int potLen  = A7;

const int ldr1 = A0;
const int ldr2 = A1;

const int pwmOut = 3; 
const int playButton = 8;

// ------------------ Scale ------------------
const float scale[] = {1.0,1.122,1.26,1.335,1.498,1.682};

unsigned long lastOLEDUpdate = 0;
bool prevPlayState = false;

// ------------------ Sequencer state ------------------
bool noteOn = false;
unsigned long noteTimer = 0;
unsigned long noteInterval = 500; // default ms
int currentFreq = 0;

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){for(;;);}
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  for(int i=0;i<numButtons;i++){
      pinMode(buttonPins[i],INPUT_PULLUP);
  }
  pinMode(playButton,INPUT_PULLUP);

  display.setCursor(0,0);
  display.println(F("Synth Ready"));
  display.display();
  delay(1000);
}

void loop() {
    unsigned long now = millis();
    bool isPlaying = digitalRead(playButton) == LOW;

    // --- Detect D8 press transition ---
    if(isPlaying && !prevPlayState){
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(F("Playing..."));
        display.display();
    }
    prevPlayState = isPlaying;

    // --- Read pots ---
    int potFreqRaw = analogRead(potFreq);
    int potLenRaw  = analogRead(potLen);
    float baseFreq = map(potFreqRaw,0,1023,100,2000);

    // --- Read LDRs ---
    float lfoFreq = map(analogRead(ldr1),0,1023,-50,50);
    int lfoLen  = map(analogRead(ldr2),0,1023,-200,200);

    // total interval = pot + LDR
    noteInterval = constrain(potLenRaw + lfoLen, 50, 2000);

    if(isPlaying){
        // pick first pressed button
        currentFreq = 0;
        for(int i=0;i<numButtons;i++){
            if(digitalRead(buttonPins[i])==LOW){
                currentFreq = baseFreq * scale[i] + lfoFreq;
                break;
            }
        }

        // sequencer: toggle note on/off every interval
        if(now - noteTimer >= noteInterval){
            noteTimer = now;
            noteOn = !noteOn;
            if(noteOn && currentFreq>0){
                tone(pwmOut,currentFreq);
            } else {
                noTone(pwmOut);
            }
        }

    } else {
        // D8 OFF: mute sound
        noTone(pwmOut);
        noteOn = false;
        currentFreq = 0;

        // OLED update every 500ms
        if(now - lastOLEDUpdate >= 500){
            lastOLEDUpdate = now;
            display.clearDisplay();
            display.setTextSize(1);

            int y=0;
            display.setCursor(0,y);
            display.println("Buttons:");
            y+=8;
            for(int i=0;i<numButtons;i++){
                display.setCursor(0,y);
                display.print("B"); display.print(i+1); display.print(": ");
                display.print(digitalRead(buttonPins[i])==LOW?"ON":"OFF");
                y+=8;
            }

            int x=70; y=0;
            display.setCursor(x,y);
            display.print("Freq:"); display.println((int)baseFreq);
            y+=8;
            display.setCursor(x,y);
            display.print("Interval:"); display.println(noteInterval);
            y+=8;
            display.setCursor(x,y);
            display.print("LFO F:"); display.println((int)lfoFreq);
            y+=8;
            display.setCursor(x,y);
            display.print("LFO L:"); display.println((int)lfoLen);

            display.display();
        }
    }
}


/*
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------ Pins ------------------
const int buttonPins[] = {2,9,4,5,6,7}; // button pins
const int numButtons = 6;

const int potFreq = A6;
const int potLen  = A7;

const int ldr1 = A0;
const int ldr2 = A1;

const int pwmOut = 3; 
const int playButton = 8; // D8 toggle

// ------------------ Scale ------------------
const float scale[] = {1.0,1.122,1.26,1.335,1.498,1.682};

// ------------------ Oscillator struct ------------------
struct Oscillator {
  float phase;
  float freq;
  float amp;
  unsigned long lastToggle;
  bool state;
  unsigned long noteStart;
  unsigned long noteLength;
  bool noteOn;
};

Oscillator oscMain[numButtons];

unsigned long lastOLEDUpdate = 0;
bool prevPlayState = false; // previous D8 state

// ------------------ Setup ------------------
void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){for(;;);}
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  for(int i=0;i<numButtons;i++){
    pinMode(buttonPins[i],INPUT_PULLUP);
    oscMain[i]={0,0,0,0,false,0,0,false};
  }
  pinMode(pwmOut,OUTPUT);
  pinMode(playButton,INPUT_PULLUP);

  display.setCursor(0,0);
  display.println(F("Synth Ready"));
  display.display();
  delay(1000);
}

// ------------------ Loop ------------------
void loop(){
  unsigned long now = millis();
  bool isPlaying = digitalRead(playButton) == LOW;

  // Read pots
  int potFreqRaw = analogRead(potFreq);
  int potLenRaw  = analogRead(potLen);
  float baseFreq = map(potFreqRaw,0,1023,100,2000);
  unsigned long notePause = map(potLenRaw,0,1023,50,1000);

  // Read LDRs
  float lfoValFreq = map(analogRead(ldr1),0,1023,-50,50);
  float lfoValAmp  = map(analogRead(ldr2),0,1023,0,127);

  // --- Detect D8 press transition ---
  if(isPlaying && !prevPlayState){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(F("Playing..."));
      display.display();
  }
  prevPlayState = isPlaying;

  if(isPlaying){
      // --- Generate PWM sound ---
      int mixed = 0;
      for(int i=0;i<numButtons;i++){
          if(digitalRead(buttonPins[i])==LOW){
              if(!oscMain[i].noteOn){
                  oscMain[i].noteOn = true;
                  oscMain[i].noteStart = now;
                  oscMain[i].noteLength = notePause;
                  oscMain[i].freq = baseFreq*scale[i]+lfoValFreq;
                  oscMain[i].amp = 127;
                  oscMain[i].phase = 0;
                  oscMain[i].lastToggle = micros();
                  oscMain[i].state = false;
              }
          }

          if(oscMain[i].noteOn && now - oscMain[i].noteStart >= oscMain[i].noteLength){
              oscMain[i].noteOn = false;
              oscMain[i].amp = 0;
          }

          if(oscMain[i].amp>0){
              unsigned long halfPeriod = 500000UL / oscMain[i].freq;
              if(micros() - oscMain[i].lastToggle >= halfPeriod){
                  oscMain[i].lastToggle = micros();
                  oscMain[i].state = !oscMain[i].state;
              }
              mixed += (oscMain[i].state ? oscMain[i].amp : -oscMain[i].amp);
          }
      }

      mixed = mixed*lfoValAmp/127;
      int outVal = mixed/numButtons + 128;
      outVal = constrain(outVal,0,255);
      analogWrite(pwmOut,outVal);

  } else {
      // --- D8 OFF: reset oscillators & mute ---
      analogWrite(pwmOut,128);
      for(int i=0;i<numButtons;i++){
          oscMain[i].noteOn = false;
          oscMain[i].amp = 0;
          oscMain[i].phase = 0;
          oscMain[i].state = false;
      }

      // OLED update every 500ms
      if(now - lastOLEDUpdate >= 500){
          lastOLEDUpdate = now;
          display.clearDisplay();
          display.setTextSize(1);

          int y=0;
          display.setCursor(0,y);
          display.println("Buttons:");
          y+=8;
          for(int i=0;i<numButtons;i++){
              display.setCursor(0,y);
              display.print("B"); display.print(i+1); display.print(": ");
              display.print(digitalRead(buttonPins[i])==LOW?"ON":"OFF");
              y+=8;
          }

          int x=70; y=0;
          display.setCursor(x,y);
          display.print("Freq:"); display.println((int)baseFreq);
          y+=8;
          display.setCursor(x,y);
          display.print("Len:"); display.println(notePause);
          y+=8;
          display.setCursor(x,y);
          display.print("LFO F:"); display.println((int)lfoValFreq);
          y+=8;
          display.setCursor(x,y);
          display.print("LFO A:"); display.println((int)lfoValAmp);

          display.display();
      }
  }
}
*/
