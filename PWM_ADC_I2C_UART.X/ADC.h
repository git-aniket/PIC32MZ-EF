/** ADC.h

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
ADC.h

@Summary
 Function prototypes for ADC.c

#ifndef _ADC_H    /* Guard against multiple inclusion */
#define _ADC_H


/*prototypes in ADC.c*/
void ADC_init();
void getADC(volatile uint16_t *,volatile uint16_t *,volatile uint16_t *);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
