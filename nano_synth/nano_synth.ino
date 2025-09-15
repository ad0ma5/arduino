#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------ Pins ------------------
const int buttonPins[] = {2, 9, 4, 5, 6, 7};  // 6 buttons
const int numButtons = 6;

const int potFreq = A6;
const int potLen  = A7;

const int ldr1 = A0;
const int ldr2 = A1;

const int pwmOut = 3;
const int playButton = 8;

// ------------------ Scale ------------------
const float scale[] = {1.0, 1.122, 1.26, 1.335, 1.498, 1.682};

// ------------------ Sequencer ------------------
const int maxNotes = 16;
int recordedNotes[maxNotes];
int noteCount = 0;       // how many notes currently stored
bool hasRecording = false;

int playIndex = 0;       // playback index
bool noteOn = false;
unsigned long noteTimer = 0;
unsigned long noteInterval = 500;
int currentFreq = 0;

unsigned long lastOLEDUpdate = 0;
bool prevPlayState = false;

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for (;;) ; }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(playButton, INPUT_PULLUP);

  display.setCursor(0, 0);
  display.println(F("Synth Ready"));
  display.display();
  delay(1000);
}

void loop() {
  unsigned long now = millis();
  bool isPlaying = digitalRead(playButton) == LOW;

  // --- Read pots & LDRs ---
  int potFreqRaw = analogRead(potFreq);
  int potLenRaw  = analogRead(potLen);
  float baseFreq = map(potFreqRaw, 0, 1023, 100, 2000);

  float lfoFreq = map(analogRead(ldr1), 0, 1023, -50, 50);
  int lfoLen = map(analogRead(ldr2), 0, 1023, -200, 200);

  noteInterval = constrain(potLenRaw + lfoLen, 10, 2000);

  if (isPlaying) {
    // --- Entered Play Mode ---
    if (isPlaying && !prevPlayState) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(F("Playing..."));
      display.display();
      playIndex = 0;
      noteOn = false;
      noteTimer = now;
    }
    prevPlayState = isPlaying;

    if (hasRecording && noteCount > 0) {
      // --- Play back recorded notes ---
      if (now - noteTimer >= noteInterval) {
        noteTimer = now;
        noteOn = !noteOn;

        if (noteOn) {
          int note = recordedNotes[playIndex];
          if (note > 0) {
            currentFreq = baseFreq * scale[note - 1] + lfoFreq;
            tone(pwmOut, currentFreq);
          }
        } else {
          noTone(pwmOut);
          playIndex = (playIndex + 1) % noteCount;
        }
      }
    } else {
      // --- Live mode if no sequence ---
      int noteFreq = 0;
      for (int i = 0; i < numButtons; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {
          noteFreq = baseFreq * scale[i] + lfoFreq;
          break;
        }
      }
      if (noteFreq > 0) {
        tone(pwmOut, noteFreq);
      } else {
        noTone(pwmOut);
      }
    }
  } else {
    // --- Stop playback ---
    noTone(pwmOut);
    prevPlayState = false;

    // --- Record mode ---
    for (int i = 0; i < numButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        if (noteCount >= maxNotes) {
          // reached full capacity → clear and restart recording
          for (int j = 0; j < maxNotes; j++) recordedNotes[j] = 0;
          noteCount = 0;
          hasRecording = false;
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println(F("Cleared (auto)"));
          display.display();
          delay(500);
        }

        // record note
        recordedNotes[noteCount] = i + 1; // store note index (1-based)
        noteCount++;
        hasRecording = true;
        delay(200); // debounce
      }
    }

    // --- OLED update every 500ms ---
    if (now - lastOLEDUpdate >= 500) {
      lastOLEDUpdate = now;
      display.clearDisplay();
      display.setTextSize(1);

      display.setCursor(0, 0);
      display.println("Recorded Notes:");

      // show up to 16 notes in 2 rows
      display.setCursor(0, 10);
      for (int i = 0; i < 8; i++) {
        if (i < noteCount) display.print(recordedNotes[i]);
        else display.print("-");
        display.print(" ");
      }

      display.setCursor(0, 20);
      for (int i = 8; i < 16; i++) {
        if (i < noteCount) display.print(recordedNotes[i]);
        else display.print("-");
        display.print(" ");
      }

      display.setCursor(0, 40);
      display.print("Freq:"); display.println((int)baseFreq);
      display.setCursor(0, 50);
      display.print("Interval:"); display.println(noteInterval);

      display.display();
    }
  }
}

