export PATH=/home/adomas/bin:$PATH
echo $PATH
arduino-cli compile --fqbn arduino:avr:leonardo leo_midi 
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:leonardo leo_midi
