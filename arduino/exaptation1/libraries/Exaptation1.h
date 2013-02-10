#ifndef Exaptation1_h
#define Exaptation1_h

#include "Arduino.h"

// The class Exaptation1 houses the code for the automatic plant growing environment controls
// and sensors methods.

class Exaptation1
{
	public:
		// pwmPin is for the pulse-width modulated output pin that is multiplexed
		
		// ledPinAddrs is a two dimensional array that contains the pins that control the
		// multiplexer.  The outer dimension is the address of the
		// multiplexer [4] and the inner dimension is for the pin on that multiplexer [2].
		
		// fanPwmPins is a one dimensional array of length 2.  It has the address of the heat and
		// cool fans respectively.
		
		// inputPins is a one dimensional array.  It has the address of the following pins:
		// analog moisture input, scl for temperature input, sda for temperature input,
		// scl for light sensor, scl for temperature input.
		Exaptation1( int waterPin, int heatPwm, int ledPwmPins[4],
						int ledPinAddrs[4][2], int fanPwmPins[2], int inputPins[5] );
		
		// channel		real		address
		//------------------------------------------
		//	0			Red 1		mux1 {0, 0, 0}	
		//	1			Red 2		mux2 {1, 0, 0}
		//	2			Red 3		mux1 {0, 1, 0}
		//	3			Red 4		mux2 {1, 1, 0}
		//	4			Red 5		mux1 {0, 0, 1}
		//	5			Blue 1		mux3 {2, 0, 0}
		//	6			Blue 2		mux4 {3, 0, 0}
		//	7			Blue 3		mux3 {2, 1, 0}
		//	8			Blue 4		mux4 {3, 1, 0}
		//	9			Blue 5		mux3 {2, 0, 1}
		// I make constants for channels in the implementing program for recognizable names.
		// Change the addresses in _muxAddr[].
		// value is 0-255 and is written through the ledPwm pin.
		void writeLightChannel( int channel, int value );
		// heat() returns false if the heatAutoShutdown timer is exceeded and shuts down the
		// the heater for heatShutdownDuration milliseconds.
		// value is 0-255 through the heatPwm pin.
		// speed is 0-255 for the fanPwmPins[]
		bool heaterOn( int value, int speed );
		void heaterOff();
		void ventilateOn( int source, int speed );	// 0 is outdoors, 1 is indoors
		void ventilateOff();
		void water();
		
		int heatAutoShutdown;		// shutdown timer for the heater
		int heatShutdownDuration;	// the duration of the heater shutdown
		int waterDuration;			// how long the water solenoid is open
		
		float getMoisture();
		float getTemperature();
		float getLight();
		
		int getHeaterStatus();
		int getVentilateStatus();
		
	private:
		int _waterPin;
		int _waterTimer;
		int _heatPwm;
		int _heatTimer;
		int _heatStatus;				// 0 is off, 1 is on, -1 is cool-down
		int _ventilateStatus;			// 0 is off, 1 is on.  Both statuses can't be 1 at once
		int _ledPwmPins[4];
		int _ledPinAddrs[4][2];
		int _MUX_ADDRS[10][3];
		// _fanPwmPins is a one dimensional array where [0] is the fan from indoors but
		// outside the box, over the heater; and [1] is the fan from the outdoors.
		int _fanPwmPins[2];
		int _FAN_COOLDOWN_DURATION;
		
		int _moistAn;
		int _tempSCL;
		int _tempSDA;
		int _lightSCL;
		int _lightSDA;
		
		static int secondsToMHz( int input );
};

#endif