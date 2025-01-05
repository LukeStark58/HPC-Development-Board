 
#include "xc.h"
#include "PWM.h"


// Activates PWM and sets initial settings //////////////////////////////////////////////
void PWM_Init (void)
{
    OC1CON1 = 0; /* It is a good practice to initially clear the control bits */
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0x07; /* The peripheral clock is selected as the clock input to the OC module */
    OC1R = 1000; /* This sets the duty cycle to 50%; however, OC1R can be changed by the potentiometer, altering the duty cycle */
    OC1RS = 2000; /* Determines the Period */
    OC1CON1bits.OCM = 6; /* This selects the Edge Aligned PWM mode*/
    OC1CON2bits.SYNCSEL = 0x1F; /* No Trigger or sync source is selected */
    OSCCONbits.IOLOCK = 0;
    RPOR1bits.RP36R = 0b010000;
    OSCCONbits.IOLOCK = 1;
}