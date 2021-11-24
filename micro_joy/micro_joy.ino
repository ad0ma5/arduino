// Simple example application that shows how to read four Arduino
// digital pins and map them to the USB Joystick library.
//
// The digital pins 9, 10, 11, and 12 are grounded when they are pressed.
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2015-11-20
//--------------------------------------------------------------------

#include "Mouse.h"
#include "Joystick.h"

const int ledPin =  A0;      // the number of the green LED pin

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(3, INPUT);//mouse ON/yellow led

  pinMode(4, INPUT);//down
  pinMode(5, INPUT);//left
  pinMode(6, INPUT);//up
  pinMode(7, INPUT);//right
  pinMode(8, INPUT);//select
  pinMode(9, INPUT);//start
  pinMode(10, INPUT);//A
  pinMode(15, INPUT);//B
  pinMode(16, INPUT);//C
  pinMode(14, INPUT);//D

  // Initialize Joystick Library
  Joystick.begin();
  Mouse.begin();
// open the serial port:

  //Serial.begin(9600);
  //Serial.println("Started.");
}


// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap[10] = {4,5,6,7,8,9,10,15,16,14} ;

// Last state of the button
int lastButtonState[10] = {0,0,0,0,0,0,0,0,0,0};
int mouseOnPin = 3;
int mStep = 5;
void loop() {

  int mouseState = digitalRead(mouseOnPin);

  bool anyPressed = false;
  // Read pin values
  for (int index = 0; index < 10; index++)
  {
    int currentButtonState = digitalRead(pinToButtonMap[index]);
    if (currentButtonState != lastButtonState[index])
    {
      if(mouseState == HIGH && currentButtonState == HIGH){
        //Serial.println("values:");
        //Serial.print(pinToButtonMap[index]);

        switch (pinToButtonMap[index]) {
					case 6:
						// move mouse up
						Mouse.move(0, -mStep);
						break;
					case 4:
						// move mouse down
						Mouse.move(0, mStep);
						break;
					case 5:
						// move mouse left
						Mouse.move(-mStep, 0);
						break;
					case 7:
						// move mouse right
						Mouse.move(mStep, 0);
						break;
					case 15:
						// perform mouse left click
						Mouse.click(MOUSE_LEFT);
						break;
					case 10:
						// perform mouse left click
						Mouse.click(MOUSE_RIGHT);
						break;
        }
      }else{
        Joystick.setButton(index, currentButtonState);
      }
      lastButtonState[index] = currentButtonState;
    }
    if(currentButtonState == HIGH) {
      anyPressed = true;
    }
  }

	if(anyPressed == true) {
		// turn LED on:
		digitalWrite(ledPin, HIGH);
	} else {
		// turn LED off:
		digitalWrite(ledPin, LOW);
	}
  //delay(50);
}
