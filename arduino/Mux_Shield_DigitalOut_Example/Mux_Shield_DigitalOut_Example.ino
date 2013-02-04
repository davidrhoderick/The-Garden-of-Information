//Mux_Shield_DigitalOut_Example
//http://mayhewlabs.com/arduino-mux-shield

/*
This example shows how to output high or low on all 48 pins.  To use the analog pins as digital, we use
pin numbers 14-16 (instead of analog numbers 0-2). 

To simplify this code further, one might use nested for loops or function calls.
*/

//Give convenient names to the control pins
#define CONTROL0 5    //MUX control pin 0 (S3 is connected to Arduino pin 2)
#define CONTROL1 4
#define CONTROL2 3
#define CONTROL3 2

void setup()
{
  //Set MUX control pins to output
  pinMode(CONTROL0, OUTPUT);
  pinMode(CONTROL1, OUTPUT);
  pinMode(CONTROL2, OUTPUT);
  pinMode(CONTROL3, OUTPUT);
}
  

void loop()
{

  //Since all 3 multiplexers have the same control pins, the one multiplexer data line we want to 
  //talk to should be set to output and the other two multiplexer lines should be be 'bypassed' by 
  //setting the pins to input
    
  //Turn on output to digital pin 14 (MUX 0) and turn off the other 2 multiplexer data pins
  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
    
  //This for loop is used to scroll through the FIRST multiplexer
  for (int i=0; i<16; i++)
  {    
    //The following 4 commands set the correct logic for the control pins to select the desired input
    //See the Arduino Bitwise AND Reference: http://www.arduino.cc/en/Reference/BitwiseAnd
    //See the Aruino Bitshift Reference: http://www.arduino.cc/en/Reference/Bitshift
    digitalWrite(CONTROL0, (i&15)>>3); //S3
    digitalWrite(CONTROL1, (i&7)>>2);  //S2
    digitalWrite(CONTROL2, (i&3)>>1);  //S1
    digitalWrite(CONTROL3, (i&1));     //S0
    
    digitalWrite(14, HIGH);
    delay(100);
    digitalWrite(14, LOW);
    delay(100);
  }
  
  //Turn on output to digital pin 15 (MUX 1) and turn off the other 2 multiplexer data pins
  pinMode(14, INPUT);
  pinMode(15, OUTPUT);
  pinMode(16, INPUT);
  
  //This for loop is used to scroll through the SECOND multiplexer
  for (int i=0; i<16; i++)
  {
    digitalWrite(CONTROL0, (i&15)>>3); //S3
    digitalWrite(CONTROL1, (i&7)>>2);  //S2
    digitalWrite(CONTROL2, (i&3)>>1);  //S1
    digitalWrite(CONTROL3, (i&1));     //S0
    
    digitalWrite(15, HIGH);
    delay(100);
    digitalWrite(15, LOW);
    delay(100);
  }
  
  //Turn on output to digital pin 16 (MUX 2) and turn off the other 2 multiplexer data pins
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, OUTPUT);
  
  //This for loop is used to scroll through the THIRD multiplexer
  for (int i=0; i<16; i++)
  {
    digitalWrite(CONTROL0, (i&15)>>3); //S3
    digitalWrite(CONTROL1, (i&7)>>2);  //S2
    digitalWrite(CONTROL2, (i&3)>>1);  //S1
    digitalWrite(CONTROL3, (i&1));     //S0
    
    digitalWrite(16, HIGH);
    delay(100);
    digitalWrite(16, LOW);
    delay(100);
  }
}
