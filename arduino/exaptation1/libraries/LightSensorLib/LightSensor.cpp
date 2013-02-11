/*
  ADJD-S311 Color Sensor Breakout Example Code
  by: Jim Lindblom
  SparkFun Electronics
  date: 8/9/11
  License: MIT license (http://www.opensource.org/licenses/mit-license.php)  
  This code is slightly modified from that posted on bildr.org's
  excellent tutorial (http://bildr.org/2011/01/adjd-s371-tutorial/), which
  was taken from Marcus' great initial code
  (http://interactive-matter.eu/2008/08/tinkering-with-adjd-s371-q999/).
  Thanks to Adam and Marcus for the initial code! I'd definitely recommend checking out 
  their tutorial/posts.

  This example code initializes and calibrates the sensor upon startup. When calibrating
  the code is assuming the sensor is faced with a stable WHITE source. To best calibrate
  the sensor's capacitor and integration registers, it needs to be looking at a usual white
  object at calibration.
  
  After start-up, the sensor will wait for serial input. Open up the serial 
  monitor at 9600 bps. A space ' ', will prompt the sensor for all of it's register
  values. 'c' will perform calibration (make sure it's aimed at a white object. 'o' 
  will get the offset values (these are not trimmed, see getOffset() function). And 'l'
  will continously get the sensor readings and output them to an RGB LED.
  
  You may want to try with the sensor's LED (connected to Arduino pin 2) both on and off.
  I usually get better results with the LED off, but it depends on the object being sensed.
  It does a really good job of sensing colors off my LCD monitor.

  the hookup:
  ADJD-S311 Breakout ------------- Arduino
  ----------------------------------------
      LED ---------------------------D2
      3.3V -------------------------3.3V
      GND -------------------------- GND
      SCL -------------------------- A5
      SDA -------------------------- A4
      GND -------------------------- GND  
      SLP --------------------- Not connected
      CLK --------------------- Not connected
*/
#include "Arduino.h"
#include "Wire.h"  // We use Wire.h to talk I2C to the sensor
#include "LightSensor.h"

LightSensor::LightSensor()
{
	// Pin definitions:
	int sdaPin = A4;  // serial data, hardwired, can't change
	int sclPin = A5;  // serial clock, hardwired, can't change
	int ledPin = 2;  // LED light source pin, any unused pin will work

	// RGB LED pins, should all be PWM output pins:
	int redledPin = 9;
	int greenledPin = 10;
	int blueledPin = 11;
	int rgbPins[3] = {redledPin, greenledPin, blueledPin};

	// initial values for integration time registers
	unsigned char colorCap[4] = {9, 9, 2, 5};  // values must be between 0 and 15
	unsigned int colorInt[4] = {2048, 2048, 2048, 2048};  // max value for these is 4095
	unsigned int colorData[4];  // This is where we store the RGB and C data values
	signed char colorOffset[4];  // Stores RGB and C offset values
}

/* printADJD_S311Values() reads, formats, and prints all important registers
of the ADJD-S311. 
It doesn't perform any measurements, so you'll need to call getRGBC() to print
new values.
*/
void LightSensor::printADJD_S311Values()
{
  Serial.println("\t\t Red \t Green \t Blue \t Clear");
  Serial.print("Data: \t\t ");
  for (int i=0; i<4; i++)
  {
    Serial.print(colorData[i]);
    Serial.print("\t ");
  }
  Serial.println();
  Serial.print("Caps: \t\t ");
  for (int i=0; i<4; i++)
  {
    Serial.print(readRegister(CAP_RED+i), DEC);
    Serial.print("\t ");
  }
  Serial.println();
  Serial.print("Int: \t\t ");
  for (int i=0; i<4; i++)
  {
    Serial.print(readRegisterInt(INT_RED_LO+(i*2)), DEC);
    Serial.print("\t ");
  }
  Serial.println();
  Serial.print("Offset: \t ");
  for (int i=0; i<4; i++)
  {
    Serial.print((signed char) readRegister(OFFSET_RED+i), DEC);
    Serial.print("\t ");
  }
  Serial.println();
}

