/* ************************************************************************** */
/** AS5600L.h

  @Company
 University of Groningen

  @File Name
 AS5600L.h

  @Summary
 header file containing defines and function prototypes for Encoder

  @Description
 This file contains the defines the and the functions prototypes for the 
 * AS5600L encoder from AMS system.
 /* ************************************************************************** */

#ifndef _AS5600L_H    /* Guard against multiple inclusion */
#define _AS5600L_H

//Defines for the encoders
#define KNEE_ENCODER_ADDRESS 0x30     // The address of knee Encoder on the I2C bus
#define ANKLE_ENCODER_ADDRESS 0x40      //The address of the ankle Encoder on tht I2c bus
#define ENCODER_ANGLE_REG  0x0E  // Register that stores the angle data in the encoder


/* Methods for the Encoders*
 * ****************************************************************
 * ............*/
// Write byte value to register at data_address
void encoderWrite(uint8_t i2c_address, uint8_t data_address,uint8_t value);
// Read 2 bytes from register at data_address and return in *angle
void encoderRead(uint8_t i2c_address, uint8_t data_address, uint16_t *angle);


#endif 

