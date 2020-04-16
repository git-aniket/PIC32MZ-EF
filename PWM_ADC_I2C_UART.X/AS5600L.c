/* ************************************************************************** */
/** AS5600L.c

  @Company
 University of Groningen

  @File Name
 AS5600L.c

  @Summary
 source file containing defines and function prototypes for Encoder

  @Description
 This file contains the function definition of the functions for the 
 * AS5600L encoder from AMS system.
 /* ************************************************************************** */


#include <xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>
#include "I2C.h"

/*Methods for the Encoders
 ........................
 *************************/
// Write byte value to register at reg_address
/* This method is used to read a perform a single byte write sequence from master to slave*/
void encoderWrite(uint8_t i2c_address, uint8_t data_address,uint8_t value)
{
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address << 1, 1);     /* Send Encoder's  address, read/write bit not set (AD + W) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_write(value, 1);				/* Send the value to set it to */  
    I2C_stop();    						/* Send stop condition */  
}

// Read two bytes from register at data_address and return in *angle
/* This method is used to perform a 2 byte read sequence form the slave to the master*/
void encoderRead(uint8_t i2c_address, uint8_t data_address, uint16_t *angle)
{
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address<< 1, 1);      /* Send Encoder's address, read/write bit not set (AD + R) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_restart();						/* Send repeated start condition */
    I2C_write(i2c_address << 1 | 1, 1);	/* Send Encoder's address, read/write bit set (AD + W) */  
    
    //read the i2c bus twice for 2 bytes of data and store in the variable angle
    uint8_t temp=0;                     /* Temporary variable to store a byte of data*/
    I2C_read(&temp, 0);                 /* Read a byte of data from the I2C bus and store it in variable value */  
    *angle=0;
    *angle=temp<<8;
    I2C_read(&temp, 1);                 /* Read next byte of data from the I2C bus */  
    *angle|=temp;                       /* Combine the two reads to get the angle value form the sensor*/
    
    //Stop the sequential read
    I2C_stop();    						/* Send stop condition */  
}


