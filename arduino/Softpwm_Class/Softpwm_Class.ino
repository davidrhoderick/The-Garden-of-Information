#define PWM_NUM_PINS 16
#define PWM_MAX_DUTY 255
#define PWM_DEFAULT_FREQ (16000000.0/128.0) //2MHz for /8 prescale from 16MHz

//?? Software based PWM (Pulse width modulation) library for the ATMEGA168/328 (Arduino).
// 
// This class implements PWM in software.  PWM is a method whose purpose is to emulate an analog voltage by rapidly toggling a digital
// pin between the high and low states.  By changing the duty cycle -- the percent of time the pin is "on" verses "off" the apparent voltage changes.
// This technique is only useful if the pin is controlling a device that averages voltages; for example:
// * Inductors (electromagnets, motors)
// * Capacitors
// * Human perception (via LEDs for example)
// This library does not work as efficiently as the hardware based PWM availabe in the ATMEGA, so that should be used if possible.
// However this library offers PWM on all pins, and also allows you to specify what the "on" and "off" pin states are.  For example, you could choose the "off" state to be high impedence.
// 
class SoftPWM
{
  public:
  typedef enum
  {
    DRIVEN_LOW = 2 | 0,
    DRIVEN_HIGH = 2 | 1,
    DRIVEN = 2,
 
    FLOATING = 0,
    PULL_UP = 1,
    HIGH_IMPEDENCE = 0,

    UNUSED = 4   
  } PinBehavior;
  //?? Constructor
  SoftPWM();

  //?? Call this periodically to trigger a state change
  void loop(void);
  
  //?? Automatically call the loop() function periodically.  Once you call this function YOU SHOULD NOT CALL LOOP()!
  void startAutoLoop(int timer=2,int frequency=PWM_DEFAULT_FREQ);  
  //?? Stop automatic looping.
  void stopAutoLoop(void);
  
  //?? Duty Cycle.  The fraction of time that the pin is HIGH is duty/255
  uint8_t duty[PWM_NUM_PINS];
  
  //?? Set what it means for a pin to be off or on.
  // Typically you would want to PWM an output pin and toggle output voltage.  This is what PWM normally means, and is the default.
  //       However, for other applications you may want to toggle the impedence and value simultaneously.
  //       For example, if the pin is sinking the base of a PNP transistor then you would switch from a high impedence (i.e. open circuit) state to a low output state (i.e. closed circuit, sinking current).
  //       
  void enablePin(int pin, PinBehavior offMode=DRIVEN_LOW, PinBehavior onMode=DRIVEN_HIGH)
  {
    pinOnBehavior[pin] = onMode; 
    pinOffBehavior[pin] = offMode;
  }
  void disablePin(int pin) { pinOnBehavior[pin] = UNUSED; pinOffBehavior[pin] = UNUSED; }
 
  //?? Set all duty cycles to 0
  void zero(void)
  {
    for (int i=0;i != PWM_NUM_PINS;i++)
      duty[i] = 0;
  }

  uint8_t pinOffBehavior[PWM_NUM_PINS];
  uint8_t pinOnBehavior[PWM_NUM_PINS];
  int bresenham[PWM_NUM_PINS];
};

SoftPWM::SoftPWM()
{
  for (int c=0;c != PWM_NUM_PINS;c++)
  {
    bresenham[c] = 0;
    pinOffBehavior[c] = UNUSED;
    pinOnBehavior[c] = UNUSED;
  }
  
  
}

void SoftPWM::loop(void)
  {
  boolean lvl;
  unsigned char regLvlOut=0;
  unsigned char regLvlIn;
  
  unsigned char regDirOut=0;
  unsigned char regDirIn;
  
  for (int i=0;i != PWM_NUM_PINS;i++)
    {
      if (i==0) { regLvlIn = PORTD; regDirIn = DDRD; }
      if (i==8) { regLvlIn = PORTB; regDirIn = DDRB; }
      
      if (i==8)
      {
        DDRD = regDirOut;
        PORTD = regLvlOut;
      }

      regLvlOut >>=1;
      regDirOut >>=1;

      if ((pinOnBehavior[i] & UNUSED) == UNUSED)  // If its unused then keep the values the same
        {
          regDirOut |= (regDirIn&1);
          regLvlOut |= (regLvlIn&1);
        }
      else
        {
        bresenham[i] += duty[i];
        if (bresenham[i]>=PWM_MAX_DUTY) 
          {
          bresenham[i] -= PWM_MAX_DUTY;
          lvl = true;
          }
        else lvl = false;
        
        if (lvl)
          {
            regLvlOut |= (pinOnBehavior[i]&1) ? 0x80:0;
            regDirOut |= (pinOnBehavior[i]>>1) ? 0x80:0;
          }
        else
          {
            regLvlOut |= (pinOffBehavior[i]&1) ? 0x80:0;
            regDirOut |= (pinOffBehavior[i]>>1) ? 0x80:0;            
          }
      
        }

      regLvlIn >>=1;
      regDirIn >>=1;
    }
    
    if (1)
      {
        DDRB = regDirOut;
        PORTB = regLvlOut;
      }    
  }
