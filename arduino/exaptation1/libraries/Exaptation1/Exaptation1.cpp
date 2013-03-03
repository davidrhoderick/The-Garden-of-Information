#include "Arduino.h"
#include "Wire.h"
#include "Exaptation1.h"

Exaptation1::Exaptation1( int waterPin, int ledPwmPins[4], int ledPinAddrs[6],
							int fanPwmPins[2], int moisturePins[3] )
{	
	pinMode( waterPin, OUTPUT );
	_waterPin = waterPin;
	digitalWrite( _waterPin, LOW );
	
	//pinMode( heatPwm, OUTPUT );
	//_heatPwm = heatPwm;
	
	for( int i = 0; i < 4; i++ )
	{
		pinMode( ledPwmPins[i], OUTPUT );
		_ledPwmPins[i] = ledPwmPins[i];
	}
		
	for( int j = 0; j < 6; j++ )
	{
		pinMode( ledPinAddrs[j], OUTPUT );
		_ledPinAddrs[j] = ledPinAddrs[j];
	}
	
	for( int k = 0; k < 2; k++ )
	{
		pinMode( fanPwmPins[k], OUTPUT );
		_fanPwmPins[k] = fanPwmPins[k];
	}
	
	pinMode( moisturePins[0], OUTPUT );
	_moistAn= moisturePins[0];
	
	//pinMode( inputPins[1], OUTPUT );
	//_tempSCL = inputPins[1];
	
	//pinMode( inputPins[2], OUTPUT );
	//_tempSDA = inputPins[2];
		
	_waterTimer = 0;
	
	_ventilateStatus = 0;
}

void Exaptation1::receiveSerial()
{
	char receiveControlArray[11];
  //byte parityControlArray[11];
  
  if( Serial.available() > 0 )
  {
    if( Serial.readBytesUntil( 's', receiveControlArray, 11 ) == 11 )
    {
      /*
      check the validity of the serial received by sending it
      
      for( int i = 0; i < 11; i++ )
      {
        parityControlArray[i] = receiveControlArray[i];
      }
      Serial.write( parityControlArray, 11 );
      Serial.flush();*/
      
      for( int i = 0; i < 8; i+=2 )
      {
        writeLightChannel( receiveControlArray[i], receiveControlArray[i+1] );
      }
    }
  }
}

/* setupLightSensor() - This function initializes the ADJD-S311 and its
capacitor and integration registers
The values for those registers are defined near the top of the code.
the colorCap[] array defines all capacitor values, colorInt[] defines
all integration values.
*/
void Exaptation1::setupLightSensor()
{
	// initial values for integration time registers
	unsigned char colorCap[4] = {9, 9, 2, 5};  // values must be between 0 and 15
	unsigned int colorInt[4] = {2048, 2048, 2048, 2048};  // max value for these is 4095
	unsigned int colorData[4];  // This is where we store the RGB and C data values
	signed char colorOffset[4];  // Stores RGB and C offset values

	writeLightChannel( 1, 255 );
	writeLightChannel( 2, 255 );
	writeLightChannel( 6, 255 );
	writeLightChannel( 7, 255 );
	
	Wire.begin();
	delay(1);  // Wait for ADJD reset sequence
	initADJD_S311();  // Initialize the ADJD-S311, sets up cap and int registers
	
	calibrateColor();  // This calibrates R, G, and B int registers
	calibrateClear();  // This calibrates the C int registers
	calibrateCapacitors();  // This calibrates the RGB, and C cap registers
	getRGBC();  // After calibrating, we can get the first RGB and C data readings
	
	writeLightChannel( 1, 0 );
	writeLightChannel( 2, 0 );
	writeLightChannel( 6, 0 );
	writeLightChannel( 7, 0 );
}

void Exaptation1::initADJD_S311()
{ 
  /*sensor gain registers, CAP_...
  to select number of capacitors.
  value must be <= 15 */
  writeRegister(colorCap[RED] & 0xF, 0x06);
  writeRegister(colorCap[GREEN] & 0xF, 0x07);
  writeRegister(colorCap[BLUE] & 0xF, 0x08);
  writeRegister(colorCap[CLEAR] & 0xF, 0x09);

  /* Write sensor gain registers INT_...
  to select integration time 
  value must be <= 4096 */
  writeRegister((unsigned char)colorInt[RED], 0xA);
  writeRegister((unsigned char)((colorInt[RED] & 0x1FFF) >> 8), 0xB);
  writeRegister((unsigned char)colorInt[BLUE], 0xE);
  writeRegister((unsigned char)((colorInt[BLUE] & 0x1FFF) >> 8), 0xF);
  writeRegister((unsigned char)colorInt[GREEN], 0xC);
  writeRegister((unsigned char)((colorInt[GREEN] & 0x1FFF) >> 8), 0xD);
  writeRegister((unsigned char)colorInt[CLEAR], 0x10);
  writeRegister((unsigned char)((colorInt[CLEAR] & 0x1FFF) >> 8), 0x11);
}

