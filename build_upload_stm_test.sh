#export PATH=/home/adomas/bin:$PATH
#echo $PATH
#arduino-cli compile --fqbn STMicroelectronics:stm32:GenF1:upload_method=serial stm_test 
#arduino-cli upload -p /dev/ttyACM0 --fqbn STMicroelectronics:stm32:GenF1:upload_method=serial stm_test

arduino-cli compile --fqbn STMicroelectronics:stm32:GenF1:upload_method=serial stm_test
arduino-cli upload -p /dev/ttyACM0 --fqbn STMicroelectronics:stm32:GenF1:upload_method=serial stm_test


