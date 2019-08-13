/* ************************************************************************** */
/** the main file for the project
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 
  @Company
 Universiy of Groningen
 

  @File Name
 main.c

  @Summary
 This project configures the PIC32MZEF board to be used as the primary board for
 * the My_LEG project. The peripherals configured in this project include.
 * 3 ADC channels RB2,RB3 and RPB4 with a resolution of 12 bits. These need to be connected to 3.3V only
 * 1 UART channel RPD10 as TX and RPD15 as RX
 * 3 I2C devices on I2C bus1 with speed of 100KHz- address = 0x30,0x40 for encoders and 0x68 for IMU
 * 2 PWM pins at a frequency of 48KHz- Pins RPE8 and RF2, duty cycle between 0(0%) and 4095(100%)
  
 * The ADC's may be used to get analog signal from the Current measurement unit
 * The UART is to transmit data to the PC
 * The I2C bus is to read I2C encoders and IMU for knee and ankle angle
 * The PWM is to drive the Motor( Additional pins need to be configured to run the Motor Driver)
    
 */
/* ************************************************************************** */

#include "config.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <proc/p32mz2048efm100.h>
#include "i2c.h"    

void main()
{
    //variables for IMU
	int16_t  accelX=0,accelY=0,accelZ=0;//Raw values
    int16_t  ub_AccelX=0, ub_AccelY=0, ub_AccelZ=0;
    //variables for PWM
    uint16_t DC1=1000, DC2=2000;
    //variables to store value from the ADC
    uint16_t ADC1=0,ADC2=0,ADC3=0;
    //variable to store angle from encoder
    uint16_t knee_angle=0,ankle_angle=0;
    
    //Set max performance mode
	set_performance_mode();
    //set all ports to digital instead of analog
    set_digital();
    
	
    // Initialize ADC
    ADC_init();
	//Initialize PWM
    PWM_init();
    // Initialize peripheral UART1
	UART_Init();
    // Initialize I2C at 100KHz 
    I2C_init(100000);
	
    
	//Setup IMU parameters
    //set IMU to a sensitivity of 16g
    configIMUSensitivity(IMU_ADDRESS,ACCEL_CONFIG,0b0011000 );
    //set IMU to accelerometer only mode to save power
    disableIMUGyro(IMU_ADDRESS, PWR_MGMT_2,0b00000111 );
    //configureIMU data rate to 1KHz and Lowpass filter of 41Hz with a delay of 11.80 ms
    configAccelDataFilterRate(IMU_ADDRESS,ACCEL_CONFIG2 ,0b00000011);
	
    //Loop
	while (1)
	{       
        //readADC
        getADC(&ADC1,&ADC2,&ADC3);
        
        //readIMU
        IMUReadBytes(IMU_ADDRESS, ACCEL_XOUT_H, &accelX);
        IMUReadBytes(IMU_ADDRESS, ACCEL_YOUT_H, &accelY);
        IMUReadBytes(IMU_ADDRESS, ACCEL_ZOUT_H, &accelZ);
        
        //readEncoders
        //encoderRead(KNEE_ENCODER_ADDRESS, ENCODER_ANGLE_REG, &knee_angle);
        //encoderRead(ANKLE_ENCODER_ADDRESS, ENCODER_ANGLE_REG, &ankle_angle);
        
		//set Duty Cycles
        setDutyCycleM1(DC1);
        setDutyCycleM2(DC2);
        
        
        ub_AccelX=accelX-ACCEL_BIAS_X;
        ub_AccelY=accelY-ACCEL_BIAS_Y;
        ub_AccelZ=accelZ-ACCEL_BIAS_Z;
        
        
        //printf("ADC1= %d ",ADC1);
        //printf("ADC2= %d ",ADC2);
        //printf("ADC3= %d ",ADC3);
        //printf("%d ", ankle_angle);
        //printf("%d ", knee_angle);
        
        printf("%d ",ub_AccelX );
        printf("%d ",ub_AccelY );
        printf("%d\n",ub_AccelZ );
                       
	}
}