#include "Arduino.h"
#include "Exaptation1.h"

Exaptation1::Exaptation1( int waterPin, int heatPwm, int ledPwm,
							int ledPinsArray[], int fanPins[], inputPins[])
{
	

	pinMode( waterPin, OUTPUT );
	_waterPin = waterPin;
	
	pinMode( heatPwm, OUTPUT );
	_heatPwm = heatPwm;
	
	pinMode( ledPwm, OUTPUT );
	_ledPwm = ledPwm;
	
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 2; j++ )
			pinMode( ledPinsArray[i][j], OUTPUT );
	}
	_ledPinsArray = ledPinsArray;
	
	for( int k = 0; k < 2; k++ )
		pinMode( fanPins[k], OUTPUT );
	_fanPins = fanPins;
	
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
	
	_muxAddr[10][3] = {
		{0, 0, 0},
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		{0, 0, 1},
		{2, 0, 0},
		{3, 0, 0},
		{2, 1, 0},
		{3, 1, 0},
		{2, 0, 1}
	};
	
	_fanCooldownDelay = secondsToMHz( 2 * 60 );
	_heatTimer = 0;
	_waterTimer = 0;
}

void Exaptation1::writeLEDMux( int channel, int value )
{
	for( int i = 1; i <= 2; i++ )
	{
		digitalWrite( _ledPinsArray[_muxAddr[channel][0]], _muxAddr[channel][i] );
	}
	
	analogWrite( _ledPwm, value );
}

bool Exaptation1::heaterOn( int value = 128, int speed = 128 )
{
	if( _heatStatus == 1 && _heatTimer < heatAutoShutdown )
	{
		analogWrite( _heatPwm, value );
		analogWrite( _fanPins[0], speed );
		digitalWrite( _fanPins[1], LOW );
		_heatTimer++;
		return true;
	}
	else if( _heatStatus == 1 && _heatTimer >= heatAutoShutdown  )
	{
		digitalWrite(_
	}
}

void Exaptation1::heaterOff()
{
	digitalWrite( _heatPwm, LOW );
	digitalWrite( _fanPins[0], LOW );
	digitalWrite( _fanPins[1], LOW );
}

void Exaptation1::ventilateOn( int source = 0, int speed = 128 )
{
	if( source == 0 )
	{
		digitalWrite( _fanPins[0], LOW );
		analogWrite( _fanPins[1], speed );
	}
	else
	{
		analogWrite( _fanPins[0], speed );
		digitalWrite( _fanPins[1], LOW );
	}
}

void Exaptation1::ventilateOff()
{
	digitalWrite( _fanPins[0], LOW );
	digitalWrite( _fanPins[1], LOW );
}

void Exaptation1::water()
{
	
}

int Exaptation1::getHeaterStatus()
{
	return _heaterStatus;
}

int Exaptation1::getVentilateStatus()
{
	return _ventilateStatus;
}

static int Exaptation1::secondsToMHz( int input )
{
	return input * 16000000;
}