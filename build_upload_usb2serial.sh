arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old usbToSerial
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old usbToSerial 
