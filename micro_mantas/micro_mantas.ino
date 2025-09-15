/*
// Define the pins for the sensors
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;
const int trigPin3 = 6;
const int echoPin3 = 7;
const int trigPin4 = 8;
const int echoPin4 = 9;
const int trigPin5 = 10;
const int echoPin5 = 16;

void setup() {
  // Start the Serial communication
  Serial.begin(9600);

  // Set the trigger pins as OUTPUT and echo pins as INPUT
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
}

void loop() {
  // Read distances from each sensor
  long distance1 = measureDistance(trigPin1, echoPin1);
  long distance2 = measureDistance(trigPin2, echoPin2);
  long distance3 = measureDistance(trigPin3, echoPin3);
  long distance4 = measureDistance(trigPin4, echoPin4);
  long distance5 = measureDistance(trigPin5, echoPin5);

  // Print the distances to the Serial Monitor
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  Serial.print("Distance 3: ");
  Serial.print(distance3);
  Serial.println(" cm");

  Serial.print("Distance 4: ");
  Serial.print(distance4);
  Serial.println(" cm");

  Serial.print("Distance 5: ");
  Serial.print(distance5);
  Serial.println(" cm");

  // Wait for a second before the next measurement
  delay(1000);
}

long measureDistance(int trigPin, int echoPin) {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  long distance = duration * 0.034 / 2; // Speed of sound is 0.034 cm/µs
  
  return distance;
}
*/
//*
// Define the pins for the sensors
const int trigPins[] = {2, 4, 6, 8, 10};
const int echoPins[] = {3, 5, 7, 9, 16};


// Timing variables
unsigned long previousMillis = 0;
const long interval = 10; // Interval for triggering the sensors
int currentSensor = 0;
long distances[5];
long lastDistances[5]; // Store the last known distances

void setup() {
  // Start the Serial communication
  Serial.begin(9600);

  // Initialize lastDistances to a known state
  for (int i = 0; i < 5; i++) {
    lastDistances[i] = -1; // Use -1 to indicate no previous measurement
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to trigger the next sensor
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Trigger the current sensor
    digitalWrite(trigPins[currentSensor], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[currentSensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[currentSensor], LOW);

    // Read the echo pin
    long duration = pulseIn(echoPins[currentSensor], HIGH, 20000); // Timeout after 30ms

    // Calculate the distance
    distances[currentSensor] = duration * 0.034 / 2; // Calculate distance

    // Only print if the distance has changed
    if (distances[currentSensor] != lastDistances[currentSensor]) {
      Serial.print("digital ");
      Serial.print(currentSensor + 1);
      Serial.print(" ");
      Serial.print(distances[currentSensor]);
      Serial.println("");
      lastDistances[currentSensor] = distances[currentSensor]; // Update last known distance
    }

    // Move to the next sensor
    currentSensor++;
    if (currentSensor >= 5) {
      currentSensor = 0; // Reset to the first sensor
    }
  }
}


//*/
