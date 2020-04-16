/* I2C.h
 
  @Author
 Aniket Mazumder
 Department of Robotics
 a.mazumder@rug.nl
 
  @Company
 Universiy of Groningen
 
  @File Name
 I2C.h

  @Summary
 Header file containing all methods for the i2c communication protocol for PIC32 MZ
 
 @Description
 
 */

#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

#include <xc.h>



/*Methods for the I2C bus*/

void I2C_wait_for_idle(void);// Function constantly performs checks to find if the i2c bus is idle or not
void I2C_start();   // This function sends the start condition for a data transfer to happen using i2c
void I2C_stop(); //This function sends the stop condition to terminate the data transfer using i2c
void I2C_restart(); // Function sends a repeated start or restart condition
void I2C_ack(void); // Sends an acknowledge condition
void I2C_nack(void); // Not acknowledge Data bit
//void i2c_init(double deviceAddress); // Initialize the i2c bus at the given frequency
void I2C_write(unsigned char address, char wait_ack);
// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_read(unsigned char *value, char ack_nack);
// I2C_init() initializes I2C1 at at frequency of [frequency]Hz
void I2C_init(double frequency);


#endif /* _EXAMPLE_FILE_NAME_H */
