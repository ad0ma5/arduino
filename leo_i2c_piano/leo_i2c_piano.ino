#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEBUG 1

#if DEBUG
  #define DBG(...) Serial.print(__VA_ARGS__)
  #define DBGLN(...) Serial.println(__VA_ARGS__)
#else
  #define DBG(...)
  #define DBGLN(...)
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

const byte NANO_ADDR = 0x10;

uint16_t receivedValue = 0;

void setup()
{
#if DEBUG
  Serial.begin(115200);
#endif

  Wire.begin();

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();

  DBGLN("Leonardo Master Ready");
}

void loop()
{
  Wire.requestFrom(NANO_ADDR, sizeof(receivedValue));

  if (Wire.available() >= sizeof(receivedValue))
  {
    Wire.readBytes(
      (char*)&receivedValue,
      sizeof(receivedValue)
    );

    DBG("Received: ");
    DBGLN(receivedValue);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(receivedValue);
    display.display();

    //sendMidiCC(receivedValue);
  }

  delay(20);
}
