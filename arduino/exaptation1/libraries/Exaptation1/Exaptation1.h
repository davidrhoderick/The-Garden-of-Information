#ifndef Exaptation1_h
#define Exaptation1_h

#include "Arduino.h"
#include "Wire.h"

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

// The class Exaptation1 houses the code for the automatic plant growing environment controls
// and sensors methods.

class Exaptation1
{
	public:
		/*	waterPin is for activating the watering mechanism
		
			ledPwmPins is an array containing the pulse width modulation pins for each mux
		
			ledPinAddrs is an array containing the address pins for each mux
		
			fanPwmPins is an array containing the pulse width modulation pins for each fan
			
			moisturePins is an array containing the analog input pins for the moisture sensors
		*/
		Exaptation1( int waterPin, int ledPwmPins[4], int ledPinAddrs[6],
						int fanPwmPins[2], int moisturePins[3] );
						
		void receiveSerial();
		
		/*	channel		real		address
		---------------------------------------------
			0			Red 1		mux1 {0, 0, 0}	
			1			Red 2		mux2 {1, 0}
			2			Red 3		mux1 {0, 1, 0}
			3			Red 4		mux2 {1, 1}
			4			Red 5		mux1 {0, 0, 1}
			5			Blue 1		mux3 {2, 0, 0}
			6			Blue 2		mux4 {3, 0}
			7			Blue 3		mux3 {2, 1, 0}
			8			Blue 4		mux4 {3, 1}
			9			Blue 5		mux3 {2, 0, 1}
		
		These values are in MUX_ADDRS_ODD & MUX_ADDRS_EVEN as constants below the declarations.
		ODD is for the odd light arrays and EVEN is for the even light arrays
		*/
		
		// value is 0-255 and is written through the ledPwm pin.
		int writeLightChannel( int channel, int value );
		
		void ventilateOn( int source, int speed );	// TODO 0 is outdoors, 1 is indoors
		void ventilateOff();						// TODO
		void water();								// TODO
		
		void setupLightSensor();
		
		float getMoisture();						// TODO
		
		unsigned int readRedChannel();
		unsigned int readBlueChannel();
		
		int getVentilateStatus();					// TODO
		
	private:
		// initial values for integration time registers
		unsigned char colorCap[4];  // values must be between 0 and 15
		unsigned int colorInt[4];  // max value for these is 4095
		unsigned int colorData[4];  // This is where we store the RGB and C data values
		signed char colorOffset[4];  // Stores RGB and C offset values
		
		void initADJD_S311();
		
		int calibrateClear();
		int calibrateColor();
		void calibrateCapacitors();
		void writeInt( int address, int gain );
		void performMeasurement();
		void getOffset();
		void writeRegister( unsigned char data, unsigned char address );
		unsigned char readRegister( unsigned char address );
		int readRegisterInt( unsigned char address );
		void getRGBC();
		
		int _waterPin;
		int _waterTimer;
		int _ventilateStatus;			// 0 is off, 1 is on.  Both statuses can't be 1 at once
		int _ledPwmPins[4];
		int _ledPinAddrs[6];
		// _fanPwmPins is a one dimensional array where [0] is the fan from indoors but
		// outside the box, over the heater; and [1] is the fan from the outdoors.
		int _fanPwmPins[2];

		int _moistAn;
		
		void calibrateLightSensor();
};

const int MUX_ADDRS_ODD[6][2] = {
			{0, 0},
			{1, 0},
			{0, 1},
			{0, 0},
			{1, 0},
			{0, 1}
		};
const int MUX_ADDRS_EVEN[4] = { 0, 1, 0, 1 };
		
// 16000000 is for the speed of the processor in Hz that is multiplied by 60 seconds and then
// by the amount of minutes for the duration.
const int WATER_DURATION = 16000000 * 2;

#endif