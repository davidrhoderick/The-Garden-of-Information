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

int LED_MUX_PINS[4][2] = { 
      { 22, 23 },
      { 24, 25 },
      { 26, 27 },
      { 28, 29 }
  };
int LED_PWM_PINS[4] = { 5, 6, 7, 8 };

int _MUX_ADDRS[10][3] = {
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

void setup()
{
  for( int i = 0; i < 4; i++ )
  {
    pinMode( LED_PWM_PINS[i], OUTPUT );
    	
    for( int j = 0; j < 2; j++ )
      pinMode( LED_MUX_PINS[i][j], OUTPUT );
  }
}

void loop()
{
  /*for( int h = 0; h < 10; h++ )
  {
    for( int i = 0; i < 256; i++ )
    {
      writeLightChannel( h, i );
      delay( 1 );
    }
    for( int i = 254; i > 0; i-- )
    {
      writeLightChannel( h, i );
      delay( 1 );
    }
    delay(100);
  }*/
  
  digitalWrite( 22, 0 );
  digitalWrite( 23, 0 );
  analogWrite( 5, 40 );
  delay( 1000 );
  digitalWrite( 22, 0 );
  digitalWrite( 23, 0 );
  analogWrite( 5, 0 );
  delay( 1000 );
  
  digitalWrite( 24, 0 );
  digitalWrite( 25, 0 );
  analogWrite( 6, 40 );
  delay( 1000 );
  digitalWrite( 24, 0 );
  digitalWrite( 25, 0 );
  analogWrite( 6, 0 );
  delay( 1000 );
  
  digitalWrite( 22, 1 );
  digitalWrite( 23, 0 );
  analogWrite( 5, 40 );
  delay( 1000 );
  digitalWrite( 22, 1 );
  digitalWrite( 23, 0 );
  analogWrite( 5, 0 );
  delay( 1000 );
  
}

void writeLightChannel( int channel, int value )
{
  for( int i = 0; i < 2; i++ )
    digitalWrite( LED_MUX_PINS[_MUX_ADDRS[channel][0]][i], _MUX_ADDRS[channel][i+1] );
	
  analogWrite( LED_PWM_PINS[_MUX_ADDRS[channel][0]], value );
}
