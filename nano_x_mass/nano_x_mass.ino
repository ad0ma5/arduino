/*
#define LED_BUILTIN1 2
void setup() {
    pinMode(LED_BUILTIN1, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN1, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN1, LOW);
    delay(1000);
}
*/
/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-oled
 */
/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#include "pitches.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 5


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000011, B11000000,
  B00001100, B10110000,
  B00010000, B10001000,
  B00100001, B10000100,
  B01000001, B01000010,
  B01000010, B01000010,
  B10000010, B00100001,
  B10000100, B00100001,
  B10000100, B00010001,
  B10001000, B00010001,
  B01010000, B00001010,
  B01111111, B11111111,
  B00100000, B00000100,
  B01010000, B00001010,
  B01001100, B00110010,
  B10000011, B11000001 };
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};


int LDR1 = 0;
int LDR2 = 0;
int oldLDR1 = 0;
int oldLDR2 = 0;
String status = ""; 
int steps = 0;
int oldState = LOW;
int direction = 1;
  int min1 = 1000;
  int max1 = 0;
  int min2 = 1000;
  int max2 = 0;
int average1 = 0;
int average2 = 0;
int dt1 = 0;
int dt2 = 0;

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(200);

/*
  // Clear the buffer.
  display.clearDisplay();

  // draw scrolling text
  testscrolltext();
  delay(2000);
  display.clearDisplay();


  // miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  delay(1);

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  display.clearDisplay();
  // draw a bitmap icon and 'animate' movement
  display.clearDisplay();
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
*/
  pinMode(2, INPUT);//motor step
  pinMode(9, OUTPUT);//motor11
  pinMode(10, OUTPUT);//motor12
  pinMode(11, OUTPUT);//motor21
  pinMode(12, OUTPUT);//motor22

  pinMode(A2, INPUT);//LDR1
  pinMode(A3, INPUT);//LDR2

  pinMode(5, OUTPUT);//speaker1
  pinMode(6, OUTPUT);//speaker2
  printState("WILL CALIBRATE!");
  calibrate();
  printState("INIT");
  playTune();
}

void loop() {
  int stepped = 0;
  int stepState = digitalRead(2);
  if(stepState != oldState){
    steps += stepState * direction;
    oldState = stepState;
    stepped = 1;
  } 
  LDR1 = analogRead(A2);
  LDR2 = analogRead(A3);
 //noTone(6);
 //tone(5, 440, 5); 
  dt1 = (min1 - LDR1) > (LDR1 - max1) ? (min1 - LDR1) : (LDR1 - max1); 
  dt2 = (min2 - LDR2) > (LDR2 - max2) ? (min2 - LDR2) : (LDR2 - max2); 
  //dt2 = LDR2 - min2) > abs(max2 - LDR2) ? abs(LDR2 - min2) :  abs(max2 - LDR2); 
  //if(oldLDR1 > LDR1 +50 ){
if(dt1 > 20 || dt2 > 20) {
  if( 
    dt1 > dt2
    //&&
    //LDR1 <  min1 //- (average1 - min1) 
    //||
    //LDR1 >  max1 //- (average1 - max1) 
  ){
    if(direction == 1) return;
    direction = 1;
    status = "<<";
    printState("LEFT!");
    tone(6, 880, 5); 
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
  //}else if(oldLDR2 > LDR2 +50){
  }else if(
    dt1 < dt2
    //&&
    //LDR2 < min2 //- (average2 - min2) 
    //||
    //LDR2 > max2 //- (average2 - max2) 
  ){
    if(direction == -1) return;
    direction = -1;
    status = ">>";
    printState("RIGHT!");
    tone(6, 880, 5); 
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
  }
}else{
  if ( 
    abs(oldLDR1 - LDR1) > 1
    ||
    abs(oldLDR2 - LDR2) > 1
    || 
    stepped == 1
  ){ 
    printState("AWAIT");
  }
  
  if(direction != 0){
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    tone(6, 1760, 5); 
    status = "STOP";
    direction = 0;
  }
}
  oldLDR1 = LDR1;
  oldLDR2 = LDR2;
  dt1 = dt2 = 0;
  
}

void calibrate(){
  int sum1 = 0;
  int sum2 = 0;
  for(uint8_t i = 0; i < 20; i++){
    LDR1 = analogRead(A2);
    LDR2 = analogRead(A3);
    sum1 += LDR1;
    sum2 += LDR2;
    min1 = LDR1 < min1 ? LDR1 : min1;
    min2 = LDR2 < min2 ? LDR2 : min2;
    max1 = LDR1 > max1 ? LDR1 : max1;
    max2 = LDR2 > max2 ? LDR2 : max2;

    tone(6, 440*1, 5); 
    delay(200);
    printState("Calibrate");
  }
  average1 = sum1/20;
  average2 = sum2/20;
}

void printState(String title){
 //noTone(5);
 //tone(6, 880, 5); 
  //display.setTextSize(2);
  //display.setTextColor(WHITE);
  //display.println(0xDEADBEEF, HEX);
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.print(title);
  display.print(" status: ");
  display.println(status);

  display.print("LDR1: ");
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(LDR1);
  //display.setTextColor(WHITE);
  display.print(" LDR2: ");
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(LDR2);

  display.print("steps: ");
  display.println(steps);

  //display.print(" min1: ");
  //display.print(min1);
  //display.print(" max1: ");
  //display.println(max1);

  //display.print(" min2: ");
  //display.print(min2);
  //display.print(" max2: ");
  //display.println(max2);

  display.print(" avr1: ");
  display.print(average1);
  display.print(" avr2: ");
  display.println(average2);

  display.print(" dt1: ");
  display.print(dt1);
  display.print(" dt2: ");
  display.print(dt2);

  display.display();
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  //while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
   //}
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(1);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  delay(1);
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}


void playTune() {

  // iterate over the notes of the melody:

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(5, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(5);

  }
}
