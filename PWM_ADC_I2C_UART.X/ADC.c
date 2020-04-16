/** ADC.c

@Author
Aniket Mazumder
Department of Robotics
a.mazumder@rug.nl
March, 2020
  
@Company
University of Groningen

  @File Name
ADC.c

@Summary
 This function sets up Class 1 Input ADC for ADC modules 2,3,4.
 The outputs are available at pins RB2,RB3 and RPB4 of the Curiosity 
 development platform respectively.
 
 * The conversion is triggered from Software hence no extra peripherals 
 * have been used for the conversion trigger.
 * Default Voltage references have been selected(Vdd and Vss)
 * No timers or interrupts have been used  

@Description
    This file sets up the ADC's for conversion of analog sensor data .
 */


#include<xc.h>
#include "header.h"
#include <proc/p32mz2048efm100.h>


void ADC_init()
{
    
/* Function to initialize ADC for PIC32 MZ
 */    
    asm volatile("di"); // Disable all interrupts. Don't enable global interrupts before all peripherals are configured.

    
/*Configure pins 2,3,and 4 of port B( AN2,AN3,AN4) as analog pins*/
    ANSELBbits.ANSB2=1;
    ANSELBbits.ANSB3=1;
    ANSELBbits.ANSB4=1;

/*Configure pins 2,3,and 4 of port B as input pins*/
    TRISBbits.TRISB2=1;
    TRISBbits.TRISB3=1;
    TRISBbits.TRISB4=1;

/* initialize ADC calibration setting. Check section 34 Special features.*/
    ADC0CFG = DEVADC0;
    ADC1CFG = DEVADC1;
    ADC2CFG = DEVADC2;
    ADC3CFG = DEVADC3;
    ADC4CFG = DEVADC4;
    ADC7CFG = DEVADC7;
  
/*Configure ADCCON1*/ 
    ADCCON1 = 0; // No ADCCON1 features are enabled including: Stop-in-Idle, turbo,
// CVD mode, Fractional mode and scan trigger source.
    //Configure ADCCON2
    ADCCON2 = 0; // Since, we are using only the Class 1 inputs, no setting is
// required for ADCDIV

//Initialize warm up time register 
    ADCANCON = 0;
    ADCANCONbits.WKUPCLKCNT = 5; // Wakeup exponent = 32 * TADx


//Clock setting
    ADCCON3 = 0;
    ADCCON3bits.ADCSEL = 0; // Select input clock source
    ADCCON3bits.CONCLKDIV = 1; // Control clock frequency is half of input clock
    ADCCON3bits.VREFSEL = 0; // Select AVDD and AVSS as reference source

//Select ADC sample time and conversion clock
    ADC2TIMEbits.ADCDIV = 1; // ADC0 clock frequency is half of control clock = TAD0
    ADC2TIMEbits.SAMC = 5; // ADC0 sampling time = 5 * TAD0
    ADC2TIMEbits.SELRES = 3; // ADC0 resolution is 12 bits
    ADC3TIMEbits.ADCDIV = 1; // ADC1 clock frequency is half of control clock = TAD1
    ADC3TIMEbits.SAMC = 5; // ADC1 sampling time = 5 * TAD1
    ADC3TIMEbits.SELRES = 3; // ADC1 resolution is 12 bits
    ADC4TIMEbits.ADCDIV = 1; // ADC2 clock frequency is half of control clock = TAD2
    ADC4TIMEbits.SAMC = 5; // ADC2 sampling time = 5 * TAD2
    ADC4TIMEbits.SELRES = 3; // ADC2 resolution is 12 bits


//Select analog input for ADC modules, no presync trigger, not sync sampling
    
    ADCTRGMODEbits.SH2ALT = 0; // ADC1 = AN1
    ADCTRGMODEbits.SH3ALT = 0; // ADC2 = AN2
    ADCTRGMODEbits.SH4ALT = 0; // ADC4 = AN4
//Select ADC input mode
    ADCIMCON1bits.SIGN2 = 0; // unsigned data format
    ADCIMCON1bits.DIFF2 = 0; // Single ended mode
    ADCIMCON1bits.SIGN3 = 0; // unsigned data format
    ADCIMCON1bits.DIFF3 = 0; // Single ended mode
    ADCIMCON1bits.SIGN4 = 0; // unsigned data format
    ADCIMCON1bits.DIFF4 = 0; // Single ended mode

//Configure ADCGIRQENx
    ADCGIRQEN1 = 0; // No interrupts are used
    ADCGIRQEN2 = 0;
//Configure ADCCSSx
    ADCCSS1 = 0; // No scanning is used
    ADCCSS2 = 0;
//Configure ADCCMPCONx
    ADCCMPCON1 = 0; // No digital comparators are used. Setting the ADCCMPCONx
    ADCCMPCON2 = 0; // register to '0' ensures that the comparator is disabled.
    ADCCMPCON3 = 0; // Other registers are ?don't care?.
    ADCCMPCON4 = 0;
    ADCCMPCON5 = 0;
    ADCCMPCON6 = 0;

    
//Configure ADCFLTRx
    ADCFLTR1 = 0; // No oversampling filters are used.
    ADCFLTR2 = 0;
    ADCFLTR3 = 0;
    ADCFLTR4 = 0;
    ADCFLTR5 = 0;
    ADCFLTR6 = 0;
 
//Set up the trigger sources
    
    ADCTRGSNSbits.LVL2 = 0; // Edge trigger
    ADCTRGSNSbits.LVL3 = 0; // Edge trigger
    ADCTRGSNSbits.LVL4 = 0; // Edge trigger
    
    ADCTRG1bits.TRGSRC2 = 1; // Set AN2 to trigger from software.
    ADCTRG1bits.TRGSRC3 = 1; // Set AN3 to trigger from software.
    ADCTRG2bits.TRGSRC4 = 1; // Set AN4 to trigger from software.
//Early interrupt
    ADCEIEN1 = 0; // No early interrupt
    ADCEIEN2 = 0;
    
//Turn the ADC on
    ADCCON1bits.ON = 1;
//Wait for voltage reference to be stable
    while(!ADCCON2bits.BGVRRDY); // Wait until the reference voltage is ready
    while(ADCCON2bits.REFFLT); // Wait if there is a fault with the reference voltage
//Enable clock to analog circuit
    
    ADCANCONbits.ANEN2 = 1; // Enable the clock to analog bias
    ADCANCONbits.ANEN3 = 1; // Enable the clock to analog bias
    ADCANCONbits.ANEN4 = 1; // Enable the clock to analog bias
//Wait for ADC to be ready
    
    while(!ADCANCONbits.WKRDY2); // Wait until ADC2 is ready
    while(!ADCANCONbits.WKRDY3); // Wait until ADC3 is ready
    while(!ADCANCONbits.WKRDY4); // Wait until ADC4 is ready
//Enable the ADC module
    
    ADCCON3bits.DIGEN2 = 1; // Enable ADC2
    ADCCON3bits.DIGEN3 = 1; // Enable ADC3
    ADCCON3bits.DIGEN4 = 1; // Enable ADC4
 
    //asm volatile("ei"); // Enable Global Interrupts once all peripherals are configured

    
}



void getADC(uint16_t *ADC1, uint16_t *ADC2, uint16_t *ADC3)
{
        //extern uint16_t resultADC[3];
        // Trigger a conversion 
        ADCCON3bits.GSWTRG = 1;
        // Wait the conversions to complete 
        while (ADCDSTAT1bits.ARDY2 == 0);
        // fetch the result 
        //resultADC[0] = ADCDATA2;
        *ADC1=ADCDATA2;
        while (ADCDSTAT1bits.ARDY3 == 0);
        // fetch the result 
        //resultADC[1] = ADCDATA3;
        *ADC2=ADCDATA3;
        while (ADCDSTAT1bits.ARDY4 == 0);
        // fetch the result 
        //resultADC[2] = ADCDATA4;
        *ADC3=ADCDATA4;
        //return resultADC;
}