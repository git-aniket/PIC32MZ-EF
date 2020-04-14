/*
 * /** UART.c
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 March, 2020
  
 @Company
 Universiy of Groningen
 
  @File Name
 UART.c

  @Summary
 Source file to initialize UART1 to communicate to PC at 38400bps using a circular buffer. Higher speeds don't work reliably.
 This file consists of setup functions for the UART bus1. This bus is being used for
 * send/receive any data to the PC using the serial port. The USB UART click board needs 
 * to be used for proper voltage level conversions. 
 * RPD10 us configured as TX 
 * RPD15 is configured as RX  
 * Timer8 has been set up to send data at regular intervals to data buffer.
 * Functions ReadUART and WriteUART can be used to send/receive data by using a circular buffer called data_buf[BUFLEN]
 * There are also functions for delay and printf that may be utilized in the code.
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
    TRISDbits.TRISD10=0b1; //set the uart RX pin as input
	RPD15R = 0b0001; // RPD15 = U1TX
    TRISDbits.TRISD15=0b0;//set the uart TX pin as output
    
    /**************************************************************************/    
    //Set up baud rates and data stop bit configuration
    //U1MODE = off; // Set UART 1 off prior to setting it up
    U1MODEbits.BRGH =  STANDARD_SPEED_MODE;    
    U1BRG = pbClk2 / (16 * 38400) - 1;// This is the formula straight from the data sheet, max value for UXBRG=2^16-1
    U1STA = clear; // Disable the TX and RX pins, clear all flags
    U1MODEbits.PDSEL = EIGHT_BIT_DATA_NO_PARITY;  // this is the default of 8-bit data, no parity bits that most terminals use
    U1MODEbits.STSEL = ONE_STOP_BIT ; // STSEL controls how many stop bits we use, let's use the default of 1
    U1MODEbits.UEN=2;//configure hardware flow control using RTS and CTS

    /***************************************************************************/
    //Enable UART1
    U1STAbits.UTXEN = onn; // Enable the TX pin
    U1STAbits.URXEN = onn; // Enable the RX pin
    U1MODEbits.ON = onn; // Turn on the UART 1 peripheral
    
	/***************************************************************************/
	//Enable timer8 to send data over circular buffer
	T8CON=0X0;// Disable timer8 
	TMR8=clear;//set timer counter to 0
  	IEC1bits.T8IE=off;// Disable timer8 interrupt
	
	T8CONbits.TCKPS=0b101;//Pre-scaler of 32
	PR8=UART_DATA_FREQUENCY_100;	

	//Enable timer interrupts for timer8
	IFS1bits.T8IF=clear;//clear timer8 interrupt flag
	IPC9bits.T8IP=2;//Interrupt priority=2
	IPC9bits.T8IS=1;//Sub-priority 1
	IEC1bits.T8IE=onn;//Enable timer8 interrupt

    T8CONbits.ON=onn;//switch onn timer 8
}

/*ISR for Timer8; Use this to send whatever data has to sent*/
void __attribute__((vector(_TIMER_8_VECTOR), interrupt(ipl2srs),nomips16)) data_transmit()
{
    static uint16_t i=0;
    if(start)
    {
        //Add whatever data needs to be sent via UART to this section
        buffer_write(i);
        buffer_write(i+10);
        //buffer_write(i+20);
        //buffer_write(i+30);
        ++i;
    }
	IFS1bits.T8IF=clear;//clear timer8 interrupt flag
    
}


/*Read from UART1
 block other functions until you get a '\r' or '\n'
 send the pointer to your char array and the number of elements in the array
 */
void ReadUART(char * message, uint16_t maxLength) 
{
  char data = 0;
  uint8_t complete = 0;
  uint16_t num_bytes = 0;
  
  //loop until you get a '\r' or '\n'
  while (!complete) 
  {
    if (U1STAbits.URXDA) // if data is available
	{ 
      data = U1RXREG;// read the data
      
      if ((data == '\n') || (data == '\r')) 
	  {
        complete = 1;
      }
      
      else 
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


/*Function to write a character array to UART*/
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

/*Function to check if data buffer is empty*/
uint8_t buffer_empty() 
{    // return true if the buffer is empty (read = write)
  return read == write; 
}

/*Function returning the state of buffer*/
uint8_t buffer_full() 
{     // return true if the buffer is full.  
  return (write + 1) % BUFLEN == read; 
}

/*Function to read data written to the data buffer(array)*/
int16_t buffer_read() 
{  
	// reads from current buffer location; assumes buffer not empty
  int16_t val = data_buf[read];
  ++read;               // increments read index
  if(read >= BUFLEN) 
  {
	// wraps the read index around if necessary
    read = 0;
  }
  return val;
}


/*Function to write data to the data buffer(array)*/
void buffer_write(int16_t data) 
{ // add an element to the buffer.  
  if(!buffer_full()) 
  { 
	// if the buffer is full the data is lost
    data_buf[write] = data;
    ++write;                  // increment the write index and wrap around if necessary
    if(write >= BUFLEN) 
	{
      write = 0;
    }
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