/* calibrateClear() - This function calibrates the clear integration registers
of the ADJD-S311.
*/
int Exaptation1::calibrateClear()
{
  int gainFound = 0;
  int upperBox=4096;
  int lowerBox = 0;
  int half;
  
  while (!gainFound)
  {
    half = ((upperBox-lowerBox)/2)+lowerBox;
    //no further halfing possbile
    if (half==lowerBox)
      gainFound=1;
    else 
    {
      writeInt(INT_CLEAR_LO, half);
      performMeasurement();
      int halfValue = readRegisterInt(DATA_CLEAR_LO);

      if (halfValue>1000)
        upperBox=half;
      else if (halfValue<1000)
        lowerBox=half;
      else
        gainFound=1;
    }
  }
  return half;
}

/* calibrateColor() - This function clalibrates the RG and B 
integration registers.
*/
int Exaptation1::calibrateColor()
{
  int gainFound = 0;
  int upperBox=4096;
  int lowerBox = 0;
  int half;
  
  while (!gainFound)
  {
    half = ((upperBox-lowerBox)/2)+lowerBox;
    //no further halfing possbile
    if (half==lowerBox)
    {
      gainFound=1;
    }
    else {
      writeInt(INT_RED_LO, half);
      writeInt(INT_GREEN_LO, half);
      writeInt(INT_BLUE_LO, half);

      performMeasurement();
      int halfValue = 0;

      halfValue=max(halfValue, readRegisterInt(DATA_RED_LO));
      halfValue=max(halfValue, readRegisterInt(DATA_GREEN_LO));
      halfValue=max(halfValue, readRegisterInt(DATA_BLUE_LO));

      if (halfValue>1000) {
        upperBox=half;
      }
      else if (halfValue<1000) {
        lowerBox=half;
      }
      else {
        gainFound=1;
      }
    }
  }
  return half;
}

/* calibrateCapacitors() - This function calibrates each of the RGB and C
capacitor registers.
*/
void Exaptation1::calibrateCapacitors()
{
  int  calibrationRed = 0;
  int  calibrationBlue = 0;
  int  calibrationGreen = 0;
  int calibrated = 0;

  //need to store detect better calibration
  int oldDiff = 5000;

  while (!calibrated)
  {
    // sensor gain setting (Avago app note 5330)
    // CAPs are 4bit (higher value will result in lower output)
    writeRegister(calibrationRed, CAP_RED);
    writeRegister(calibrationGreen, CAP_GREEN);
    writeRegister(calibrationBlue, CAP_BLUE);

    // int colorGain = _calibrateColorGain();
    int colorGain = readRegisterInt(INT_RED_LO);
    writeInt(INT_RED_LO, colorGain);
    writeInt(INT_GREEN_LO, colorGain);
    writeInt(INT_BLUE_LO, colorGain);

    int maxRead = 0;
    int minRead = 4096;
    int red   = 0;
    int green = 0;
    int blue  = 0;
    
    for (int i=0; i<4 ;i ++)
    {
      performMeasurement();
      red   += readRegisterInt(DATA_RED_LO);
      green += readRegisterInt(DATA_GREEN_LO);
      blue  += readRegisterInt(DATA_BLUE_LO);
    }
    red   /= 4;
    green /= 4;
    blue  /= 4;

    maxRead = max(maxRead, red);
    maxRead = max(maxRead, green);
    maxRead = max(maxRead, blue);

    minRead = min(minRead, red);
    minRead = min(minRead, green);
    minRead = min(minRead, blue);

    int diff = maxRead - minRead;

    if (oldDiff != diff)
    {
      if ((maxRead==red) && (calibrationRed<15))
        calibrationRed++;
      else if ((maxRead == green) && (calibrationGreen<15))
        calibrationGreen++;
      else if ((maxRead == blue) && (calibrationBlue<15))
        calibrationBlue++;
    }
    else
      calibrated = 1;
      
    oldDiff=diff;

    int rCal = calibrationRed;
    int gCal = calibrationGreen;
    int bCal = calibrationBlue;
  }
  
}

