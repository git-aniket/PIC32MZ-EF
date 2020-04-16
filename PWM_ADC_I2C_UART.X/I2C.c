
/* I2C.c
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 
  @Company
 University of Groningen
 
  @File Name
 I2C.c

  @Summary
 Source file containing definition of all methods for the i2c communication protocol for PIC32 MZ
 
 @Description
 This code sets up the I2C protocol for the PIC32.
 * USER LED's
 * LED1 RED LED ON- Unable to read from I2C device./ Unable to generate start condition 
 * LED3 YELLOW LED ON- Unable to write to I2C device./ Unable to generate stop condition. 
 */


#include "I2C.h"
#include<stdlib.h>
#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>



/*This function checks if the i2c bus is idle or not. If it is busy then it waits for the bus
 to get free before proceeding with the next operation*/
void I2C_wait_for_idle(void)
{
    while(I2C1CON & 0x1F); // Acknowledge sequence not in progress
                                // Receive sequence not in progress
                                // Stop condition not in progress
                                // Repeated Start condition not in progress
                                // Start condition not in progress
    while(I2C1STATbits.TRSTAT); // Bit = 0 ? Master transmit is not in progress
}

// I2C_start() sends a start condition  
void I2C_start()
{
    RED_LED_ON;//turn on RED user LED to depict failure to generate start condition
    I2C_wait_for_idle();
    I2C1CONbits.SEN = 1;

    while (I2C1CONbits.SEN == 1);
    RED_LED_OFF;
}

// I2C_stop() sends a stop condition  
void I2C_stop()
{   
    YELLOW_LED_ON;
    I2C_wait_for_idle();
    I2C1CONbits.PEN = 1;
    YELLOW_LED_OFF;
}

// I2C_restart() sends a repeated start/restart condition
void I2C_restart()
{
    I2C_wait_for_idle();
    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN == 1);
}

// I2C_ack() sends an ACK condition
void I2C_ack(void)
{
    I2C_wait_for_idle();
    I2C1CONbits.ACKDT = 0; // Set hardware to send ACK bit
    I2C1CONbits.ACKEN = 1; // Send ACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

// I2C_nack() sends a NACK condition
void I2C_nack(void) // Acknowledge Data bit
{
    I2C_wait_for_idle();
    I2C1CONbits.ACKDT = 1; // Set hardware to send NACK bit
    I2C1CONbits.ACKEN = 1; // Send NACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning NACK bit has been sent
}

// address is I2C slave address, set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking  
void I2C_write(unsigned char address, char wait_ack)
{
    YELLOW_LED_ON;
    I2C1TRN = address | 0;				// Send slave address with Read/Write bit cleared
    while (I2C1STATbits.TBF == 1);		// Wait until transmit buffer is empty
    I2C_wait_for_idle();				// Wait until I2C bus is idle
    if (wait_ack) while (I2C1STATbits.ACKSTAT == 1); // Wait until ACK is received  
    YELLOW_LED_OFF;
}

// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_read(unsigned char *value, char ack_nack)
{
    RED_LED_ON;
    I2C1CONbits.RCEN = 1;				// Receive enable
    while (I2C1CONbits.RCEN);			// Wait until RCEN is cleared (automatic)  
    while (!I2C1STATbits.RBF);    		// Wait until Receive Buffer is Full (RBF flag)  
    *value = I2C1RCV;    				// Retrieve value from I2C1RCV
    
    if (!ack_nack)						// Do we need to send an ACK or a NACK?  
        I2C_ack();						// Send ACK  
    else
        I2C_nack();						// Send NACK 
    RED_LED_OFF;
}

// I2C_init() initializes I2C1 at at frequency of [frequency]Hz
void I2C_init(double frequency)
{
    asm volatile("di"); // Disable all interrupts. Don't enable global interrupts before all peripherals are configured.

    //set i2c pins as input
    TRISAbits.TRISA14=1;
    TRISAbits.TRISA15=1;
    
    //set pull up resistors 
    //CNPUAbits.CNPUA14=1;
    //CNPUAbits.CNPUA15=1;
     
    double BRG;
    
    I2C1CON = 0;			// Turn off I2C1 module
    I2C1CONbits.DISSLW = 1; // Disable slew rate for 100kHz
    
    BRG = (1 / (2 * frequency)) - 0.000000104;
    BRG *= (SYS_FREQ / 2) - 2;    
    
    I2C1BRG = (int)BRG;		// Set baud rate
    I2C1CONbits.ON = 1;		// Turn on I2C1 module
    
    //asm volatile("ei"); // Enable Global Interrupts once all peripherals are configured

}


