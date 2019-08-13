/** Descriptive File Name
 
  @Author
 Aniket Mazumder
 
  @Company
 Universiy of Groningen

  @File Name
 PWM.c

  @Summary
  source file to setup PWM's of PIC 32MZ.
  
    
 */





#include <xc.h>


/*Timer 2(x) is used as the timer source for the PWM
 Value in the period register PR sets the frequency for the PWM.
 This corresponds to the TOP value for the timer2(x)
 Value in register OC1R,OC2R,OC1RS,OC2RS set the duty cycle of the PWM 
 */

//Initializes global PWM frequency to be used by the motors
//OUTPUTs at RPE8 and RF2
void PWM_init()
{
    /* Initialize a PWM of 60KHz that is to be used to drive the motors
     * Peripherals used:
     * Timer2, OC1,OC2  
     * PortE8,PortF2   
     */
    
    
    //Change default division factor for PBCLK which acts as a reference for the 
    //timers to 1, by default it is 2;
    //check https://microchipdeveloper.com/32bit:mz-osc
    //and  Section 42. Oscillators with Enhanced PLL page13 of microchip reference manual 
    
    
    SYSKEY = 0xAA996655; // Unlocking sequence
    SYSKEY = 0x556699AA;
    PB3DIVbits.PBDIV = 0; // Divide by 1
    PB3DIVbits.ON = 1; // PB3DIV enable
    SYSKEY = 0x33333333; // Locking sequence
    
    
    //convert pin8 of port E to a digitalPIN
    //this needs to be done as the development board has only a single pin for PWM
    ANSELEbits.ANSE8=0;
    // set pin8 of port E as output
    TRISEbits.TRISE8=0;
    
    
    
    RPE8R=0b1100;// set output pin for OC1 as pin8 port E 
    RPF2R=0b1011;// set output pin for OC2 as pi2 port F 
    
    OC1CONbits.ON=0; //disable output compare1
    OC2CONbits.ON=0; //disable output compare2
    T2CONbits.ON=0; //disable timer2
    
    //setup Output Compare1 to be used for generating PWM1
    OC1CONbits.OC32=0;// set compare mode to 16 bit timer source(in this case timer 2)
    OC1CONbits.OCTSEL=0; //select timer 2(X) as the comparison reference timer.
    OC1CONbits.OCM=0b110;//compare event forces OC1 high
    OC1R=100;// set initial duty cycle of signal as 0
    
    //setup Output Compare1 to be used for generating PWM2
    OC2CONbits.OC32=0;// set compare mode to 16 bit timer source(in this case timer 2)
    OC2CONbits.OCTSEL=0; //select timer 2(X) as the comparison reference timer.
    OC2CONbits.OCM=0b110;//compare event forces OC2 high
    OC2R=100;// set initial duty cycle of signal as 0
    
    //setup timer2 for output compare
    T2CONbits.TCKPS0=0; T2CONbits.TCKPS1=0;T2CONbits.TCKPS2=0;//set timer1 pre-scaler to none
    PR2=4095; //setup  period register for overflow
    
    OC1CONbits.ON=1; //enable output compare
    OC2CONbits.ON=1; //enable output compare
    T2CONbits.ON=1; //enable timer
    
   
    //to change duty cycle write new duty cycle value to OC1RS and not directly OC1R; 
}

//set dutyCycle to Motor1; set 4095 for 100% DC 
void setDutyCycleM1(uint16_t dutyCycle1)
{
    // required dutyCycle is applied to Motor1
    OC1RS=dutyCycle1;
}

//set dutyCycle to Motor2; set 4095 for 100% DC 
void setDutyCycleM2(uint16_t dutyCycle2)
{
    // required dutyCycle is applied to Motor2
    OC2RS=dutyCycle2;
}
