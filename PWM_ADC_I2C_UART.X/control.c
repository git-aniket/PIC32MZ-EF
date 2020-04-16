/* ************************************************************************** */
/** control.c

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
control.c

@Summary
 Peripherals used:
  * Timer6 & Timer7 - Current and position control loops. Timer3 and timer5 have been used left out as they can be used for ADC triggering
 
@Description
   This file is used to change values for the gains of up the current and the position control loops that can be used to 
 * control two motors simultaneously.
 */


#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>
#include"mpu9250.h"
#include"AS5600L.h"



/*Interrupt service routines for timers 6 and 7 that control the looping speeds
 These timers have been set in motorDriver.c */

/*Current control function that tries to get the current up to the desired current based on the 
     measured and reference current. This function generated duty cycle that is assigned to the Motor*/
void __attribute__((vector(_TIMER_6_VECTOR), interrupt(ipl3srs), nomips16)) current_control_loop()
{
 //Loop runs at 1000 Hz   
    LATDbits.LATD9^=1;//Flip bits to check for looping frequency on RD9
    
    flag_ankle_current=1;
    getADC(&ADC1,&ADC2,&ADC3);
    
    IFS0bits.T6IF = 0;  // Clear interrupt flag for timer 6   
}


/*Position control function that checks the current position and generates torque signal to 
     reach the reference position. This function generates current/torque command that is passed on to the
     current control function
     */
void __attribute__((vector(_TIMER_7_VECTOR), interrupt(ipl4srs), nomips16)) position_control_loop()
{
    //Loop runs at 100Hz
    //uint32_t timeloop=0;
    //_CP0_SET_COUNT(0);
        
    IMUReadBytes(IMU_ADDRESS, ACCEL_XOUT_H,ACCEL_BIAS_X, &accelX);
    LATDbits.LATD12^=1;//Flip bits to check for looping frequency on RD12
    //IMUReadBytes(IMU_ADDRESS, ACCEL_YOUT_H,ACCEL_BIAS_Y, &accelY);
    flag_ankle_encoder=1;
    //IMUReadBytes(IMU_ADDRESS, ACCEL_ZOUT_H,ACCEL_BIAS_Z, &accelZ);

    
    IFS1bits.T7IF = 0;  // Clear interrupt flag for timer 7

    //timeloop=_CP0_GET_COUNT();
}



/*Functions to set the duty cycles to motor1 and motor2*/
void set_dutycycleM1(uint16_t dutyCycle1)
{
    /*This function is used to set dutyCycle to Motor1 on the fly*/ 
    OC1RS=dutyCycle1;
}


void set_dutycycleM2(uint16_t dutyCycle2)
{
    /*This function is used to set dutyCycle to Motor1 on the fly*/ 
    OC2RS=dutyCycle2;
}