/* initADJD_S311() - This function initializes the ADJD-S311 and its
capacitor and integration registers 
The vaules for those registers are defined near the top of the code.
the colorCap[] array defines all capacitor values, colorInt[] defines
all integration values.
*/
void LightSensor::initADJD_S311()
{ 
  /*sensor gain registers, CAP_...
  to select number of capacitors.
  value must be <= 15 */
  writeRegister(colorCap[RED] & 0xF, CAP_RED);
  writeRegister(colorCap[GREEN] & 0xF, CAP_GREEN);
  writeRegister(colorCap[BLUE] & 0xF, CAP_BLUE);
  writeRegister(colorCap[CLEAR] & 0xF, CAP_CLEAR);

  /* Write sensor gain registers INT_...
  to select integration time 
  value must be <= 4096 */
  writeRegister((unsigned char)colorInt[RED], INT_RED_LO);
  writeRegister((unsigned char)((colorInt[RED] & 0x1FFF) >> 8), INT_RED_HI);
  writeRegister((unsigned char)colorInt[BLUE], INT_BLUE_LO);
  writeRegister((unsigned char)((colorInt[BLUE] & 0x1FFF) >> 8), INT_BLUE_HI);
  writeRegister((unsigned char)colorInt[GREEN], INT_GREEN_LO);
  writeRegister((unsigned char)((colorInt[GREEN] & 0x1FFF) >> 8), INT_GREEN_HI);
  writeRegister((unsigned char)colorInt[CLEAR], INT_CLEAR_LO);
  writeRegister((unsigned char)((colorInt[CLEAR] & 0x1FFF) >> 8), INT_CLEAR_HI);
}

/* calibrateClear() - This function calibrates the clear integration registers
of the ADJD-S311.
*/
int LightSensor::calibrateClear()
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
int LightSensor::calibrateColor()
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
void LightSensor::calibrateCapacitors()
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

/* writeInt() - This function writes a 12-bit value
to the LO and HI integration registers */
void LightSensor::writeInt(int address, int gain)
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
void LightSensor::performMeasurement()
{  
  writeRegister(0x01, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
}

/* getRGBC() - This function reads all of the ADJD-S311's
data registers and stores them into colorData[]. To get the
most up-to-date data make sure you call performMeasurement() 
before calling this function.*/
void LightSensor::getRGBC()
{
  performMeasurement();
  
  colorData[RED] = readRegisterInt(DATA_RED_LO);
  colorData[GREEN] = readRegisterInt(DATA_GREEN_LO);
  colorData[BLUE] = readRegisterInt(DATA_BLUE_LO);
  colorData[CLEAR] = readRegisterInt(DATA_CLEAR_LO);
}

/* getOffset() - This function performs the offset reading
and stores the offset data into the colorOffset[] array.
You can turn on data trimming by uncommenting out the 
writing 0x01 to 0x01 code.
*/
void LightSensor::getOffset()
{
  digitalWrite(ledPin, LOW);  // turn LED off
  delay(10);  // wait a tic
  writeRegister(0x02, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
  //writeRegister(0x01, 0x01);  // set trim
  //delay(100);
  for (int i=0; i<4; i++)
    colorOffset[i] = (signed char) readRegister(OFFSET_RED+i);
  digitalWrite(ledPin, HIGH);
}

/* I2C functions...*/
// Write a byte of data to a specific ADJD-S311 address
void LightSensor::writeRegister(unsigned char data, unsigned char address)
{
  Wire.beginTransmission(ADJD_S311_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

// read a byte of data from ADJD-S311 address
unsigned char LightSensor::readRegister(unsigned char address)
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
int LightSensor::readRegisterInt(unsigned char address)
{
  return readRegister(address) + (readRegister(address+1)<<8);
}