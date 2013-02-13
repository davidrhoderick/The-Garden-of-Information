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
		
		//Serial.print( "LED PWM pin " );
		Serial.println( _ledPwmPins[i] );
		//Serial.println( " ready for output" );
		
		for( int j = 0; j < 2; j++ )
		{
			pinMode( ledPinAddrs[i][j], OUTPUT );
			_ledPinAddrs[i][j] = ledPinAddrs[i][j];
			
			//Serial.print( "LED address pin " );
			Serial.println( _ledPinAddrs[i][j] );
			//Serial.println( " ready for output" );
		}
	}
	
	for( int k = 0; k < 2; k++ )
	{
		pinMode( fanPwmPins[k], OUTPUT );
		_fanPwmPins[k] = fanPwmPins[k];
	}
	
	pinMode( inputPins[0], OUTPUT );
	_moistAn= inputPins[0];
	
	pinMode( inputPins[1], OUTPUT );
	_tempSCL = inputPins[1];
	
	pinMode( inputPins[2], OUTPUT );
	_tempSDA = inputPins[2];
	
	pinMode( inputPins[3], OUTPUT );
	_lightSCL = inputPins[3];
	
	pinMode( inputPins[4], OUTPUT );
	_lightSDA = inputPins[4];
		
	_heatTimer = 0;
	_waterTimer = 0;
	
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
	for( int i = 0; i < 2; i++ )
	{
		digitalWrite( _ledPinAddrs[MUX_ADDRS[channel][0]][i], MUX_ADDRS[channel][i+1] );
		Serial.print( _ledPinAddrs[MUX_ADDRS[channel][0]][i] );
		Serial.print( "\t" );
		Serial.println(MUX_ADDRS[channel][i+1]);
	}
	
	analogWrite( _ledPwmPins[MUX_ADDRS[channel][0]], value );
}

float Exaptation1::readTemperature()
{
	int temperature = 0;
	// step 1: request reading from sensor 
	Wire.requestFrom(TEMPERATURE_SENSOR_ADDRESS, 2); 

	if (2 <= Wire.available())  // if two bytes were received 
	{
		_tempMsb = Wire.read();  // receive high byte (full degrees)
		_tempLsb = Wire.read();  // receive low byte (fraction degrees) 
		temperature = ((_tempMsb) << 4);  // MSB
		temperature |= (_tempLsb >> 4);   // LSB
	}
	return temperature * 0.0625;
}

bool Exaptation1::heaterOn( int value = 128, int speed = 128 )
{
	if( _heatStatus == 1 && _heatTimer < HEAT_AUTO_SHUTDOWN )
	{
		analogWrite( _heatPwm, value );
		analogWrite( _fanPwmPins[0], speed );
		digitalWrite( _fanPwmPins[1], LOW );
		_heatTimer++;
		return true;
	}
	/*else if( _heatStatus == 1 && _heatTimer >= HEAT_AUTO_SHUTDOWN  )
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

void Exaptation1::printLightChannel( int channel )
{
	for(int i=0; i<3; i++)
		Serial.println( MUX_ADDRS[channel][i] );
}