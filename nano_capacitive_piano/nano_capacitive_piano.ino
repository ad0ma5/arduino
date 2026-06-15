#include <Wire.h>
#include <CapacitiveSensor.h>

#define I2C_ADDRESS 0x10

struct ControllerData
{
  uint16_t pots[4];
  uint16_t touchBits;
};

volatile ControllerData controllerData;

const int sendPin = A1;
// Pot pins
const uint8_t potPins[4] =
{
A7, A6, A0, A2
};

const int Ncaps = 13; //*** total number of capacitive touchers
const int capPin[Ncaps] = {
  1,2,0,3,4,5,6,7,8,9,10,11,12
}; 
// Touch sensors
//CapacitiveSensor capSensors[Ncaps] = {};
/*
{
  CapacitiveSensor(sendPin, capPin[0]),
  CapacitiveSensor(sendPin, capPin[1]),
  CapacitiveSensor(sendPin, capPin[2]),
  CapacitiveSensor(sendPin, capPin[3]),
  CapacitiveSensor(sendPin, capPin[4]),
  CapacitiveSensor(sendPin, capPin[5]),
  CapacitiveSensor(sendPin, capPin[6]),
  CapacitiveSensor(sendPin, capPin[7]),
  CapacitiveSensor(sendPin, capPin[8]),
  CapacitiveSensor(sendPin, capPin[9]),
  CapacitiveSensor(sendPin, capPin[10]),
  CapacitiveSensor(sendPin, capPin[11]),
  CapacitiveSensor(sendPin, capPin[12])
};
*/
long baseline[Ncaps];
long threshold[Ncaps];

void requestEvent()
{
  Wire.write(
    (uint8_t *)&controllerData,
    sizeof(controllerData)
  );
}

void setup()
{
  //pinMode(13, OUTPUT);
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(requestEvent);

  memset((void *)&controllerData, 0, sizeof(controllerData));
/*
  // Disable auto calibration
  for (int i = 0; i < Ncaps; i++) {
    capSensors[i].set_CS_AutocaL_Millis(0xFFFFFFFF);  //Calibrate the sensor...  
  }

  delay(1000);

  // Initial calibration
  for(int i = 0; i < Ncaps; i++)
  {
    long sum = 0;

    for(int j = 0; j < 10; j++)
    {
      sum += capSensors[i].capacitiveSensor(5);
      delay(5);
    }

    baseline[i] = sum / 10;

    // Adjust if needed
    threshold[i] = baseline[i] + 80;
  }
*/
  // no more caps
  for(int pin=0; pin<=12; pin++)
  {
      pinMode(pin, INPUT_PULLUP);
  }

  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
}

void loop()
{
  //digitalWrite(13, HIGH);
  readPots();
  //readTouches();
  readGroundTouches();
  //digitalWrite(13, LOW);
  //delay(50);
}

void readPots()
{
  for(int i = 0; i < 4; i++)
  {
    uint16_t v = analogRead(potPins[i]);

    // Simple jitter suppression
    if(abs((int)v - (int)controllerData.pots[i]) > 2)
    {
      controllerData.pots[i] = v;
    }
  }
}
void readGroundTouches()
{
    uint16_t bits = 0;

    // D0-D7 -> bits 0-7
    bits |= (~PIND) & 0xFF;

    // D8-D12 -> bits 8-12
    bits |= (((~PINB) & 0x1F) << 8);

    controllerData.touchBits = bits;
}
/*
void readTouches()
{
  uint16_t bits = 0;

  for(int i = 0; i < Ncaps; i++)
  {
    long value = capSensors[i].capacitiveSensorRaw(1);//capacitiveSensor(5);


    if(value > threshold[i])
    {
      bits |= (1 << i);
    }
  }

  controllerData.touchBits = bits;
}
*/
/////////////////////////
/*
#include <Wire.h>

#define DEBUG 0

const byte I2C_ADDR = 0x10;

struct ControllerData
{
  uint16_t pots[4];
  uint16_t touchBits;
  uint8_t changed;
};

volatile ControllerData data;

const int potPin[NPots] = {A7, A6, A0, A2};                                            
const int capPin[Ncaps] = {
  1,2,0,3,4,5,6,7,8,9,10,11,12
}; 

#if DEBUG
#define DBG(x) Serial.print(x)
#define DBGLN(x) Serial.println(x)
#else
#define DBG(x)
#define DBGLN(x)
#endif

void requestEvent()
{
  Wire.write((uint8_t*)&data, sizeof(data));

  data.changed = 0;
}

void setup()
{
#if DEBUG
  Serial.begin(115200);
#endif

  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent);

  memset((void*)&data, 0, sizeof(data));
}
void loop()
{
  bool anyChange = false;

  // Read pots
  for(int i=0;i<4;i++)
  {
    uint16_t newValue = analogRead(potPin[i]);

    // Ignore tiny jitter
    if(abs((int)newValue - (int)data.pots[i]) > 3)
    {
      data.pots[i] = newValue;
      anyChange = true;
    }
  }

  // Example touch reads
  uint16_t newTouchBits = 0;

  for(int i=0;i<12;i++)
  {
    bool touched = digitalRead(capPin[i]);

    if(touched)
      newTouchBits |= (1 << i);
  }

  if(newTouchBits != data.touchBits)
  {
    data.touchBits = newTouchBits;
    anyChange = true;
  }

  if(anyChange)
  {
    data.changed = 1;

#if DEBUG
    DBG("Touch bits: ");
    DBGLN(data.touchBits);
#endif
  }

  delay(5);
}
*/
