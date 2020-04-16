/* ************************************************************************** */
h/** motorDriver.c

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
motorDriver.h

@Summary
 Function prototypes for motordriver.c 

#ifndef _MOTOR_DRIVER_H    /* Guard against multiple inclusion */
#define _MOTOR_DRIVER_H

/*prototypes for motorDriver.c*/
void PWM_init();
void Control_loop_init();
void Motor_driver_init();
void set_dutycycleM1(uint16_t);
void set_dutycycleM2(uint16_t);


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
