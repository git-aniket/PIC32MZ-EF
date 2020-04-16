/** mpu9250.c

  @Company
 University of Groningen

  @File Name
 mpu9250.c

  @Summary
 Source file for the MPU9250 IMU
  
  @Description
 File contains the function definitions for the IMU 9250 
 
/* **************************************************************************

 
/*Methods for the IMU
 ........................
 *************************/


#include<xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>
#include "mpu9250.h"

//configure IMU, ie set sensitivity of the accelerometers
void configIMUSensitivity(uint8_t i2c_address, uint8_t data_address,uint8_t value )
{
    /*
     Reg 28[4:3]- 2g(00),4g(01),8g(10),16g(11)
     * value=0b0001100 for 16g resolution
     */
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address << 1, 1);     /* Send IMU's  address, read/write bit not set (AD + W) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_write(value, 1);				/* Send the value to set it to */  
    I2C_stop();    						/* Send stop condition */  
}
//configure IMU power, ie disable the Gyroscope to save power
void disableIMUGyro(uint8_t i2c_address, uint8_t data_address,uint8_t value )
{
    /*configure  IMU powermode
    * value=0b00000111 to keep only the accelerometers active.
     */      
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address << 1, 1);     /* Send IMU's  address, read/write bit not set (AD + W) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_write(value, 1);				/* Send the value to set it to */  
    I2C_stop();    						/* Send stop condition */  
}

//configure Accelerometer data rate and data filter, 
void configAccelDataFilterRate(uint8_t i2c_address, uint8_t data_address,uint8_t value )
{
    /*configure Accelerometer datarate and the Filter to the 
    * value=0b00000010 to set a Lowpass filter of 92Hz bandwidth and data rate of 1KHz. delay of 7.8 ms and Noise density of 250ug/rtHz
     */      
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address << 1, 1);     /* Send IMU's  address, read/write bit not set (AD + W) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_write(value, 1);				/* Send the value to set it to */  
    I2C_stop();    						/* Send stop condition */  
}



// Read 2 bytes to give the accelerationX
void IMUReadBytes(uint8_t i2c_address, uint8_t data_address,int16_t bias, volatile int16_t *dataBytes)
{
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address<< 1, 1);      /* Send IMU's address, read/write bit not set (AD + R) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_restart();						/* Send repeated start condition */
    I2C_write(i2c_address << 1 | 1, 1);	/* Send IMU's address, read/write bit set (AD + W) */  
    
    //read the i2c bus twice for 2 bytes of data and store in the variable angle
    uint8_t temp=0;                     /* Temporary variable to store a byte of data*/
    I2C_read(&temp, 0);                 /* Read a byte of data from the I2C bus and store it in variable value */  
    *dataBytes=0;
    *dataBytes=temp<<8;
    I2C_read(&temp, 1);                 /* Read next byte of data from the I2C bus */  
    *dataBytes|=temp;                   /* Combine the two reads to get the angle value form the sensor*/
    *dataBytes=*dataBytes-bias;
    //Stop the sequential read
    I2C_stop();    						/* Send stop condition */  
}

//setup IMU sensitivity, power mode and data filtration rate
void setIMU_sensitivity()
{
    //set IMU to a sensitivity of 16g
    configIMUSensitivity(IMU_ADDRESS,ACCEL_CONFIG,0b0011000 );
    //set IMU to accelerometer only mode to save power
    disableIMUGyro(IMU_ADDRESS, PWR_MGMT_2,0b00000111 );
    //configureIMU data rate to 1KHz and Low pass filter of 41Hz with a delay of 11.80 ms
    configAccelDataFilterRate(IMU_ADDRESS,ACCEL_CONFIG2 ,0b00000011);
}