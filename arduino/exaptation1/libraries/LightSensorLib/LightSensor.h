#ifndef LightSensor_h
#define LightSensor_h

#include "Arduino.h"
#include "Wire.h"  // We use Wire.h to talk I2C to the sensor

// ADJD-S311's I2C address, don't change
#define ADJD_S311_ADDRESS 0x74

#define RED 0
#define GREEN 1
#define BLUE 2
#define CLEAR 3

// ADJD-S311's register list
#define CTRL 0x00
#define CONFIG 0x01
#define CAP_RED 0x06
#define CAP_GREEN 0x07
#define CAP_BLUE 0x08
#define CAP_CLEAR 0x09
#define INT_RED_LO 0xA
#define INT_RED_HI 0xB
#define INT_GREEN_LO 0xC
#define INT_GREEN_HI 0xD
#define INT_BLUE_LO 0xE
#define INT_BLUE_HI 0xF
#define INT_CLEAR_LO 0x10
#define INT_CLEAR_HI 0x11
#define DATA_RED_LO 0x40
#define DATA_RED_HI 0x41
#define DATA_GREEN_LO 0x42
#define DATA_GREEN_HI 0x43
#define DATA_BLUE_LO 0x44
#define DATA_BLUE_HI 0x45
#define DATA_CLEAR_LO 0x46
#define DATA_CLEAR_HI 0x47
#define OFFSET_RED 0x48
#define OFFSET_GREEN 0x49
#define OFFSET_BLUE 0x4A
#define OFFSET_CLEAR 0x4B

class LightSensor
{
	public:
		// Pin definitions:
		int sdaPin;  // serial data, hardwired, can't change
		int sclPin;  // serial clock, hardwired, can't change
		int ledPin;  // LED light source pin, any unused pin will work

		// RGB LED pins, should all be PWM output pins:
		int redledPin;
		int greenledPin;
		int blueledPin;
		int rgbPins[3];

		// initial values for integration time registers
		unsigned char colorCap[4];  // values must be between 0 and 15
		unsigned int colorInt[4];  // max value for these is 4095
		unsigned int colorData[4];  // This is where we store the RGB and C data values
		signed char colorOffset[4];  // Stores RGB and C offset values
		
		LightSensor();
		void printADJD_S311Values();
		void initADJD_S311();
		int calibrateClear();
		int calibrateColor();
		void calibrateCapacitors();
		void writeInt(int address, int gain);
		void performMeasurement();
		void getRGBC();
		void getOffset();
		void writeRegister(unsigned char data, unsigned char address);
		unsigned char readRegister(unsigned char address);
		int readRegisterInt(unsigned char address);
};

#endif