
/* ************************************************************************** */
/**UART.h 

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
 * UART.h

@Summary
 Header file for to set UART1 peripheral. 

@Description
 This file sets up the function prototypes for the UART along with the delay functions for the PIC.
/***************************************************************************************/
#ifndef _UART_H    
#define _UART_H


/*Defines for interrupt types*/
#define WHILE_TRANSMIT_BUFFER_EMPTY_INTERRUPT 0b10
#define ALL_CHARACTERS_TRANSMITTED_INTERRUPT 0b01
#define TRANSMIT_BUFFER_ATLEAST_ONE_EMPTY_INTERRUPT 0b00

/*Speed Modes*/
#define STANDARD_SPEED_MODE 0b0
#define HIGH_SPEED_MODE 0b1

/*Parity and data selection bits*/
#define EIGHT_BIT_DATA_NO_PARITY 0b00
#define EIGHT_BIT_DATA_EVEN_PARITY 0b01
#define EIGHT_BIT_DATA_ODD_PARITY 0b10
#define NINE_BIT_DATA_NO_PARITY 0b11

#define ONE_STOP_BIT 0b0
#define TWO_STOP_BITS 0b1

#define BUFLEN   1024// length of the buffer

extern volatile int16_t data_buf[BUFLEN];// array that stores the data
extern volatile int16_t read , write ; // circular buf indexes
extern volatile uint8_t start;// set to start recording

/*prototypes in UART.c*/
void UART_Init();//Function enables UART and timer 8 to send data using circular buffer ISR
void ReadUART(char *, uint16_t);
void WriteUART(const char *);
uint8_t buffer_empty();
uint8_t buffer_full();
int16_t buffer_read();
void buffer_write(int16_t data); 

void _mon_putc (char c);
void delay_us(unsigned int us);
void delay_ms(int ms);
void setTicks(uint32_t value);
void getTicks(double *dt);

#endif 

