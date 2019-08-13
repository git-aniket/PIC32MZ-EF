/*
 Authors:
 * Aniket Mazumder,R.P Trimmers, V.Raveendranathan
 * References:-http://www.aidanmocke.com/blog/2018/11/27/i2c/


 * This file consists of setup functions for the UART bus1. This bus is being used for
 * send any data to the PC using the serial port. The USB UART click board needs 
 * to be used for proper voltage level conversions. 
 * RPD10 us configured as TX 
 * RPD15 is configured as RX  
 */


#include <xc.h>
#include "header.h"
// Initialise peripheral UART 1
void UART_Init()
{
    int pbClk;
    
    pbClk = SYS_FREQ / 2; // Our PBCLK2 divider was set to 1, so PBCLK2 is exactly half the speed of the system clock, or 100Mhz
        
    U1MODE = 0; // Set UART 5 off prior to setting it up
    U1MODEbits.BRGH = 0; // We want standard speed mode. Not necessary as we just set U5MODE to 0 so this is just for explanation's sake
    U1BRG = pbClk / (16 * 38400) - 1;// This is the formula straight from the datasheet
    U1STA = 0; // Disable the TX and RX pins, clear all flags
    U1STAbits.UTXEN = 1; // Enable the TX pin
    U1STAbits.URXEN = 1; // Enable the RX pin
    U1MODEbits.PDSEL = 0; // PDSEL controls how many data bits and how many parity bits we want, this is the default of 8-bit data, no parity bits that most terminals use
    U1MODEbits.STSEL = 0; // STSEL controls how many stop bits we use, let's use the default of 1
    U1MODEbits.ON = 1; // Turn on the UART 5 peripheral
   
	// Set up Peripheral Pin Select for UART 1
    U1RXR = 0b0011; // RF1 = U5RX
	RPD15R = 0b0001; // RF0 = U5TX
    
    // Set up PPS for UART 5
	//U5RXR = 0b0011; // RF1 = U1RX
	//RPD15R = 0b0011; // RF0 = U1TX
}

// Overwrite _mon_putc() to enable us to use printf
void _mon_putc (char c)
 {
   while (U1STAbits.UTXBF); // Wait til current transmission is complete
   U1TXREG = c;
}

/*Delay functions
  ********************************************
  ********************************************
  *********************************************/
void delay_us(unsigned int us)
{
    // Convert microseconds us into how many clock ticks it will take
	us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks
       
    _CP0_SET_COUNT(0); // Set Core Timer count to 0
    
    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void delay_ms(int ms)
{
    delay_us(ms * 1000);
}