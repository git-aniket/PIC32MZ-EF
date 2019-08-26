/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Description
    Header  file containing the function prototypes for the generic functions used in the main program 
 */
/* ************************************************************************** */

#ifndef _HEADER_H    /* Guard against multiple inclusion */
#define _HEADER_H
#define _XTAL_FREQ 200000000UL
#define SYS_FREQ 200000000UL
#include <xc.h>

//function prototypes

void PWM_init();// initialize the PWM frequency to be used by the motors

void ADC_init();// initialize the ADC's to capture data from the analog peripherals.
void setDutyCycleM1(uint16_t duty_cycle);
void setDutyCycleM2(uint16_t duty_cycle);
//void getADC();
void getADC(uint16_t *ADC1,uint16_t *ADC2,uint16_t *ADC3);
/* This function sets up the clocks to be used as source for the 
 * peripherals of the PIC32 microcontroller*/
long set_performance_mode();
void set_digital();//set all ports to be digital ports by default
//initialize UART
void UART_Init();

//send character via UART
void _mon_putc (char c);

#endif /* _CONFIG_H */

/* *****************************************************************************
 End of File
 */
