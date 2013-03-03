#include <Exaptation1.h>
#include <Wire.h>

const int RED_1 = 0;
const int RED_2 = 1;
const int RED_3 = 2;
const int RED_4 = 3;
const int RED_5 = 4;
const int BLUE_1 = 5;
const int BLUE_2 = 6;
const int BLUE_3 = 7;
const int BLUE_4 = 8;
const int BLUE_5 = 9;

int WATER_PIN = A3;

int LED_MUX_PINS[6] = { 13, 12, 8, 7, 4, 2 };
int LED_PWM_PINS[4] = { 11, 10, 9, 6 };
int FAN_PWM_PINS[2] = { 3, 5 };
int MOISTURE_PINS[3] = { 0, 1, 2 };

unsigned int *lightReading;

int randomLights[4];
int waterTestTimer = 0;

Exaptation1 plantRobot( WATER_PIN, LED_PWM_PINS, LED_MUX_PINS, FAN_PWM_PINS, MOISTURE_PINS );

void setup()
{
  Serial.begin( 9600 );
  //plantRobot.setupLightSensor();
}

void loop()
{
  plantRobot.receiveSerial();
  
  /*plantRobot.writeLightChannel( 4, 255 );
  plantRobot.writeLightChannel( 9, 255 );
  delay( 1000 );
  
  plantRobot.writeLightChannel( 2, 255 );
  plantRobot.writeLightChannel( 7, 255 );
  delay( 1000 );
  
  plantRobot.writeLightChannel( 0, 255 );
  plantRobot.writeLightChannel( 5, 255 );
  delay( 1000 );*/
  
}
