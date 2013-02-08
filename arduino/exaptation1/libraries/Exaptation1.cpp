#include "Arduino.h"
#include "Exaptation1.h"

Exaptation1::Exaptation1( int waterPin, int heatPwm, int ledPwm[4],
							int ledPinAddrs[4][2], int fanPwmPins[2], int inputPins[5] )
{
	

	pinMode( waterPin, OUTPUT );
	_waterPin = waterPin;
	
	pinMode( heatPwm, OUTPUT );
	_heatPwm = heatPwm;
	
	for( int i = 0; i < 4; i++ )
	{
		pinMode( ledPwmPins[i], OUTPUT );
		_ledPwmPins[i] = ledPwmPins[i];
		
		for( int j = 0; j < 2; j++ )
			pinMode( ledPinAddrs[i][j], OUTPUT );
	}
	_ledPinAddrs = ledPinAddrs;
	
	for( int k = 0; k < 2; k++ )
		pinMode( fanPwmPins[k], OUTPUT );
	_fanPwmPins = fanPwmPins;
	
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
	
	_MUX_ADDRS[10][3] = {
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
	
	_FAN_COOLDOWN_DURATION = secondsToMHz( 2 * 60 );
	_heatTimer = 0;
	_waterTimer = 0;
}

void Exaptation1::writeLightChannel( int channel, int value )
{
	for( int i = 1; i <= 2; i++ )
	{
		digitalWrite( _ledPinsArray[_MUX_ADDRS[channel][0]], _MUX_ADDRS[channel][i] );
	}
	
	analogWrite( _ledPwmPins[_MUX_ADDRS[channel][0]], value );
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