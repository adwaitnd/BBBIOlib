#---------------------------------------------------
# Paths etc.
#---------------------------------------------------
LIB_PATH = ./BBBio_lib
INCLUDE_PATH = ./BBBio_lib
DEMO_PATH = ./Demo
TOOLKIT_PATH = ./Toolkit
LAB_PATH = ./Lab
MATLAB_PATH = ./matlab_lib/Prep
MATLAB_PATH2 = ./matlab_lib/Prep_fft


LIBRARY = BBBio

#---------------------------------------------------
# Compiler options
#---------------------------------------------------

# add location to CC_PREFIX if required
CC_PREFIX = 
CC = ${CC_PREFIX}gcc
AR = ${CC_PREFIX}ar
LD = ${CC_PREFIX}ld

# Display information
help : 
	@echo "BBBio Makefile help"
	@echo
	@echo "The library can be used the following way:"
	@echo "$ make <command>"
	@echo
	@echo "help, all or <blank>: show this prompt"
	@echo "install: create BBBio.a"
	@echo "clean: clean library (useful if you changed something)"
	@echo "demo name: compile a particular demo"

all: help

#---------------------------------------------------
# Compile Library
#---------------------------------------------------
${LIB_PATH}/libBBBio.a : ${LIB_PATH}/BBBiolib.c ${LIB_PATH}/BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	${CC} -c ${LIB_PATH}/BBBiolib.c -o ${LIB_PATH}/BBBiolib.o
	${AR} -rs ${LIB_PATH}/libBBBio.a ${LIB_PATH}/BBBiolib.o ${LIB_PATH}/BBBiolib_PWMSS.o ${LIB_PATH}/BBBiolib_McSPI.o ${LIB_PATH}/BBBiolib_ADCTSC.o
#	cp ${LIB_PATH}${LIB_PATH}/libBBBio.a ./

${MATLAB_PATH}/Prep.a : do_script
${MATLAB_PATH}/Prep_fft.a : do_script
do_script:
	cd ${MATLAB_PATH} && make -f Prep_rtw.mk
	cd ${MATLAB_PATH2} && make -f Prep_fft_rtw.mk

BBBiolib_PWMSS.o : ${LIB_PATH}/BBBiolib_PWMSS.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_PWMSS.c -o ${LIB_PATH}/BBBiolib_PWMSS.o -W 

BBBiolib_McSPI.o : ${LIB_PATH}/BBBiolib_McSPI.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_McSPI.c -o ${LIB_PATH}/BBBiolib_McSPI.o -W

BBBiolib_ADCTSC.o : ${LIB_PATH}/BBBiolib_ADCTSC.c ${LIB_PATH}/BBBiolib_ADCTSC.h
	${CC} -c ${LIB_PATH}/BBBiolib_ADCTSC.c -o ${LIB_PATH}/BBBiolib_ADCTSC.o -W


install : ${LIB_PATH}/libBBBio.a 
	

#---------------------------------------------------
# Compile Applications
#---------------------------------------------------

LED : ${DEMO_PATH}/Demo_LED/LED.c ${LIB_PATH}/libBBBio.a
	${CC} -o LED.out ${DEMO_PATH}/Demo_LED/LED.c -L ${LIB_PATH} -l${LIBRARY}

ADT7301 : ${DEMO_PATH}/Demo_ADT7301/ADT7301.c ${LIB_PATH}/libBBBio.a
	${CC} -o ADT7301.out ${DEMO_PATH}/Demo_ADT7301/ADT7301.c -L ${LIB_PATH} -l${LIBRARY}

ADXL345 :  ${DEMO_PATH}/Demo_ADXL345/ADXL345.c ${LIB_PATH}/libBBBio.a
	${CC} -o ADXL345.out ${DEMO_PATH}/Demo_ADXL345/ADXL345.c -L ${LIB_PATH} -l${LIBRARY}
ADXL345_NET :  ${DEMO_PATH}/Demo_ADXL345/ADXL345_net.c ${LIB_PATH}/libBBBio.a
	${CC} -o ADXL345.out ${DEMO_PATH}/Demo_ADXL345/ADXL345_net.c -L ${LIB_PATH} -l${LIBRARY}

L3G4200D : ${DEMO_PATH}/Demo_L3G4200D/L3G4200D.c ${LIB_PATH}/libBBBio.a
	${CC} -o L3G4200D.out ${DEMO_PATH}/Demo_L3G4200D/L3G4200D.c -L ${LIB_PATH} -l${LIBRARY}

SEVEN_SCAN : ${DEMO_PATH}/Demo_SevenScan/SevenScan.c ${LIB_PATH}/libBBBio.a
	${CC} -o SevenScan.out ${DEMO_PATH}/Demo_SevenScan/SevenScan.c  -L ${LIB_PATH} -l${LIBRARY}

SMOTOR : ${DEMO_PATH}/Demo_ServoMotor/ServoMotor.c ${LIB_PATH}/libBBBio.a
	${CC} -o SMOTOR.out ${DEMO_PATH}/Demo_ServoMotor/ServoMotor.c -L ${LIB_PATH} -l${LIBRARY}

LED_GPIO : ${DEMO_PATH}/Demo_LED_GPIO/LED_GPIO.c ${LIB_PATH}/libBBBio.a
	${CC} -o LED_GPIO.out ${DEMO_PATH}/Demo_LED_GPIO/LED_GPIO.c -L ${LIB_PATH} -l${LIBRARY} -pthread

