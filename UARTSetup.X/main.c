/*main.c
@Author :
Aniket Mazumder

@Company
University of Groningen

@Description
Main file to project to setup UART peripheral both TX and RX. Timer8 interrupt has been used in place of TX/RX interrupt
 * Use USB UART click board to receive data
 * Send enter using serial terminal to start receiving data
 * What ever data needs to be sent has to be written into buffer in timer8 ISR. Check UART.c.
 * Refer to embedded programming by Kevin Lynch for further details
 */

#include "config.h"
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include <proc/p32mz2048efm100.h>
#include "header.h"

#include"UART.h"

//global variables of UART
volatile int16_t data_buf[BUFLEN]; // array that stores the data as 16bit integer
volatile int16_t read = 0, write = 0; // circular buffer indexes
volatile uint8_t start = 0; // set to start recording


/*function prototypes*/
/*prototypes in initialSetup.c*/
void set_performance_mode();
void set_digital();

void main()
{
	set_performance_mode();//sets the peripheral clocks and other startup items
	set_digital();//sets all ports to digital output
    
    char msg[100] = {};// char array to hold the intermediate data to read/write to/from terminal
		
	RED_RGB_LED;  
       
	/* setups for peripherals go here */
	UART_Init();
    
    // Enable Global Interrupts once all peripherals are configured
	asm volatile("ei"); 
    
	ReadUART(msg,sizeof(msg));  // wait for the user to press enter before continuing
   	sprintf(msg, "%s\r\n", "STREAMING"); //add the string "STREAMING" to char array 'msg'
   	WriteUART(msg);// send char array to terminal via UART
  
	start = 1;//start streaming data
  	GREEN_RGB_LED;
	
    //System ready
	while(1)
	{        
        
        
            while(buffer_empty()) 
            {
                ;
            }// wait for data to be in the queue

            sprintf(msg,"%d\r\n", buffer_read());  // read from the buffer, add contents of buffer to char array msg, 
            WriteUART(msg);//send contents of char array over UART to the client/user
   
    
        
	}
}
