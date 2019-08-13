/* ************************************************************************** */
/*
  @Company
 Universiy of Groningen

  @File Name
    i2c.h

  @Summary
    header files for the i2c driver and devices.

  @Description
    file contains defines and function prototypes for the i2c driver and functions utilized 
    in calling getting the data from the encoder and the IMU.
 * 
 * **********************
 * IMU details:
 * IMU has address of 0x68 when AD0 pin is connected to ground. In this software
 * the gyroscope of the IMU has been disabled to conserve power. The accelerometer has
 * been set at a max reading of 16G and sensitivity scale factor of 2048 LSB/g.
 * A low pass filter of frequency  41Hz with a delay of 11.80 ms and a sampling rate of 1 KHz is used.
 * 
 ******************************************************************************
 * Encoder details:
 * 2 encoders have been used, one each at the knee and ankle. The encoders work
 * on I2C and have addresses configured as 0x30 and 0x40 respectively.
 * No filtering of the data has been done, the data has been read from the angle
 * register 0x0E as mentioned in the data sheet.
/* ************************************************************************** */

#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H
#include "header.h"


//Defines for the encoders
#define KNEE_ENCODER_ADDRESS 0x30     // The address of knee Encoder on the I2C bus
#define ANKLE_ENCODER_ADDRESS 0x40      //The address of the ankle Encoder on tht I2c bus
#define ENCODER_ANGLE_REG  0x0E  // Register that stores the angle data in the encoder


//Defines for the IMU 9250
//Magnetometer Registers
// #define AK8963_ADDRESS   0x0C
#define IMU_ADDRESS      0x68

/*define Accelerometer biases
 * Calculate the biases from the accelerometer and put these values here*/
//Biases for the Given MPU 9150(Marked with tape))
#define ACCEL_BIAS_X    50
#define ACCEL_BIAS_Y    30
#define ACCEL_BIAS_Z    80

#define AK8963_WHO_AM_I  0x00 // should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L	 0x03  // data
#define AK8963_XOUT_H	 0x04
#define AK8963_YOUT_L	 0x05
#define AK8963_YOUT_H	 0x06
#define AK8963_ZOUT_L	 0x07
#define AK8963_ZOUT_H	 0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

// #define X_FINE_GAIN      0x03 // [7:0] fine gain
// #define Y_FINE_GAIN      0x04
// #define Z_FINE_GAIN      0x05
// #define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
// #define XA_OFFSET_L_TC   0x07
// #define YA_OFFSET_H      0x08
// #define YA_OFFSET_L_TC   0x09
// #define ZA_OFFSET_H      0x0A
// #define ZA_OFFSET_L_TC   0x0B

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A      0x10

#define XG_OFFSET_H      0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L      0x14
#define YG_OFFSET_H      0x15
#define YG_OFFSET_L      0x16
#define ZG_OFFSET_H      0x17
#define ZG_OFFSET_L      0x18

#define SMPLRT_DIV       0x19
#define MPU_CONFIG           0x1A// renamed to keep unique configuration for MPU
//registers to set the sensitivity of the accelerometer and gyro
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C


#define ACCEL_CONFIG2    0x1D
#define LP_ACCEL_ODR     0x1E
#define WOM_THR          0x1F

#define MOT_DUR          0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define ZMOT_THR         0x21  // Zero-motion detection threshold bits [7:0]
#define ZRMOT_DUR        0x22  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms

#define FIFO_EN          0x23
#define I2C_MST_CTRL     0x24
#define I2C_SLV0_ADDR    0x25
#define I2C_SLV0_REG     0x26
#define I2C_SLV0_CTRL    0x27
#define I2C_SLV1_ADDR    0x28
#define I2C_SLV1_REG     0x29
#define I2C_SLV1_CTRL    0x2A
#define I2C_SLV2_ADDR    0x2B
#define I2C_SLV2_REG     0x2C
#define I2C_SLV2_CTRL    0x2D
#define I2C_SLV3_ADDR    0x2E
#define I2C_SLV3_REG     0x2F
#define I2C_SLV3_CTRL    0x30
#define I2C_SLV4_ADDR    0x31
#define I2C_SLV4_REG     0x32
#define I2C_SLV4_DO      0x33
#define I2C_SLV4_CTRL    0x34
#define I2C_SLV4_DI      0x35
#define I2C_MST_STATUS   0x36
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define DMP_INT_STATUS   0x39  // Check DMP interrupt
#define INT_STATUS       0x3A

//data registers used to capture data
#define ACCEL_XOUT_H     0x3B
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41
#define TEMP_OUT_L       0x42
#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48


#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS 0x61
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL  0x69
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP

//Set power management registers of the IMU to sleep the features not required
#define PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2       0x6C

#define DMP_BANK         0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT       0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG          0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1        0x70
#define DMP_REG_2        0x71
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71

// Accelerometer Offset Registers
#define XA_OFFSET_H      0x77
#define XA_OFFSET_L      0x78
#define YA_OFFSET_H      0x7A
#define YA_OFFSET_L      0x7B
#define ZA_OFFSET_H      0x7D
#define ZA_OFFSET_L      0x7E


/*Methods for the I2C bus*/

void I2C_wait_for_idle(void);// Function constantly performs checks to find if the i2c bus is idle or not
void I2C_start();   // This function sends the start condition for a data transfer to happen using i2c
void I2C_stop(); //This function sends the stop condition to terminate the data transfer using i2c
void I2C_restart(); // Function sends a repeated start or restart condition
void I2C_ack(void); // Sends an acknowledge condition
void I2C_nack(void); // Not acknowledge Data bit
void i2c_init(uint16_t deviceAddress); // Initialize the i2c bus at the given frequency
void I2C_write(unsigned char address, char wait_ack);
// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_read(unsigned char *value, char ack_nack);
// I2C_init() initialises I2C1 at at frequency of [frequency]Hz
void I2C_init(double frequency);


/* Methods for the Encoders*
 * ****************************************************************
 * ............*/
// Write byte value to register at data_address
void encoderWrite(uint8_t i2c_address, uint8_t data_address,uint8_t value);
// Read 2 bytes from register at data_address and return in *angle
void encoderRead(uint8_t i2c_address, uint8_t data_address, uint16_t *angle);


/*Methods for the IMU MPU9250
 *******************************************************************
 .................*/
/*configure  IMU sensitivity
* value=0b0001100 for 16g resolution
*/   
void configIMUSensitivity(uint8_t i2c_address, uint8_t data_address,uint8_t value );


/*configure  IMU powermode
* value=0b00000111 to keep only the accelerometers active.
*/   
void disableIMUGyro(uint8_t i2c_address, uint8_t data_address,uint8_t value );


/*configure  Accelerometer datarate and the Filter to the 
* value=0b00000010 to set a Lowpass filter of 92Hz bandwidth and data rate of 1KHz. delay of 7.8 ms and Noise density of 250ug/rtHz
*/      
void configAccelDataFilterRate(uint8_t i2c_address, uint8_t data_address,uint8_t value );


/*Function to read a couple of bytes from the IMU starting at the address "data_address"
 * data_address=ACCEL_XOUT_H for X acceleration
 *              ACCEL_YOUT_H for Y acceleration
 *              ACCEL_ZOUT_H for Z acceleration
 *              GYRO_XOUT_H for Gyro X
 *              GYRO_YOUT_H for Gyro Y
 *              GYRO_ZOUT_H for Gyro Z
 */  
void IMUReadBytes(uint8_t i2c_address, uint8_t data_address, int16_t *dataBytes);


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
