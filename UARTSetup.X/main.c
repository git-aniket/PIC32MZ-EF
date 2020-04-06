/*main.c
@Author :
Aniket Mazumder

@Company
University of Groningen

@Description
Main file to project to setup UART peripheral


 */
#include "config.h"
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include <proc/p32mz2048efm100.h>
#include "header.h"

#include"UART.h"

/*function prototypes*/
/*prototypes in initialSetup.c*/
void set_performance_mode();
void set_digital();

void main()
{

	set_performance_mode();//sets the peripheral clocks and other startup items
	set_digital();//sets all ports to digital output
	RED_RGB_LED;  

	/* setups for peripherals go here */
	UART_Init();
	//UART_Init_Interrupt();

	// Enable Global Interrupts once all peripherals are configured
	asm volatile("ei"); 

	GREEN_RGB_LED;
	//System ready
	while(1)
	{
		//printf("2\n");
		WriteUART("Hello\n");
		Nop();
	}
}
