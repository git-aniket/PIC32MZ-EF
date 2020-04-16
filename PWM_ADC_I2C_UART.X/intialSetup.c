/** Descriptive File Name
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 
  @Company
 Universiy of Groningen
 
  @File Name
 InitialSetup.c

  @Summary
 Source file to initialize the microcontroller at 200MHz and set all pins as digital outputs with low.
 The set_performance_mode and set_digital functions needs to be called at the beginning of the main loop in the program above any other loop.
 
 * Check the following website for more details:
 * https://microchipdeveloper.com/32bit:mz-osc-sysclk   
 */



#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>

void set_performance_mode()
{   
	unsigned int cp0;
	
    // Unlock Sequence
    asm volatile("di"); // Disable all interrupts. Don't enable global interrupts before all peripherals are configured.
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;  

    // PB1DIV... Controls WDT, RTC, CFG, PPS, DMT
    // Peripheral Bus 1 cannot be turned off, so there's no need to turn it on
    PB1DIVbits.PBDIV = 1; // Peripheral Bus 1 Clock Divisor Control (PBCLK1 is SYSCLK divided by 2)

    // PB2DIV....Controls I2C,UART, SPI
    PB2DIVbits.ON = 1; // Peripheral Bus 2 Output Clock Enable (Output clock is enabled)
    PB2DIVbits.PBDIV = 1; // Peripheral Bus 2 Clock Divisor Control (PBCLK2 is SYSCLK divided by 2)

    // PB3DIV... Controls ADC, Timers, OC, IC,Comparators
    PB3DIVbits.ON = 1; // Peripheral Bus 3 Output Clock Enable (Output clock is enabled)
    PB3DIVbits.PBDIV = 1; // Peripheral Bus 3 Clock Divisor Control (PBCLK3 is SYSCLK divided by 2)

    // PB4DIV... Controls the Digital IO ports A through to K
    PB4DIVbits.ON = 1; // Peripheral Bus 4 Output Clock Enable (Output clock is enabled)
    while (!PB4DIVbits.PBDIVRDY); // Wait until it is ready to write to
    PB4DIVbits.PBDIV = 1; // Peripheral Bus 4 Clock Divisor Control (PBCLK4 is SYSCLK divided by 2)

    // PB5DIV...Controls Ethernet, CAN and HS USB
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
    
    //enable multi vectored interrupt mode
    INTCONbits.MVEC = 1;
    
    // Configure SRS Priority Selection
    PRISSbits.SS0 = 0;
    PRISSbits.PRI1SS = 1;
    PRISSbits.PRI2SS = 2;
    PRISSbits.PRI3SS = 3;
    PRISSbits.PRI4SS = 4;
    PRISSbits.PRI5SS = 5;
    PRISSbits.PRI6SS = 6;
    PRISSbits.PRI7SS = 7;
    
    //asm volatile("ei"); // Enable Global Interrupts once all peripherals are configured

}
void set_digital()
{
    // Set up all ports to be digital and not analog
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;
    
    // Make all ports outputs 
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISF = 0;
    TRISG = 0;
        
    
    // Set all port values low 
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;
       
     	
}
