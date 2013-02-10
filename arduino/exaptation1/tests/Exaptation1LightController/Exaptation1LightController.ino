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
      { 24, 25 },
      { 22, 23 },
      { 28, 29 },
      { 26, 27 }
  };
int LED_PWM_PINS[4] = { 6, 5, 8, 7 };

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
  for( int h = 0; h < 10; h++ )
  {
    for( int i = 0; i < 256; i++ )
    {
      writeLightChannel( h, i );
      delay( 10 );
    }
    for( int i = 254; i > 0; i++ )
    {
      writeLightChannel( h, i );
      delay( 10 );
    }
  }
}

void writeLightChannel( int channel, int value )
{
  for( int i = 1; i <= 2; i++ )
    digitalWrite( LED_MUX_PINS[_MUX_ADDRS[channel][i]][i], _MUX_ADDRS[channel][i] );
	
  analogWrite( LED_PWM_PINS[_MUX_ADDRS[channel][0]], value );
}
