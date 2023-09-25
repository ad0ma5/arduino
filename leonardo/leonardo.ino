#define COMPRESSOR 3
#define RECORDER 12
#define GUITAR 4
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(COMPRESSOR, OUTPUT);
  pinMode(RECORDER, OUTPUT);

	//Serial.begin(9600);
	Serial.begin(200000);
	while (!Serial){
				digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
				delay(500);
				digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
				delay(500);
	}
	Serial.println("on/off, gon/goff, rec/roff, com(1-3)/coff");
}
String word_of_law = "";
bool ser_read = false;
bool DEBUG = false;
// the loop function runs over and over again forever
void loop() {
  if(!ser_read && Serial.available() > 0){
		if(DEBUG) Serial.println("on/off, gon/goff, rec/roff, com(1-3)/coff");
		ser_read = true;
		String s = Serial.readStringUntil("\r");

		//int time = millis();
		//if(DEBUG)Serial.println(time);
		if(DEBUG)Serial.println("s="+s);
    s.trim();
		if(s.equals("ping")) word_of_law = "ping";
		if(s.equals("on")) word_of_law = "on";
		if(s.equals("off")) word_of_law = "off";
		if(s.equals("gon")) word_of_law = "gon";
		if(s.equals("goff")) word_of_law = "goff";
		if(s.equals("rec")) word_of_law = "rec";
		if(s.equals("roff")) word_of_law = "roff";
		if(s.equals("com3")) word_of_law = "com3";
		if(s.equals("com1")) word_of_law = "com1";
		if(s.equals("com2")) word_of_law = "com2";
		if(s.equals("coff")) word_of_law = "coff";
		if(s.equals("DEBUG")) DEBUG = true;
		if(s.equals("DEBUGOFF")) DEBUG = false;
		if(s.equals("exit")) word_of_law = "exit";

		if(DEBUG)Serial.println("w="+word_of_law);
		ser_read = false;
	}
  if(!word_of_law.equals("")){
 
		if(word_of_law.equals("ping")){
			Serial.println("pong");
		}
		if(word_of_law.equals("off")){
			digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
		}
		if(word_of_law.equals("goff")){
			digitalWrite(GUITAR, LOW);    // turn the LED off by making the voltage LOW
		}
		if(word_of_law.equals("roff")){
			digitalWrite(RECORDER, LOW);   // turn the LED on (HIGH is the voltage level)
		}
		if(word_of_law.equals("coff")){
			//digitalWrite(COMPRESSOR, LOW);   // turn the LED on (HIGH is the voltage level)
			digitalWrite(COMPRESSOR, LOW);   // turn the LED on (HIGH is the voltage level)
		}
		if(word_of_law.equals("on")){
			digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
			//delay(100);                       // wait for a second
			//delay(100);                       // wait for a second
		}
		if(word_of_law.equals("gon")){
			digitalWrite(GUITAR, HIGH);   // turn the LED on (HIGH is the voltage level)
			//delay(100);                       // wait for a second
			//delay(100);                       // wait for a second
		}
		if(word_of_law.equals("rec")){
			digitalWrite(RECORDER, HIGH);   // turn the LED on (HIGH is the voltage level)
		}
		if(word_of_law.equals("com3")){
			digitalWrite(COMPRESSOR, HIGH);   // turn the LED on (HIGH is the voltage level)
		}
		if(word_of_law.equals("com1")){
			analogWrite(COMPRESSOR, 64);   // turn the LED on (HIGH is the voltage level)
		}
		if(word_of_law.equals("com2")){
			analogWrite(COMPRESSOR, 128);   // turn the LED on (HIGH is the voltage level)
		}
    if(word_of_law.equals("exit")){
			Serial.end();
			Serial.begin(9600);
			while (!Serial){
				digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
				delay(500);
				digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
				delay(500);
			}
		}else{
			Serial.println("ok");
		}
			word_of_law = "";
	}
}

