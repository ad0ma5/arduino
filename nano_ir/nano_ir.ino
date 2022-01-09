/* IR read codes     
*  by Hanie kiani     
*  https://electropeak.com/learn/        
*/     
#include <IRremote.h>  //including infrared remote header file     
int RECV_PIN = 7; // the pin where you connect the output pin of IR sensor
int motor1Pin1 = 6;
int motor1Pin2 = 5;
int motor2Pin1 = 4;
int motor2Pin2 = 3;

int ir1 = 26775;//left
int ir2 = 10455;//down
int ir3 = 30855;//right
int ir4 = 28815;//up
int ir5 = 18615;//stop

int ir6 = -3571;//2
int ir7 = 29325;//8
int ir8 = 23205;//4
int ir9 = 31365;//6
int ir10 = -4081;//5

int counter = 0; // delay counter
int currentAction = 0;
int stop = 0;

IRrecv irrecv(RECV_PIN);     
decode_results results;     

void setup()     
{     
pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
pinMode(motor2Pin1, OUTPUT);
pinMode(motor2Pin2, OUTPUT);
Serial.begin(9600);     
irrecv.enableIRIn();     

 Serial.print("START ");     
}
     
void loop()     
{     
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
  {     
    int value = results.value;// Results of decoding are stored in result.value     

    if(value == -1){
      value = currentAction;  
    }else{
      currentAction = value;
    }

    Serial.println(" ");     
    Serial.print("Code: [");     
    Serial.print(value); //prints the value a a button press     
    Serial.println("] "); 
    //Serial.print(stop); //prints the value a a button press     
    //Serial.println("] "); 
    //Serial.print(counter); //prints the value a a button press     
    //Serial.println("] "); 
    if(ir4 == value || ir6 == value){//up
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor2Pin2, HIGH);
    }else if(ir2 == value || ir7 == value){//down
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin1, HIGH);
    }else if(ir3 == value || ir9 == value){//right
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin2, HIGH);
    }else if(ir1 == value || ir8 == value){//left
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor2Pin1, HIGH);
    }else if(ir5 == value || ir10 == value){
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
   // }else if(value == -1){
    //}else if (currentAction == ) {
    //  value = 0;
    //  counter = 251;
    }

//*
if(
  //( 
  value == ir4
  ||
  value == ir2
  ||
  value == ir3
  ||
  value == ir1
/*
  value == 0
*/
  //)
//  &&
//  counter > 250
){ 
  stop = 1;  
  //delay(500);
//      digitalWrite(motor1Pin1, LOW);
//      digitalWrite(motor1Pin2, LOW);
//      digitalWrite(motor2Pin1, LOW);
//      digitalWrite(motor2Pin2, LOW);
  counter = 0;
}else{
  stop = 0;
}
//*/
    irrecv.resume(); // Restart the ISR state machine and Receive the next value     
  counter = 0;
  }else{
     if(stop == 1){
      counter++;
    Serial.println(counter );     
    //Serial.println( stop);     
     }
     if( counter > 25){
      stop = 0;
      counter = 0;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);

    Serial.println(" STOPPING ");     
    //Serial.print("Code: [");     
    //Serial.print(value); //prints the value a a button press     
    }else{
      //Serial.println(" GOING ");     

    }
  }//end else  
}