DEBOUNCING : ${DEMO_PATH}/Demo_Debouncing/Debouncing.c ${LIB_PATH}/libBBBio.a
	${CC} -o Debouncing.out ${DEMO_PATH}/Demo_Debouncing/Debouncing.c -L ${LIB_PATH} -l${LIBRARY}

4x4keypad : ${DEMO_PATH}/Demo_4x4keypad/4x4keypad.c ${LIB_PATH}/libBBBio.a
	${CC} -o 4x4keypad.out ${DEMO_PATH}/Demo_4x4keypad/4x4keypad.c -L ${LIB_PATH} -l${LIBRARY}

PWM : ${DEMO_PATH}/Demo_PWM/PWM.c ${LIB_PATH}/libBBBio.a
	${CC} -o PWM.out ${DEMO_PATH}/Demo_PWM/PWM.c -L ${LIB_PATH} -l${LIBRARY}

BMOTOR : ${DEMO_PATH}/Demo_BrushlessMotor/BrushlessMotor.c ${LIB_PATH}/libBBBio.a
	 ${CC} -o BMOTOR.out ${DEMO_PATH}/Demo_BrushlessMotor/BrushlessMotor.c -L ${LIB_PATH} -l${LIBRARY}

ADC : ${DEMO_PATH}/Demo_ADC/ADC.c ${LIB_PATH}/libBBBio.a
	${CC} -o ADC.out ${DEMO_PATH}/Demo_ADC/ADC.c -L ${LIB_PATH} -l${LIBRARY} -lm

ADC_VOICE : ${DEMO_PATH}/Demo_ADC/ADC_voice.c ${LIB_PATH}/libBBBio.a
	${CC} -o ADC_VOICE.out ${DEMO_PATH}/Demo_ADC/ADC_voice.c -L ${LIB_PATH} -l${LIBRARY} -lm -pthread -O3


DAC_VOICE : ${DEMO_PATH}/Demo_DAC/DAC_voice.c ${LIB_PATH}/libBBBio.a
	${CC} -o DAC_VOICE.out ${DEMO_PATH}/Demo_DAC/DAC_voice.c -L ${LIB_PATH} -l${LIBRARY} -lm -pthread -O3

Sound_Capture_ADC : ${DEMO_PATH}/Demo_ADC/ADC.c ${LIB_PATH}/libBBBio.a
	${CC} -o Sound_Capture_ADC.out ${DEMO_PATH}/Demo_Sound_Capture_ADC/Capture.c -L ${LIB_PATH} -I ${INCLUDE_PATH} -l${LIBRARY} -lm

US : ${DEMO_PATH}/Demo_ADC/ADC.c ${LIB_PATH}/libBBBio.a ${MATLAB_PATH}/Prep.a 
	${CC} -o ${DEMO_PATH}/Demo_US/US.out ${DEMO_PATH}/Demo_US/US.c -L ${LIB_PATH} ${MATLAB_PATH}/Prep.a -I ${INCLUDE_PATH} -I ${MATLAB_PATH} -l${LIBRARY} -lm
	${CC} -o ${DEMO_PATH}/Demo_US/US_online.out ${DEMO_PATH}/Demo_US/US_online.c -L ${LIB_PATH} ${MATLAB_PATH2}/Prep_fft.a -I ${INCLUDE_PATH} -I ${MATLAB_PATH2} -l${LIBRARY} -lm
	${CC} -o ${DEMO_PATH}/Demo_US/Tone.out ${DEMO_PATH}/Demo_US/Tone.c -L ${LIB_PATH} ${MATLAB_PATH}/Prep.a -I ${INCLUDE_PATH} -I ${MATLAB_PATH} -I ${MATLAB_PATH2} -l${LIBRARY} -lm
#---------------------------------------------------
#---------------------------------------------------
# toolkit 
#---------------------------------------------------

GPIO_STATUS : ${TOOLKIT_PATH}Toolkit_GPIO_CLK_Status/GPIO_status.c ${LIB_PATH}/libBBBio.a
	${CC} -o GPIO_CLK_status ${TOOLKIT_PATH}Toolkit_GPIO_CLK_Status/GPIO_status.c  -L ${LIB_PATH} -l${LIBRARY}

EP_STATUS : ${TOOLKIT_PATH}Toolkit_EP_Status/EP_status.c ${LIB_PATH}/libBBBio.a
	${CC} -o EP_status ${TOOLKIT_PATH}Toolkit_EP_Status/EP_status.c -L ${LIB_PATH} -l${LIBRARY}


ADC_CALC : ${TOOLKIT_PATH}Toolkit_ADC_CALC/ADC_CALC.c
	${CC} -o ADC_CALC ${TOOLKIT_PATH}Toolkit_ADC_CALC/ADC_CALC.c


#---------------------------------------------------
# Lab
#---------------------------------------------------

RA : ${LAB_PATH}Lab_Robot_Arm/Robot_Arm.c ${LIB_PATH}/libBBBio.a
	${CC} -o RA  ${LAB_PATH}/Lab_Robot_Arm/Robot_Arm.c -L ${LIB_PATH} -l${LIBRARY} -lm

VD : ${LAB_PATH}Voice_Door/voice_door.cpp ${LIB_PATH}/libBBBio.a
	g++ -o VD ${LAB_PATH}/Voice_Door/voice_door.cpp -L ${LIB_PATH} -l${LIBRARY} -lfftw3 -lm -pthread -O3


.PHONY: clean
clean :
	rm -rf ${LIB_PATH}/*.o ${LIB_PATH}/libBBBio.a  ${MATLAB_PATH}/Prep.a ${MATLAB_PATH2}/Prep_fft.a *.out
