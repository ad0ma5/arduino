/*
 * Show two values => LED1 on => wait for S1 pressed => sound => LED off => repeat
 */

#include <MultiFunctionShield.h>

MultiFunctionShield MFS;

int PotValue;
int lastPotValue = -1;
int freq = 0;
  uint8_t i = 0;


void setup ()
{
  MFS.begin();
  pinMode (BUTTON_1_PIN, INPUT);    // insert J2 on board for pull-ups!
  pinMode (BUTTON_2_PIN, INPUT);    // insert J2 on board for pull-ups!
  pinMode (BUTTON_3_PIN, INPUT);    // insert J2 on board for pull-ups!

  pinMode (LED_1_PIN, OUTPUT);  
  pinMode (LED_2_PIN, OUTPUT);  
  pinMode (LED_3_PIN, OUTPUT);  
  pinMode (LED_4_PIN, OUTPUT);  

  pinMode (BUZZER_PIN, OUTPUT);  
  pinMode (5, OUTPUT);  
  //tone(BUZZER_PIN, 20);
  delay(50);
  //noTone(BUZZER_PIN);
  delay(1000);
}
 
void loop()
{
     // noTone(3);
        //noTone(BUZZER_PIN);
        //digitalWrite(BUZZER_PIN, LOW);
        //tone(BUZZER_PIN, 0, 500);
  
                                    delay(100);
                                    //
                                    //
  PotValue = analogRead(POT_PIN);
  // Map to audible frequency (say 100 Hz – 2000 Hz)
  freq = map(PotValue, 0, 1023, 20, 800);
  
    MFS.Display (PotValue);
    //delay (500);
    
    int b1 = analogRead(BUTTON_1_PIN);     // wait until S1 (low active)
    int b2 = analogRead(BUTTON_2_PIN);     // wait until S1 (low active)
    int b3 = analogRead(BUTTON_3_PIN);     // wait until S1 (low active)
                                          //
    if( b1 == 0 ){ 
      digitalWrite(LED_4_PIN, LOW);
      freq = freq / 2;
    }else 
      digitalWrite(LED_4_PIN, HIGH);
    
    if( b2 == 0 ) 
      digitalWrite(LED_3_PIN, LOW);
    else 
      digitalWrite(LED_3_PIN, HIGH);
    
    if( b3 == 0 ){ 
      digitalWrite(LED_2_PIN, LOW);
      freq = freq * 2;
    }else 
      digitalWrite(LED_2_PIN, HIGH);
    
    //if( b1 > 0 || b2>0 || b3>0){
    if( b1 == 0 || b2==0 || b3==0){
      //tone(3, freq);
      tone(5, freq);
      digitalWrite(LED_1_PIN, HIGH);    // off

    }else
    //for (i = 0; i < 10; i++)    
    {
      digitalWrite(LED_1_PIN, LOW);   // ON 
        //noTone(3);
        noTone(5);
        //freq += freq+(i*1);
        //tone(BUZZER_PIN, 0);
        //tone(3, 0);
        //tone(5, 0);
      //digitalWrite(BUZZER_PIN, LOW);
      //delay (50);
      //digitalWrite(BUZZER_PIN, HIGH);
     // digitalWrite(3, HIGH);
      //delay (10);
    }
      /*                              delay(100);
      tone(3, 40);
      delay(50);
        tone(3, 0);
      delay(50);
    */    
    //tone(BUZZER_PIN, 0, 50);
        //noTone(BUZZER_PIN);
    //digitalWrite(LED_1_PIN, HIGH);    
}
