/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <xc.h>

#include <stdbool.h>

#include "leds.h"

#include "..\io_mapping.h"

#define LED_D3_LAT      LATCbits.LATC0
#define LED_D4_LAT      LATCbits.LATC1
#define LED_D5_LAT      LATBbits.LATB4
#define LED_D6_LAT      LATAbits.LATA8
#define LED_D7_LAT      LATEbits.LATE12
#define LED_D8_LAT      LATEbits.LATE13
#define LED_D9_LAT      LATEbits.LATE14
#define LED_D10_LAT     LATCbits.LATC8      //Overlaps with S5

#define P88_C9_LAT      LATCbits.LATC9
#define P88_OUTPUT_TRIS TRISCbits.TRISC9

#define LED_D3_TRIS     TRISCbits.TRISC0
#define LED_D4_TRIS     TRISCbits.TRISC1
#define LED_D5_TRIS     TRISBbits.TRISB4
#define LED_D6_TRIS     TRISAbits.TRISA8
#define LED_D7_TRIS     TRISEbits.TRISE12
#define LED_D8_TRIS     TRISEbits.TRISE13
#define LED_D9_TRIS     TRISEbits.TRISE14
#define LED_D10_TRIS    TRISCbits.TRISC8    //Overlaps with S5

#define LED_ON  1
#define LED_OFF 0

#define P88_ON 1
#define P88_OFF 0

#define INPUT  1
#define OUTPUT 0


