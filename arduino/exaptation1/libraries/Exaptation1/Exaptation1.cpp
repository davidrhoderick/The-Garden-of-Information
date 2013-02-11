#include "Arduino.h"
#include "Wire.h"
#include "Exaptation1.h"
#include "LightSensor.h"

Exaptation1::Exaptation1( int waterPin, int heatPwm, int ledPwmPins[4],
							int ledPinAddrs[4][2], int fanPwmPins[2], int inputPins[5] )
{
	LightSensor lightInput();
	pinMode( waterPin, OUTPUT );
	_waterPin = waterPin;
	
	pinMode( heatPwm, OUTPUT );
	_heatPwm = heatPwm;
	
	for( int i = 0; i < 4; i++ )
	{
		pinMode( ledPwmPins[i], OUTPUT );
		_ledPwmPins[i] = ledPwmPins[i];
		
		for( int j = 0; j < 2; j++ )
		{
			pinMode( ledPinAddrs[i][j], OUTPUT );
			_ledPinAddrs[i][j] = ledPinAddrs[i][j];
		}
	}
	
	for( int k = 0; k < 2; k++ )
	{
		pinMode( fanPwmPins[k], OUTPUT );
		_fanPwmPins[k] = fanPwmPins[k];
	}
	
	pinMode( inputPins[0], OUTPUT );
	_moistAn= inputPins[0];
	
	_tempSensAddr = 0x91 >> 1;
	
	pinMode( inputPins[1], OUTPUT );
	_tempSCL = inputPins[1];
	
	pinMode( inputPins[2], OUTPUT );
	_tempSDA = inputPins[2];
	
	pinMode( inputPins[3], OUTPUT );
	_lightSCL = inputPins[3];
	
	pinMode( inputPins[4], OUTPUT );
	_lightSDA = inputPins[4];
	
	_MUX_ADDRS[10][3] = (
			(0, 0, 0),
			(1, 0, 0),
			(0, 1, 0),
			(1, 1, 0),
			(0, 0, 1),
			(2, 0, 0),
			(3, 0, 0),
			(2, 1, 0),
			(3, 1, 0),
			(2, 0, 1)
		);
	
	_fanCooldownDuration = secondsToMHz( 2 * 60 );
	_heatTimer = 0;
	_waterTimer = 0;
	heatAutoShutdown = secondsToMHz( 2 * 60 );
	heatShutdownDuration = secondsToMHz( 2 * 60 );
	
	_heatStatus = 0;
	_ventilateStatus = 0;
}

bool Exaptation1::setupLightSensor()
{
	Wire.begin();
	delay(1);  // Wait for ADJD reset sequence
	lightInput.initADJD_S311();  // Initialize the ADJD-S311, sets up cap and int registers

	/* First we'll see the initial values
	getRGBC();  // Call this to put new RGB and C values into the colorData array
	printADJD_S311Values();  // Formats and prints all important registers of ADJD-S311
	*/
	Serial.println("\nCalibrating...this may take a moment\n");
	lightInput.calibrateColor();  // This calibrates R, G, and B int registers
	lightInput.calibrateClear();  // This calibrates the C int registers
	lightInput.calibrateCapacitors();  // This calibrates the RGB, and C cap registers
	lightInput.getRGBC();  // After calibrating, we can get the first RGB and C data readings
	
	lightInput.printADJD_S311Values();  // Formats and prints all important ADJD-S311 registers
	
	return true;
}

void Exaptation1::calibrateLightSensor()
{
	Serial.println("\nCalibrating...\n");
	lightInput.calibrateColor();
	lightInput.calibrateClear();
	lightInput.calibrateCapacitors();
	lightInput.getRGBC();
	lightInput.printADJD_S311Values();
}

void Exaptation1::readLightChannel()
{
	while(!Serial.available())
	;  // Wait till something's pressed
	char inKey = Serial.read();

	lightInput.getRGBC();
	lightInput.printADJD_S311Values();
}

void Exaptation1::writeLightChannel( int channel, int value )
{
	for( int i = 1; i <= 2; i++ )
	{
		digitalWrite( _ledPinAddrs[_MUX_ADDRS[channel][0]][i], _MUX_ADDRS[channel][i+1] );
	}
	
	analogWrite( _ledPwmPins[_MUX_ADDRS[channel][0]], value );
}

float Exaptation1::readTemperature()
{
	int temperature = 0;
	// step 1: request reading from sensor 
	Wire.requestFrom(_tempSensAddr, 2); 

	if (2 <= Wire.available())  // if two bytes were received 
	{
		_tempMsb = Wire.read();  // receive high byte (full degrees)
		_tempLsb = Wire.read();  // receive low byte (fraction degrees) 
		temperature = ((_tempMsb) << 4);  // MSB
		temperature |= (_tempLsb >> 4);   // LSB
		temperature *= 0.0625;
	}
	return temperature;
}

bool Exaptation1::heaterOn( int value = 128, int speed = 128 )
{
	if( _heatStatus == 1 && _heatTimer < heatAutoShutdown )
	{
		analogWrite( _heatPwm, value );
		analogWrite( _fanPwmPins[0], speed );
		digitalWrite( _fanPwmPins[1], LOW );
		_heatTimer++;
		return true;
	}
	/*else if( _heatStatus == 1 && _heatTimer >= heatAutoShutdown  )
	{
		digitalWrite(_
	}*/
}

void Exaptation1::heaterOff()
{
	digitalWrite( _heatPwm, LOW );
	digitalWrite( _fanPwmPins[0], LOW );
	digitalWrite( _fanPwmPins[1], LOW );
}

void Exaptation1::ventilateOn( int source = 0, int speed = 128 )
{
	if( source == 0 )
	{
		digitalWrite( _fanPwmPins[0], LOW );
		analogWrite( _fanPwmPins[1], speed );
	}
	else
	{
		analogWrite( _fanPwmPins[0], speed );
		digitalWrite( _fanPwmPins[1], LOW );
	}
}

void Exaptation1::ventilateOff()
{
	digitalWrite( _fanPwmPins[0], LOW );
	digitalWrite( _fanPwmPins[1], LOW );
}

void Exaptation1::water()
{
	
}

int Exaptation1::getHeaterStatus()
{
	return _heatStatus;
}

int Exaptation1::getVentilateStatus()
{
	return _ventilateStatus;
}

int Exaptation1::secondsToMHz( int input )
{
	return input * 16000000;
}