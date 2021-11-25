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

int ir1 = 26775;
int ir2 = 10455;
int ir3 = 30855;
int ir4 = 28815;

IRrecv irrecv(RECV_PIN);     
decode_results results;     

void setup()     
{     
pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
Serial.begin(9600);     
irrecv.enableIRIn();     

 Serial.print("START ");     
}
     
void loop()     
{     
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
  {     
    int value = results.value;// Results of decoding are stored in result.value     
    Serial.println(" ");     
    Serial.print("Code: [");     
    Serial.print(value); //prints the value a a button press     
    Serial.println("] ");     
    if(ir1 == value){
      digitalWrite(motor1Pin1, HIGH);
    }
    else if(ir2 == value){
      digitalWrite(motor1Pin2, HIGH);
    }else if(ir3 == value){
      digitalWrite(motor2Pin1, HIGH);
    }else if(ir4 == value){
      digitalWrite(motor2Pin2, HIGH);
    }else{
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
    }
    delay(500);
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
    irrecv.resume(); // Restart the ISR state machine and Receive the next value     
  }  
}
