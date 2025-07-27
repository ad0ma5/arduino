// In this section, you declare your constants:
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6;
const int buttonPin7 = 7;
const int buttonPin8 = 8;
const int buttonPin9 = 9;
const int buttonPin10 = 10;
const int buttonPin11 = 11;
const int buttonPin12 = 12;

// In this section, you declare your variables:
int buttonState2 = 0; // variable for reading the pushbutton status
int buttonState3 = 0; // variable for reading the pushbutton status
int buttonState4 = 0; // variable for reading the pushbutton status
int buttonState5 = 0; // variable for reading the pushbutton status
int buttonState6 = 0; // variable for reading the pushbutton status
int buttonState7 = 0; // variable for reading the pushbutton status
int buttonState8 = 0; // variable for reading the pushbutton status
int buttonState9 = 0; // variable for reading the pushbutton status
int buttonState10 = 0; // variable for reading the pushbutton status
int buttonState11 = 0; // variable for reading the pushbutton status
int buttonState12 = 0; // variable for reading the pushbutton status

int old_buttonState2 = 0; // variable for reading the pushbutton status
int old_buttonState3 = 0; // variable for reading the pushbutton status
int old_buttonState4 = 0; // variable for reading the pushbutton status
int old_buttonState5 = 0; // variable for reading the pushbutton status
int old_buttonState6 = 0; // variable for reading the pushbutton status
int old_buttonState7 = 0; // variable for reading the pushbutton status
int old_buttonState8 = 0; // variable for reading the pushbutton status
int old_buttonState9 = 0; // variable for reading the pushbutton status
int old_buttonState10 = 0; // variable for reading the pushbutton status
int old_buttonState11 = 0; // variable for reading the pushbutton status
int old_buttonState12 = 0; // variable for reading the pushbutton status
                           //
void setup() { // put your setup code here, to run once:

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin8, INPUT_PULLUP);
  pinMode(buttonPin9, INPUT_PULLUP);
  pinMode(buttonPin10, INPUT_PULLUP);
  pinMode(buttonPin11, INPUT_PULLUP);
  pinMode(buttonPin12, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  // Open the serial port:
  Serial.begin(9600);
}

void loop() { // put your main code here, to run repeatedly:

  // read the state of the pushbutton value:
  buttonState2 = digitalRead(buttonPin2 );
  buttonState3 = digitalRead(buttonPin3 );
  buttonState4 = digitalRead(buttonPin4 );
  buttonState5 = digitalRead(buttonPin5 );
  buttonState6 = digitalRead(buttonPin6 );
  buttonState7 = digitalRead(buttonPin7 );
  buttonState8 = digitalRead(buttonPin8 );
  buttonState9 = digitalRead(buttonPin9 );
  buttonState10= digitalRead(buttonPin10);
  buttonState11= digitalRead(buttonPin11);
  buttonState12= digitalRead(buttonPin12);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

  if( old_buttonState2 != buttonState2){
    old_buttonState2 = buttonState2;
    if (buttonState2 == HIGH) {
      Serial.println("digital 2 1");
      digitalWrite(LED_BUILTIN, HIGH);
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("digital 2 0");
    }
  }

  if( old_buttonState3 != buttonState3){
    old_buttonState3 = buttonState3;
    if (buttonState3 == HIGH) {
      Serial.println("digital 3 1");
    }else{
      Serial.println("digital 3 0");
    }
  }

  if( old_buttonState4 != buttonState4){
    old_buttonState4 = buttonState4;
    if (buttonState4 == HIGH) {
      Serial.println("digital 4 1");
    }else{
      Serial.println("digital 4 0");
    }
  }

  if( old_buttonState5 != buttonState5){
    old_buttonState5 = buttonState5;
    if (buttonState5 == HIGH) {
      Serial.println("digital 5 1");
    }else{
      Serial.println("digital 5 0");
    }
  }

  if( old_buttonState6 != buttonState6){
    old_buttonState6 = buttonState6;
    if (buttonState6 == HIGH) {
      Serial.println("digital 6 1");
    }else{
      Serial.println("digital 6 0");
    }
  }

  if( old_buttonState7 != buttonState7){
    old_buttonState7 = buttonState7;
    if (buttonState7 == HIGH) {
      Serial.println("digital 7 1");
    }else{
      Serial.println("digital 7 0");
    }
  }

  if( old_buttonState8 != buttonState8){
    old_buttonState8 = buttonState8;
    if (buttonState8 == HIGH) {
      Serial.println("digital 8 1");
    }else{
      Serial.println("digital 8 0");
    }
  }

  if( old_buttonState9 != buttonState9){
    old_buttonState9 = buttonState9;
    if (buttonState9 == HIGH) {
      Serial.println("digital 9 1");
    }else{
      Serial.println("digital 9 0");
    }
  }

  if( old_buttonState10 != buttonState10){
    old_buttonState10 = buttonState10;
    if (buttonState10 == HIGH) {
      Serial.println("digital 10 1");
    }else{
      Serial.println("digital 10 0");
    }
  }

  if( old_buttonState11 != buttonState11){
    old_buttonState11 = buttonState11;
    if (buttonState11 == HIGH) {
      Serial.println("digital 11 1");
    }else{
      Serial.println("digital 11 0");
    }
  }

  if( old_buttonState12 != buttonState12){
    old_buttonState12 = buttonState12;
    if (buttonState12 == HIGH) {
      Serial.println("digital 12 1");
    }else{
      Serial.println("digital 12 0");
    }
  }


}


