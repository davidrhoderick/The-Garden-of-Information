#include <Exaptation1.h>
#include <LightSensor.h>
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

int WATER_PIN = 2;
int HEAT_PWM = 3;
int LED_MUX_PINS[4][2] = { 
      { 22, 23 },
      { 24, 25 },
      { 26, 27 },
      { 28, 29 }
  };
int LED_PWM_PINS[4] = { 5, 6, 7, 8 };
int FAN_PWM_PINS[2] = { 9, 10 };
// analog moisture input, scl for temperature input, sda for temperature input,
// scl for light sensor, scl for temperature input.
int INPUT_PINS[5] = { 0, 1, 2, 3, 4 };

Exaptation1 plantRobot( WATER_PIN, HEAT_PWM, LED_PWM_PINS, LED_MUX_PINS, FAN_PWM_PINS, INPUT_PINS );

void setup()
{
  Serial.begin( 9600 );
  
  pinMode( 30, OUTPUT );
  digitalWrite( 30, LOW );
  /*for(int i=0; i<10; i++)
    plantRobot.printLightChannel(i);*/
}

void loop()
{
  /*for( int h = 0; h < 10; h++ )
  {
    for( int i = 0; i < 256; i++ )
    {
      plantRobot.writeLightChannel( h, i );
      delay( 10 );
    }
    for( int i = 254; i > 0; i++ )
    {
      plantRobot.writeLightChannel( h, i );
      delay( 10 );
    }
  }*/
  
  plantRobot.writeLightChannel( 9, 20 );
  delay(100);
  plantRobot.writeLightChannel( 9, 0 );
  delay(100);
  
  
}
