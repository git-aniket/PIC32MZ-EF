/*
 * /** UART.c
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 
  @Company
 Universiy of Groningen
 
  @File Name
 UART.c

  @Summary
 Source file to initialize UART1 to communicate to PC at 38400 bps. Higher speeds don't work reliably.
 This file consists of setup functions for the UART bus1. This bus is being used for
 * send any data to the PC using the serial port. The USB UART click board needs 
 * to be used for proper voltage level conversions. 
 * RPD10 us configured as TX 
 * RPD15 is configured as RX  
 * There are also functions for delay and printf that may be utilized in the code.
 * 
 * Check section 21 UART of the data sheet for more details
 * https://microchipdeveloper.com/32bit:mz-osc-sysclk   
 
  @Issues
 * Baud Rate higher than 38400 seems unstable.
 */


#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>
#include "UART.h"
#include <stdio.h>

/* Function to initialize peripheral UART 1 */
void UART_Init()
{
    /**************************************************************************/
    uint32_t pbClk2;//variable to store the value of clock speed used by UART module
    pbClk2 = SYS_FREQ / 2; // Our PBCLK2 divider was set to 1, so PBCLK2 is exactly half the speed of the system clock, or 100Mhz
    
    /*************************************************************************/
	// Set up Peripheral Pin Select for UART 1
    U1RXR = 0b0011; // RPD10 = U1RX
	RPD15R = 0b0001; // RPD15 = U1TX
    
    /**************************************************************************/    
    //Set up baud rates and data stop bit configuration
    U1MODE = off; // Set UART 1 off prior to setting it up
    U1MODEbits.BRGH =  STANDARD_SPEED_MODE;    
    U1BRG = pbClk2 / (16 * 38400) - 1;// This is the formula straight from the data sheet, max value for UXBRG=2^16-1
    U1STA = clear; // Disable the TX and RX pins, clear all flags
    U1MODEbits.PDSEL = EIGHT_BIT_DATA_NO_PARITY;  // this is the default of 8-bit data, no parity bits that most terminals use
    U1MODEbits.STSEL = ONE_STOP_BIT ; // STSEL controls how many stop bits we use, let's use the default of 1


    /***************************************************************************/
    //Enable UART1
    U1STAbits.UTXEN = onn; // Enable the TX pin
    U1STAbits.URXEN = onn; // Enable the RX pin
    U1MODEbits.ON = onn; // Turn on the UART 1 peripheral
    
    
}

// Read from UART1
// block other functions until you get a '\r' or '\n'
// send the pointer to your char array and the number of elements in the array
void ReadUART(char * message, int maxLength) 
{
  char data = 0;
  int complete = 0, num_bytes = 0;
  // loop until you get a '\r' or '\n'
  while (!complete) 
  {
    if (U1STAbits.URXDA) 
	{ // if data is available
      data = U1RXREG;      // read the data
      if ((data == '\n') || (data == '\r')) 
	  {
        complete = 1;
      } else 
	  {
        message[num_bytes] = data;
        ++num_bytes;
        // roll over if the array is too small
        if (num_bytes >= maxLength) 
		{
          num_bytes = 0;
        }
      }
    }
  }
  // end the string
  message[num_bytes] = '\0';
}


// Write a character array using UART
void WriteUART(const char * string)
{
  while (*string != '\0') 
  {
    while (U1STAbits.UTXBF) 
	{
      ; // wait until tx buffer isn't full
    }
    U1TXREG = *string;
    ++string;
  }
}






/* Function to enable use of printf instead of _mon_putc()*/
void _mon_putc (char c)
{
   while (U1STAbits.UTXBF); // Wait till current transmission is complete
   U1TXREG = c;
}

/*Delay functions

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

/*function to reset value of core timer to a desired value*/
void setTicks(uint32_t value)
{
    _CP0_SET_COUNT(value);
}

/*function to get time elapsed since last reset*/
void getTicks(double *dt)
{
    uint32_t tickvalue =_CP0_GET_COUNT();
    *dt=(double)tickvalue*0.00000001;
}


/*Initlaize UART using interrupts***********************************
  *****************************************************/

void UART_Init_Interrupt()
{
    /**************************************************************************/
    uint32_t pbClk2;//variable to store the value of clock speed used by UART module
    pbClk2 = SYS_FREQ / 2; // Our PBCLK2 divider was set to 1, so PBCLK2 is exactly half the speed of the system clock, or 100Mhz
    
    /*************************************************************************/
	// Set up Peripheral Pin Select for UART 1
    U1RXR = 0b0011; // RPD10 = U1RX
	RPD15R = 0b0001; // RPD15 = U1TX
    
    /**************************************************************************/    
    //Set up baud rates and data stop bit configuration
    U1MODE = off; // Set UART 1 off prior to setting it up
    U1MODEbits.BRGH =  STANDARD_SPEED_MODE;    
    U1BRG = pbClk2 / (16 * 38400) - 1;// This is the formula straight from the data sheet, max value for UXBRG=2^16-1
    U1STA = clear; // Disable the TX and RX pins, clear all flags
    U1MODEbits.PDSEL = EIGHT_BIT_DATA_NO_PARITY;  // this is the default of 8-bit data, no parity bits that most terminals use
    U1MODEbits.STSEL = ONE_STOP_BIT ; // STSEL controls how many stop bits we use, let's use the default of 1

    /*************************************************************************/
    
    //setup interrupts for the UART
    IEC3bits.U1TXIE=onn;//Enable Transmit interrupt
    IFS3bits.U1TXIF=clear;//Clear Interrupt flag for TX interrupt
    IPC28bits.U1TXIP=2;//interrupt priority 2
    IPC28bits.U1TXIS=1;
    U1STAbits.URXISEL=ALL_CHARACTERS_TRANSMITTED_INTERRUPT ;//set up interrupt mode
    

    /***************************************************************************/
    //Enable UART1
    U1STAbits.UTXEN = onn; // Enable the TX pin
    U1STAbits.URXEN = onn; // Enable the RX pin
    U1MODEbits.ON = onn; // Turn on the UART 1 peripheral
    
    
}


/* Ths function is buggy as only the first character is received in the 
termnal in the when this WriteUART_ISR is used hence both need modifications**********/

//UART Interrupt ISR
void __attribute__((vector(_UART1_TX_VECTOR), interrupt(ipl2srs), nomips16)) uart_tx_interrupt()
{
    //int a=15;
    char arr[]="Hello\n";
	if(IFS3bits.U1TXIF)
    {
        IFS3bits.U1TXIF=clear;//clear the interrupt flag
		WriteUART_ISR(arr);
        //U1TXREG = a;
    }
        
    //If it's a RX interrupt
    else if(IFS3bits.U1RXIF)
	{//do nothing incase it is a RX interrupt 
    	IFS3bits.U1TXIF=clear;//clear the interrupt flag
    }
    
    //If it's an error interrupt
    else if(IFS3bits.U1EIF)
	{//incase it is an error interrupt
   		IFS3bits.U1TXIF=clear;//clear the interrupt flag
    }
}



//This function too might need debugging to get use UART_Init_interrupt function

// Write a character array using UART1 with ISR
void WriteUART_ISR(const char * string)
{
  while (*string != '\0') 
  {
	/*
    while (U1STAbits.UTXBF) 
	{
      ; // wait until tx buffer isn't full
    }
	*/
    U1TXREG = *string;
    ++string;
  }
}



