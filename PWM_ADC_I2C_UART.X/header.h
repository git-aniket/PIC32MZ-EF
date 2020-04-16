/* ************************************************************************** */
/**header.h 

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
 * header.h

@Summary
 Header file for PIC32 MyLeg project  

@Description
 This header file contains definitions of all essential device independent 
 * headers that are used in the project.    
 */
/***************************************************************************************/

#ifndef _HEADER_H    /* Guard against multiple inclusion */
#define _HEADER_H

#define SYS_FREQ 200000000              // Running at 200MHz
#define PWM_FREQ_20 624                 //Value of Period register for 20KHz PWM
#define PWM_FREQ_10 1249                //Value of Period register for 10KHz PWM

#define CURRENT_CONTOL_FREQ_1000 6249        //Value of Period register for 1000Hz speed of current control, 50MHz and Prescaler of 8
#define POSITION_CONTROL_FREQ_100 15624     //Value of Period register for 200Hz speed of positon control 50MHz and Prescaler of 8
#define UART_DATA_FREQUENCY_50 31249 //Value of Period Register for 100 Hz frequency for, 50MHz clock and prescaler of 32 


/*******************************************************************************/
#define onn 0b1
#define off 0b0

#define set 0b1
#define clear 0b0

/***************************************************************************/
/*The RGB LED's are pulled UP by default hence, needs to be set 
in order to turn them off and vice versa*/
#define RED_RGB_LED LATBbits.LATB5=0, LATBSET=0b11;//set bits 0 and 1 and clear bit 5 to get red RGB
#define BLUE_RGB_LED LATBbits.LATB0=0, LATBSET=0b100010;// set bits 1 and 5 and clear bit 0 to get blue RGB
#define GREEN_RGB_LED LATBbits.LATB1=0, LATBSET=0b100001;//set bits 0 and 5 abd clear bit 1 to get green
#define RGB_LED_OFF LATBSET=0b100011;//set bits 0,1 and 5 to switch off the RGB

/*Unlike the RGB LED, the user LED's are pulled LOW by default*/
#define RED_LED_ON LATEbits.LATE3=1;
#define RED_LED_OFF LATEbits.LATE3=0;
#define GREEN_LED_ON LATEbits.LATE4=1;
#define GREEN_LED_OFF LATEbits.LATE4=0;
#define YELLOW_LED_ON LATEbits.LATE6=1;
#define YELLOW_LED_OFF LATEbits.LATE6=0; 

/*defines for Pre fetch Cache */
#define PREFETCH_ANY_ADDRESS 0b11
#define PREFETCH_CPU_INSTRUCTIONS_DATA 0b10
#define PREFETCH_CPU_INSTRUCTIONS 0b01
#define PREFETCH_DISABLE 0b00

#define ZERO_WAIT_STATES 0b00
#define ONE_WAIT_STATE 0b01
#define TWO_WAIT_STATES 0b10

/***************************************************************************/
/*Shared variables between ISR's and other global variables */
extern volatile uint8_t flag_ankle_IMU,flag_ankle_encoder,flag_ankle_current,flag_print; //variables utilized to flag interrupts
extern volatile uint16_t ADC1,ADC2,ADC3;//variables for ADC
extern volatile int16_t  accelX,accelY,accelZ;//variables for IMU

#endif /* _HEADER_H */
