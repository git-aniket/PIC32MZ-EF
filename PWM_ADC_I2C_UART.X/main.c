/*
 /** main.c
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 March, 2020
  @Company
 University of Groningen
 
  @File Name
 main.c

  @Summary
 This file is the main file for the PIC32 software.
 It does the following:
 * Sets up 20KHz PWM at RE8 and RF2
 * Control loops of 200 and 2000Hz at RD9 and RD12
 * UART1 TX at RD10
 * UART1 RX at RD15
 * ADC at RB2, RB3 and RPB4
 * I2C1 for IMU and encoder
*/

/***********************************include files***************************/
#include "config.h"
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include <proc/p32mz2048efm100.h>
#include "header.h"

#include"I2C.h"
#include"mpu9250.h"
#include"AS5600L.h"

#include"UART.h"


/*************************************global variables***********************/
 /*variables that are shared and used in the ISR's*/
 volatile uint8_t flag_ankle_IMU=0,flag_ankle_encoder=0,flag_ankle_current=0,flag_print=0;
 /*variables to store value from ADC*/
 volatile uint16_t  ADC1=0,ADC2=0,ADC3=0;
 /*variables to store the accelerometer values*/
 volatile int16_t  accelX=0,accelY=0,accelZ=0;

 //global variables of UART
volatile int16_t data_buf[BUFLEN]; // array that stores the data as 16bit integer
volatile int16_t read = 0, write = 0; // circular buffer indexes
volatile uint8_t start = 0; // set to start recording

/**************************************function prototypes********************/

/*****************************************main.c*********************************/
int main()
{
    
    /*variables controlling duty cycle*/
    uint16_t dc1=100, dc2=100;
    
    set_performance_mode();//sets peripheral clock frequencies and disables interrupts
    set_digital();//sets all ports to digital output
    
    char msg[100] = {};// char array to hold the intermediate data to read/write to/from terminal

    RED_RGB_LED; 
    
    /* setups for peripherals go here */
    Motor_driver_init();
    ADC_init();
    UART_Init();
    I2C_init(100000);// initialize i2c at 100KHz
    
    setIMU_sensitivity();//choose IMU sensitivity, power mode and data filtration rate
    
    ReadUART(msg,sizeof(msg));  // wait for the user to press enter before continuing
   	sprintf(msg, "%s\r\n", "STREAMING"); //add the string "STREAMING" to char array 'msg'
   	WriteUART(msg);// send char array to terminal via UART
    
	start = 1;//start streaming data
      
    
     
    asm volatile("ei"); // Enable Global Interrupts once all peripherals are configured
    GREEN_RGB_LED;//system ready

    
    while(1)
    {
        set_dutycycleM1(dc1);
        set_dutycycleM2(dc2);
        
        //ReadIMU
        //IMUReadBytes(IMU_ADDRESS, ACCEL_XOUT_H,ACCEL_BIAS_X, &accelX);
        //IMUReadBytes(IMU_ADDRESS, ACCEL_YOUT_H,ACCEL_BIAS_Y, &accelY);
        //IMUReadBytes(IMU_ADDRESS, ACCEL_ZOUT_H,ACCEL_BIAS_Z, &accelZ);
                  
        //Nop();
                      
        while(buffer_empty()) 
        { ;
        }// wait for data to be in the queue
        
        // read from the buffer, add contents of buffer to char array 'msg',
        sprintf(msg,"%d\r\n", buffer_read());   
        //send contents of char array over UART to the client/user
        WriteUART(msg);
                
    }
}
