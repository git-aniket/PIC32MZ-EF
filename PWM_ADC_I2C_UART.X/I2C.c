/*
 Authors:
 * Aniket Mazumder,R.P Trimmers
 * References:-http://www.aidanmocke.com/blog/2018/11/27/i2c/


 * This file consists of setup functions for the i2c bus1. This bus is being used for
 * reading data from 2 encoders and the MPU9250 IMU. 
 * 
 * This function takes the I2C address and the data register address of the device
 * and is capable of reading or writing to the i2c device bus.
 * 
 * 
 *  
 */


#include "i2c.h"
#include <xc.h>
/*This function checks if the i2c bus is idle or not. If it is busy then it waits for the bus
 to get free before proceeding with the next operation*/
void I2C_wait_for_idle(void)
{
    while(I2C1CON & 0x1F); // Acknowledge sequence not in progress
                                // Receive sequence not in progress
                                // Stop condition not in progress
                                // Repeated Start condition not in progress
                                // Start condition not in progress
    while(I2C1STATbits.TRSTAT); // Bit = 0 ? Master transmit is not in progress
}

// I2C_start() sends a start condition  
void I2C_start()
{
    I2C_wait_for_idle();
    I2C1CONbits.SEN = 1;

    while (I2C1CONbits.SEN == 1);
}

// I2C_stop() sends a stop condition  
void I2C_stop()
{
    I2C_wait_for_idle();
    I2C1CONbits.PEN = 1;
}

// I2C_restart() sends a repeated start/restart condition
void I2C_restart()
{
    I2C_wait_for_idle();
    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN == 1);
}

// I2C_ack() sends an ACK condition
void I2C_ack(void)
{
    I2C_wait_for_idle();
    I2C1CONbits.ACKDT = 0; // Set hardware to send ACK bit
    I2C1CONbits.ACKEN = 1; // Send ACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

// I2C_nack() sends a NACK condition
void I2C_nack(void) // Acknowledge Data bit
{
    I2C_wait_for_idle();
    I2C1CONbits.ACKDT = 1; // Set hardware to send NACK bit
    I2C1CONbits.ACKEN = 1; // Send NACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning NACK bit has been sent
}

// address is I2C slave address, set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking  
void I2C_write(unsigned char address, char wait_ack)
{
    I2C1TRN = address | 0;				// Send slave address with Read/Write bit cleared
    while (I2C1STATbits.TBF == 1);		// Wait until transmit buffer is empty
    I2C_wait_for_idle();				// Wait until I2C bus is idle
    if (wait_ack) while (I2C1STATbits.ACKSTAT == 1); // Wait until ACK is received  
}

// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_read(unsigned char *value, char ack_nack)
{
    I2C1CONbits.RCEN = 1;				// Receive enable
    while (I2C1CONbits.RCEN);			// Wait until RCEN is cleared (automatic)  
    while (!I2C1STATbits.RBF);    		// Wait until Receive Buffer is Full (RBF flag)  
    *value = I2C1RCV;    				// Retrieve value from I2C1RCV
    
    if (!ack_nack)						// Do we need to send an ACK or a NACK?  
        I2C_ack();						// Send ACK  
    else
        I2C_nack();						// Send NACK  
}

// I2C_init() initialises I2C1 at at frequency of [frequency]Hz
void I2C_init(double frequency)
{
    double BRG;
    
    I2C1CON = 0;			// Turn off I2C1 module
    I2C1CONbits.DISSLW = 1; // Disable slew rate for 100kHz
    
    BRG = (1 / (2 * frequency)) - 0.000000104;
    BRG *= (SYS_FREQ / 2) - 2;    
    
    I2C1BRG = (int)BRG;		// Set baud rate
    I2C1CONbits.ON = 1;		// Turn on I2C1 module
}


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

/*Methods for the IMU
 ........................
 *************************/

//configure IMU, ie set sensitivity of the accelerometers
void configIMUSensitivity(uint8_t i2c_address, uint8_t data_address,uint8_t value )
{
    /*
     Reg 28[4:3]- 2g(00),4g(01),8g(10),16g(11)
     * value=0b0001100 for 16g resolution
     */
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address << 1, 1);     /* Send Encoder's  address, read/write bit not set (AD + W) */  
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
    I2C_write(i2c_address << 1, 1);     /* Send Encoder's  address, read/write bit not set (AD + W) */  
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
    I2C_write(i2c_address << 1, 1);     /* Send Encoder's  address, read/write bit not set (AD + W) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_write(value, 1);				/* Send the value to set it to */  
    I2C_stop();    						/* Send stop condition */  
}



// Read 2 bytes to give the accelerationX
void IMUReadBytes(uint8_t i2c_address, uint8_t data_address, int16_t *dataBytes)
{
    I2C_start();						/* Send start condition */  
    I2C_write(i2c_address<< 1, 1);      /* Send Encoder's address, read/write bit not set (AD + R) */  
    I2C_write(data_address, 1);			/* Send the register address (RA) */  
    I2C_restart();						/* Send repeated start condition */
    I2C_write(i2c_address << 1 | 1, 1);	/* Send Encoder's address, read/write bit set (AD + W) */  
    
    //read the i2c bus twice for 2 bytes of data and store in the variable angle
    uint8_t temp=0;                     /* Temporary variable to store a byte of data*/
    I2C_read(&temp, 0);                 /* Read a byte of data from the I2C bus and store it in variable value */  
    *dataBytes=0;
    *dataBytes=temp<<8;
    I2C_read(&temp, 1);                 /* Read next byte of data from the I2C bus */  
    *dataBytes|=temp;                   /* Combine the two reads to get the angle value form the sensor*/
    
    //Stop the sequential read
    I2C_stop();    						/* Send stop condition */  
}