/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_On(LED led)
{
    switch(led)
    {
        case LED_D3:
            LED_D3_LAT = LED_ON ;
            break ;

        case LED_D4:
            LED_D4_LAT = LED_ON ;
            break ;

        case LED_D5:
            LED_D5_LAT = LED_ON;
            break;

        case LED_D6:
            LED_D6_LAT = LED_ON;
            break;
            
        case LED_D7:
            LED_D7_LAT = LED_ON ;
            break ;

        case LED_D8:
            LED_D8_LAT = LED_ON ;
            break ;

        case LED_D9:
            LED_D9_LAT = LED_ON ;
            break ;

        case LED_D10:
            LED_D10_LAT = LED_ON ;
            break ;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Off(LED led)
{
    switch(led)
    {
        case LED_D3:
            LED_D3_LAT = LED_OFF ;
            break ;

        case LED_D4:
            LED_D4_LAT = LED_OFF ;
            break ;

        case LED_D5:
            LED_D5_LAT = LED_OFF;
            break;

        case LED_D6:
            LED_D6_LAT = LED_OFF;
            break;

        case LED_D7:
            LED_D7_LAT = LED_OFF ;
            break ;

        case LED_D8:
            LED_D8_LAT = LED_OFF ;
            break ;

        case LED_D9:
            LED_D9_LAT = LED_OFF ;
            break ;

        case LED_D10:
            LED_D10_LAT = LED_OFF ;
            break ;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Toggle(LED led)
{
    switch(led)
    {
        case LED_D3:
            LED_D3_LAT ^= 1 ;
            break ;

        case LED_D4:
            LED_D4_LAT ^= 1 ;
            break ;

        case LED_D5:
            LED_D5_LAT ^= 1;
            break;

        case LED_D6:
            LED_D6_LAT ^= 1;
            break;

        case LED_D7:
            LED_D7_LAT ^= 1 ;
            break ;

        case LED_D8:
            LED_D8_LAT ^= 1 ;
            break ;

        case LED_D9:
            LED_D9_LAT ^= 1 ;
            break ;

        case LED_D10:
            LED_D10_LAT ^= 1 ;
            break ;

        default:
            break;
    }
}

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool LED_Get(LED led)
{
    switch(led)
    {
        case LED_D3:
            return ( ( LED_D3_LAT == LED_ON ) ? true : false ) ;

        case LED_D4:
            return ( ( LED_D4_LAT == LED_ON ) ? true : false ) ;

        case LED_D5:
            return ( (LED_D5_LAT == LED_ON) ? true : false );

        case LED_D6:
            return ( (LED_D6_LAT == LED_ON) ? true : false );

        case LED_D7:
            return ( ( LED_D7_LAT == LED_ON ) ? true : false ) ;

        case LED_D8:
            return ( ( LED_D8_LAT == LED_ON ) ? true : false ) ;

        case LED_D9:
            return ( ( LED_D9_LAT == LED_ON ) ? true : false ) ;

        case LED_D10:
            return ( ( LED_D10_LAT == LED_ON ) ? true : false ) ;

        default:
            break;
    }
    return false;
}

/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(LED led)
{
    switch(led)
    {
        case LED_D3:
            LED_D3_TRIS = OUTPUT ;
            break ;

        case LED_D4:
            LED_D4_TRIS = OUTPUT ;
            break ;

        case LED_D5:
            LED_D5_TRIS = OUTPUT;
            break;

        case LED_D6:
            LED_D6_TRIS = OUTPUT;
            break;

        case LED_D7:
            LED_D7_TRIS = OUTPUT ;
            break ;

        case LED_D8:
            LED_D8_TRIS = OUTPUT ;
            break ;

        case LED_D9:
            LED_D9_TRIS = OUTPUT ;
            break ;

        case LED_D10:
            LED_D10_TRIS = OUTPUT ;
            break ;

        default:
            break;
    }
}

void LED_Display (int lednumber)
{
    if (lednumber == 1)
    {
        LED_On( LED_First );
        LED_Off( LED_Second );
        LED_Off( LED_Third );
        LED_Off( LED_Fourth );
        LED_Off( LED_Fifth );
        LED_Off( LED_Sixth );
        LED_Off ( LED_Seventh );
    }
    else if (lednumber == 2)
    {
        LED_Off( LED_First );
        LED_On ( LED_Second );
        LED_Off( LED_Third );
        LED_Off( LED_Fourth );
        LED_Off( LED_Fifth );
        LED_Off( LED_Sixth );
        LED_Off ( LED_Seventh );
    }
    else if (lednumber == 3)
    {
        LED_Off( LED_First );
        LED_Off( LED_Second );
        LED_On( LED_Third );
        LED_Off( LED_Fourth );
        LED_Off( LED_Fifth );
        LED_Off( LED_Sixth );
        LED_Off ( LED_Seventh );
    }
    else if (lednumber == 4)
    {
        LED_Off( LED_First );
        LED_Off( LED_Second );
        LED_Off( LED_Third );
        LED_On( LED_Fourth );
        LED_Off( LED_Fifth );
        LED_Off( LED_Sixth );
        LED_Off ( LED_Seventh );
    }
    else if (lednumber == 5)
    {
        LED_Off( LED_First );
        LED_Off( LED_Second );
        LED_Off( LED_Third );
        LED_Off( LED_Fourth );
        LED_On( LED_Fifth );
        LED_Off( LED_Sixth );
        LED_Off ( LED_Seventh );
    }
}



//P88 Output Setup

//Turns P88 ON
void P88_On(P88 p88)
{
    switch(p88)
    {
        case P88_OUTPUT:
            P88_OUTPUT_TRIS = P88_ON ;
            break ;
    }
}

//Turns P88 OFF
void P88_Off(P88 p88)
{
    switch(p88)
    {
        case P88_OUTPUT:
            P88_OUTPUT_TRIS = P88_OFF ;
            break ;
    }
}

//Toggles P88
void P88_Toggle (P88 p88)
{
    switch(p88)
    {
        case P88_OUTPUT:
            P88_C9_LAT ^= 1 ;
            break ;
    }
}

bool P88_Get(P88 p88)
{
    switch(p88)
    {
        case P88_OUTPUT:
            return ( ( P88_OUTPUT_TRIS == P88_ON ) ? true : false ) ;

    }
}
            
void P88_Enable(P88 p88)
{
    switch (p88)
    {
        case P88_OUTPUT:
            P88_OUTPUT_TRIS = OUTPUT ;
            break;
        default:
            break;
    }
}