void Exaptation1::calibrateLightSensor()
{
	//Serial.println("\nCalibrating...\n");
	calibrateColor();
	calibrateClear();
	calibrateCapacitors();
	getRGBC();
}

/* getRGBC() - This function reads all of the ADJD-S311's
data registers and stores them into colorData[]. To get the
most up-to-date data make sure you call performMeasurement() 
before calling this function.*/
void Exaptation1::getRGBC()
{
  performMeasurement();
  
  colorData[RED] = readRegisterInt(DATA_RED_LO);
  colorData[GREEN] = readRegisterInt(DATA_GREEN_LO);
  colorData[BLUE] = readRegisterInt(DATA_BLUE_LO);
  colorData[CLEAR] = readRegisterInt(DATA_CLEAR_LO);
}

/* writeInt() - This function writes a 12-bit value
to the LO and HI integration registers */
void Exaptation1::writeInt(int address, int gain)
{
  if (gain < 4096) 
  {
    byte msb = gain >> 8;
    byte lsb = gain;

    writeRegister(lsb, address);
    writeRegister(msb, address+1);
  }
}

/* performMeasurement() - This must be called before
reading any of the data registers. This commands the
ADJD-S311 to perform a measurement, and store the data
into the data registers.*/
void Exaptation1::performMeasurement()
{  
  writeRegister(0x01, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
}

/* getOffset() - This function performs the offset reading
and stores the offset data into the colorOffset[] array.
You can turn on data trimming by uncommenting out the 
writing 0x01 to 0x01 code.
*/
void Exaptation1::getOffset()
{
  delay(10);  // wait a tic
  writeRegister(0x02, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
  //writeRegister(0x01, 0x01);  // set trim
  //delay(100);
  for (int i=0; i<4; i++)
    colorOffset[i] = (signed char) readRegister(OFFSET_RED+i);
}

/* I2C functions...*/
// Write a byte of data to a specific ADJD-S311 address
void Exaptation1::writeRegister(unsigned char data, unsigned char address)
{
  Wire.beginTransmission(ADJD_S311_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

// read a byte of data from ADJD-S311 address
unsigned char Exaptation1::readRegister(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(ADJD_S311_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(ADJD_S311_ADDRESS, 1);
  while (!Wire.available())
    ;  // wait till we can get data
  
  return Wire.read();
}

// Write two bytes of data to ADJD-S311 address and addres+1
int Exaptation1::readRegisterInt(unsigned char address)
{
  return readRegister(address) + (readRegister(address+1)<<8);
}

unsigned int Exaptation1::readRedChannel()
{
	getRGBC();
	return colorData[RED];
}

unsigned int Exaptation1::readBlueChannel()
{
	getRGBC();
	return colorData[BLUE];
}

int Exaptation1::writeLightChannel( int channel, int value )
{
	switch( channel )
	{
		case 1:
			digitalWrite( _ledPinAddrs[2], MUX_ADDRS_EVEN[0] );
			analogWrite( _ledPwmPins[1], value );
			break;
		case 3:
			digitalWrite( _ledPinAddrs[2], MUX_ADDRS_EVEN[1] );
			analogWrite( _ledPwmPins[1], value );
			break;
		case 6:
			digitalWrite( _ledPinAddrs[5], MUX_ADDRS_EVEN[2] );
			analogWrite( _ledPwmPins[3], value );
			break;
		case 8:
			digitalWrite( _ledPinAddrs[5], MUX_ADDRS_EVEN[3] );
			analogWrite( _ledPwmPins[3], value );
			break;
		case 0:
		case 2:
		case 4:	
			for( int i = 0; i < 2; i++ )
			{
				digitalWrite( _ledPinAddrs[i], MUX_ADDRS_ODD[channel/2][i] );
			}
			analogWrite( _ledPwmPins[0], value );
			break;
		default:
			for( int i = 0; i < 2; i++ )
			{
				digitalWrite( _ledPinAddrs[i+3], MUX_ADDRS_ODD[((channel-5)/2)+3][i] );
			}
			analogWrite( _ledPwmPins[2], value );
	}
	
	return value;
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

int Exaptation1::getVentilateStatus()
{
	return _ventilateStatus;
}

