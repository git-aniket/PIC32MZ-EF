// PIC32MZ_4.c - How to set up the UART via PPS on the PIC32MZ
// by Aidan Mocke 2018-08-29
//
// PIC32MZ2048EFH144 Configuration Bit Settings

// DEVCFG3
// USERID = No Setting

#include "config.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <proc/p32mz2048efm100.h>

#define SYS_FREQ 200000000 // Running at 200MHz

void set_performance_mode()
{   
	unsigned int cp0;
	
    // Unlock Sequence
    asm volatile("di"); // Disable all interrupts
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;  

    // PB1DIV
    // Peripheral Bus 1 cannot be turned off, so there's no need to turn it on
    PB1DIVbits.PBDIV = 1; // Peripheral Bus 1 Clock Divisor Control (PBCLK1 is SYSCLK divided by 2)

    // PB2DIV
    PB2DIVbits.ON = 1; // Peripheral Bus 2 Output Clock Enable (Output clock is enabled)
    PB2DIVbits.PBDIV = 1; // Peripheral Bus 2 Clock Divisor Control (PBCLK2 is SYSCLK divided by 2)

    // PB3DIV
    PB3DIVbits.ON = 1; // Peripheral Bus 2 Output Clock Enable (Output clock is enabled)
    PB3DIVbits.PBDIV = 1; // Peripheral Bus 3 Clock Divisor Control (PBCLK3 is SYSCLK divided by 2)

    // PB4DIV
    PB4DIVbits.ON = 1; // Peripheral Bus 4 Output Clock Enable (Output clock is enabled)
    while (!PB4DIVbits.PBDIVRDY); // Wait until it is ready to write to
    PB4DIVbits.PBDIV = 0; // Peripheral Bus 4 Clock Divisor Control (PBCLK4 is SYSCLK divided by 1)

    // PB5DIV
    PB5DIVbits.ON = 1; // Peripheral Bus 5 Output Clock Enable (Output clock is enabled)
    PB5DIVbits.PBDIV = 1; // Peripheral Bus 5 Clock Divisor Control (PBCLK5 is SYSCLK divided by 2)

    // PB7DIV
    PB7DIVbits.ON = 1; // Peripheral Bus 7 Output Clock Enable (Output clock is enabled)
    PB7DIVbits.PBDIV = 0; // Peripheral Bus 7 Clock Divisor Control (PBCLK7 is SYSCLK divided by 1)

    // PB8DIV
    PB8DIVbits.ON = 1; // Peripheral Bus 8 Output Clock Enable (Output clock is enabled)
    PB8DIVbits.PBDIV = 1; // Peripheral Bus 8 Clock Divisor Control (PBCLK8 is SYSCLK divided by 2)

    // PRECON - Set up prefetch
    PRECONbits.PFMSECEN = 0; // Flash SEC Interrupt Enable (Do not generate an interrupt when the PFMSEC bit is set)
    PRECONbits.PREFEN = 0b11; // Predictive Prefetch Enable (Enable predictive prefetch for any address)
    PRECONbits.PFMWS = 0b010; // PFM Access Time Defined in Terms of SYSCLK Wait States (Two wait states)

    // Set up caching
    cp0 = _mfc0(16, 0);
    cp0 &= ~0x07;
    cp0 |= 0b011; // K0 = Cacheable, non-coherent, write-back, write allocate
    _mtc0(16, 0, cp0);  

    // Lock Sequence
    SYSKEY = 0x33333333;
    asm volatile("ei"); // Enable all interrupts
}

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

// Initialise peripheral UART 5
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
}

// Overwrite _mon_putc() to enable us to use printf
void _mon_putc (char c)
 {
   while (U5STAbits.UTXBF); // Wait til current transmission is complete
   U5TXREG = c;
}


void main()
{
	int cnt;
	
	// Set up all ports to be digital and not analog
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;
   
	
	
	// Set up Peripheral Pin Select for UART 1
    U1RXR = 0b0011; // RF1 = U5RX
	RPD15R = 0b0001; // RF0 = U5TX
	TRISFbits.TRISF1 = 1; // Make RF1 an input
	
	// Set max performance mode
	set_performance_mode();
	
	// Initialise peripheral UART 5
	UART_Init();
	
	cnt = 0;
	
	// Main loop
	while (1)
	{
		printf("Counter value is %d\n", cnt);
		delay_ms(1000); // Delay 1 second
		cnt++;
	}
}