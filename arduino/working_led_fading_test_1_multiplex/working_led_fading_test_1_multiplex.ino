//Mux control pins
int s0 = 8;
int s1 = 9;
int s2 = 10;
int s3 = 11;

//Mux in "SIG" pin
int PWM_pin = 3;

int array[10] = { 0, 50, 100, 150, 200, 250, 0, 50, 100, 150 };

void setup(){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  //Serial.begin(9600);
}

void loop(){
  //Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
  
  int delaytime = 80;
  
  for(int array = 0; array < 5; array ++){
    for(int britness = 0; britness < 256; britness++ )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
    for( int britness = 255; britness >= 0; britness-- )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
  }
  
  for(int array = 3; array >= 0; array-- )
  {
    for(int britness = 0; britness < 256; britness++ )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
    for( int britness = 255; britness >= 0; britness-- )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
  }
  
  for(int array = 5; array < 10; array++ )
  {
    for(int britness = 0; britness < 256; britness++ )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
    for( int britness = 255; britness >= 0; britness-- )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
  }
  
  for( int array= 8; array >= 5; array-- )
  {
    for(int britness = 0; britness < 256; britness++ )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
    for( int britness = 255; britness >= 0; britness-- )
    {
      writeMux( array, britness );
      delayMicroseconds(delaytime);
    }
    delayMicroseconds(delaytime);
  }

}

void writeMux(int channel, int val){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  
  analogWrite( PWM_pin, val );
}
