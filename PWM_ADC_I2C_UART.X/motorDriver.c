/* ************************************************************************** */
/** motorDriver.c

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
motorDriver.c

@Summary
 Peripherals used:
 * Timer2 with OC1+OC2- PWM generation for two different motors
 * Timer6 & Timer7 - Current and position control loops. Timer3 and timer5 have been used left out as they can be used for ADC triggering
 
 
@Description
    This file sets up the PWM and the current and the position control loops that can be used to 
 * control two motors simultaneously.
 */



#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>
#include"mpu9250.h"
#include"AS5600L.h"


/*Function to initialize PWM*/
void PWM_init()
{
    /*
    * PWM generation uses Output compare(OC1 and OC2) and the Timer2 which is controlled by PBCLK3 
    * set to run at SYS_FREQ/2 in set_performance_mode(). SYS_FREQ=200MHz
    * Initializes global PWM frequency to be used by the motors
    * OUTPUTs at RPE8 and RF2
    */
    
    /* Initialize a PWM of 20KHz that is to be used to drive the motors
     * Peripherals used:
     * Timer2 and OC1,OC2 have been used to configure PortE8,PortF2 for PWM of two motors
    */
        
    /*
     * convert pin8 of port E to a digitalPIN; this is redundant code as the pins are being
     * set to digital in the set_digital function, but still needs to be done to ensure that the pins 
     * can support PWM operations when set_digital function is not being used
     */
    
    /* Force PIN to be digital output*/
    ANSELEbits.ANSE8=0;
    // set pin8 of port E as output
    TRISEbits.TRISE8=0;
    
    /*************************************************************************/   
    RPE8R=0b1100;// set output pin for OC1 as pin8 port E 
    RPF2R=0b1011;// set output pin for OC2 as pi2 port F 
    
    OC1CONbits.ON=0; //disable output compare1
    OC2CONbits.ON=0; //disable output compare2
    T2CONbits.ON=0; //disable timer2
    TMR2=0;//set timer2 value to 0
    IEC0bits.T2IE = 0;  // Disable Timer 2 Interrupt
    
    /************************************************************************/
    //setup Output Compare1 to be used for generating PWM1
    OC1CONbits.OC32=0;// set compare mode to 16 bit timer source(in this case timer 2)
    OC1CONbits.OCTSEL=0; //select timer 2(X) as the comparison reference timer.
    OC1CONbits.OCM=0b110;//compare event forces OC1 high
    OC1R=100;// set initial duty cycle of signal as 0
    
    
    /*************************************************************************/
    //setup Output Compare2 to be used for generating PWM2
    OC2CONbits.OC32=0;// set compare mode to 16 bit timer source(in this case timer 2)
    OC2CONbits.OCTSEL=0; //select timer 2(X) as the comparison reference timer.
    OC2CONbits.OCM=0b110;//compare event forces OC2 high
    OC2R=100;// set initial duty cycle of signal as 0
    
    
    /**************************************************************************/
    //setup timer2 for output compare
    T2CONbits.TCKPS=0b011;// use a prescaler of 8
    PR2=PWM_FREQ_20; //setup  period register for overflow
    /*period register is calculated by:PBCLK3/(desired speed)=prescaler*(period register +1)
     In the current application prescaler=8, desired speed =20KHz, hence PR2=624,
     * For 10KHz set period register =1249
     */
    
    /***************************************************************************/
    OC1CONbits.ON=1; //enable output compare1
    OC2CONbits.ON=1; //enable output compare2
    T2CONbits.ON=1; //enable timer2
      
    //to change duty cycle write new duty cycle value to OC1RS and not directly OC1R; 
    
       
}


/*Function to setup looping speed for inner and outer loops of the control system
 * Timer 6 and Timer 7 is being used as timer3 and timer5 can be used for ADC triggering
 */
void Control_loop_init()
{
    
    /* Don't know why but the reference clock for the timer6 and timer7 i.e PBCLK3, while 
     * running interrupts is running at a speed that is half(50MHz) of what is set during the performance mode (100MHz).
     * Apparently the clock source for the interrupt is running at 50MHz hence 
     * this value is used as a reference to calculate the value of the period register.
     * The 'current control' loop needs to run at  2000Hz while 'position control' needs to run at 200Hz     
     */ 
    
    /******************************************************************/
    asm volatile("di"); // Disable all interrupts. Don't enable global interrupts before all peripherals are configured.

    //Initialize timer6 for current control
    T6CON   = 0x0;      // Disable timer 6 when setting it up
    TMR6    = 0;        // Set timer 6 counter to 0
    IEC0bits.T6IE = 0;  // Disable Timer 6 Interrupt
    
    T6CONbits.TCKPS=0b11;// use a pre scaler of 8
    
    /*PR=(50MHz/desired_speed*Prescaler)-1*/
    PR6=CURRENT_CONTOL_FREQ_1000;//for a desired speed of 1000Hz
    
    
    IFS0bits.T6IF = 0;  // Clear interrupt flag for timer 6
    IPC7bits.T6IP = 3;  // Interrupt priority 3
    IPC7bits.T6IS = 1;  // Sub-priority 1
    IEC0bits.T6IE = 1;  // Enable Timer 6 Interrupt
    
    
    /******************************************************************/
    
    //Intialize timer7 for position control 
    T7CON   = 0x0;      // Disable timer 7 when setting it up
    TMR7    = 0;        // Set timer 7 counter to 0
    IEC1bits.T7IE = 0;  // Disable Timer 7 Interrupt    
    
    //T7CONbits.TCKPS = 0b11; // Pre-scale of 8
    T7CONbits.TCKPS = 0b101; // Pre-scale of 32
    
    
    /*PR=(50MHz/desired_speed*Prescaler)-1*/
    PR7=POSITION_CONTROL_FREQ_100;//for desired speed of 200Hz
    //PR7=31249; //for 50Hz frequency with pre scale set to 32
            
    IFS1bits.T7IF = 0;  // Clear interrupt flag for timer 7
    IPC8bits.T7IP = 4;  // Interrupt priority 4
    IPC8bits.T7IS = 1;  // Sub-priority 1
    IEC1bits.T7IE = 1;  // Enable Timer 7 Interrupt
    
    /*******************************************************************/
    
    
    T6CONbits.TON   = 1;//Turn on Timer6
    T7CONbits.TON   = 1;//Turn on Timer7
    
    //asm volatile("ei"); // Enable Global Interrupts once all peripherals are configured

}

/*Function to initialize the motor driver, i.e the PWM along with the current and position control loops*/

void Motor_driver_init()
{
    PWM_init();
    Control_loop_init();
}