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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "bsp/adc.h"
#include "bsp/lcd.h"
#include "bsp/timer_1ms.h"
#include "bsp/buttons.h"
#include "bsp/leds.h"
#include "bsp/rtcc.h"
#include "PWM.h"
 
#include "io_mapping.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************
extern void SYS_Initialize ( void ) ;
static void BlinkAliveEventHandler( void );
static void ScreenUpdateEventHandler( void );
static void DebounceEventHandler(void);

static RTCC_DATETIME time;
static RTCC_DATETIME lastTime = {0};
static volatile bool toggleBlinkAlive = false;
static volatile bool allowScreenUpdate = true;
uint8_t bPage = 1;
bool P88_Active = 0;
bool button_release_demo = 0;
bool button_release_P88 = 0;
#define MEMCMP_VALUES_IDENTICAL 0

static bool lastButtonState = false;
static bool currentButtonState = false;
static bool debounceTimerActive_demo = false;
static bool debounceTimerActive_P88 = false;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    uint16_t adcResult;
    uint16_t lastAdcResult = 0xFFFF;
    int lednumber;
    
    /* Call the System Initialize routine*/
    SYS_Initialize ( );
    PWM_Init ();
    /* To determine how the LED and Buttons are mapped to the actual board
     * features, please see io_mapping.h. */
    
    // Enable LEDS, Buttons, and ADC Channels
    LED_Enable ( LED_BLINK_ALIVE );
    LED_Enable ( LED_BUTTON_PRESSED );
    LED_Enable ( LED_First );
    LED_Enable ( LED_Second );
    LED_Enable ( LED_Third );
    LED_Enable ( LED_Fourth );
    LED_Enable ( LED_Fifth );
    LED_Enable ( LED_Sixth );
    LED_Enable ( LED_Seventh );
    P88_Enable(0);
    

    BUTTON_Enable ( BUTTON_DEMO );
    BUTTON_Enable (P88_BUTTON);
    
    ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    ADC_ChannelEnable ( ADC_CHANNEL_POTENTIOMETER );
    ADC_ChannelEnable (ADC_CHANNEL_TEMPERATURE_SENSOR);
    ADC_ChannelEnable (ADC_CHANNEL_RHE_SENSOR);
    
    /* Get a timer event once every 100ms for the blink alive. */
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_1MS );
    TIMER_RequestTick( &BlinkAliveEventHandler, 500 );
    TIMER_RequestTick( &ScreenUpdateEventHandler, 300 );
    TIMER_RequestTick(&DebounceEventHandler, 1);
    
    /* The TIMER_1MS configuration should come before the RTCC initialization as
     * there are some processor modules that require the TIMER_1MS module to be
     * configured before the RTCC module, as the RTCC module is emulated using
     * the TIMER_1MS module. */
    time.bcdFormat = false;
    lastTime.bcdFormat = false;
    RTCC_BuildTimeGet( &time );
    RTCC_Initialize( &time );
    memset(&lastTime,0,sizeof(lastTime)); 
    
    /* Clear the screen */
    printf( "\f" );   
    
    while ( 1 )
    {
        RTCC_TimeGet( &time );
        //Only print if the ADC value or time has changed since the last time 
        // around the loop and we haven't updated recently too recently. 
        if(allowScreenUpdate == true)
        {
            
            
 ///////////////// PAGE 1 - POTENTIOMETER ///////////////////////////////////////////////////////////////////////////           
            
            if (bPage == 1)
            {
                adcResult = ADC_Read12bit( ADC_CHANNEL_POTENTIOMETER );
                
 ////////////////// LCD Display
                if( (adcResult != lastAdcResult) ||
                    (memcmp(&time, &lastTime, sizeof(time)) != MEMCMP_VALUES_IDENTICAL) )
                {                  
                    // clears display - helps prevent glitching when switching pages
                    printf("\f");
                    //Either ADC or time has changed, and the screen update refresh
                    // limit has expired, update the screen.
                    allowScreenUpdate = false;
                    printf( "Time %02d:%02d:%02d   Pot = %4d\r\n", 
                            time.hour, 
                            time.minute, 
                            time.second, 
                            adcResult
                          );
                    lastAdcResult = adcResult;
                    memcpy(&lastTime, &time, sizeof(time));
                }  
                
  /////////////// LED Display
                    if (adcResult >= 0 && adcResult < 819) 
                    {
                        lednumber = 1;
                    } 
                    else if (adcResult > 819 && adcResult < 1638) 
                    {
                        lednumber = 2;
                    } 
                    else if (adcResult > 1638 && adcResult < 2457) 
                    {
                        lednumber = 3;
                    } 
                    else if (adcResult > 2457 && adcResult < 3276) 
                    {
                        lednumber = 4;
                    } 
                    else if (adcResult > 3276 && adcResult < 4096) 
                    {
                        lednumber = 5;
                    } 
                    LED_Display (lednumber);
            }

/////////////////////////////////////////////////////////////////////////////////////////
////////////////// PAGE 2 - TEMPERATURE SENSOR /////////////////////////////////////////
            
            else if (bPage == 2)
            {   
 ////////////// LCD Display
                adcResult = ADC_Read12bit( ADC_CHANNEL_TEMPERATURE_SENSOR );     
                if( (adcResult != lastAdcResult) ||
                 (memcmp(&time, &lastTime, sizeof(time)) != MEMCMP_VALUES_IDENTICAL) )
                    {  
                        //Calculations to convert adcResult to Celsius
                        float voltage = (adcResult / 4095.0) * 3.3;
                        float temp = (voltage - 0.5) / 0.01; 
                        printf("\f");
                        //Either ADC or time has changed, and the screen update refresh
                        // limit has expired, update the screen.
                        allowScreenUpdate = false;
                        printf( "Time %02d:%02d:%02d   Temp = %.2f\r\n", 
                                 time.hour, 
                                 time.minute, 
                                 time.second, 
                                 temp
                               );
                        lastAdcResult = adcResult;
                        memcpy(&lastTime, &time, sizeof(time));
                    }
/////////////////// Turns off all LEDS
                LED_Off( LED_First );
                LED_Off( LED_Second );
                LED_Off( LED_Third );
                LED_Off( LED_Fourth );
                LED_Off( LED_Fifth );
                LED_Off( LED_Sixth );
                LED_Off ( LED_Seventh );
            }   
 ////////////////////////////////////////////////////////////////////////////////// 
 ///////////////// PAGE 3 - P88 OUTPUT /////////////////////////////////////////////
            
            else if (bPage == 3)
            { 
///////////////// LCD Display
                if (P88_Active == 0)
                {
                    printf("\f");
                    allowScreenUpdate = false;
                    printf("P88 Not Powered");
                }
                else if (P88_Active == 1)
                {
                    printf("\f");
                    allowScreenUpdate = false;
                    printf("P88 Powered");
                }
            }
            
 /////////////////////////////////////////////////////////////////////////////////
 ///////////// PAGE 4 - PWM //////////////////////////////////////////////////////
            else if (bPage == 4)
            {
               adcResult = ADC_Read12bit( ADC_CHANNEL_POTENTIOMETER );
               // Calculations to convert adcResult to duty cycle and PWM
               float dutycycle = (adcResult / 4095.0) * 2000; 
               OC1R = dutycycle; // changes duty cycle of PWM
               float pwm = (dutycycle / 20);
               
 /////////////// LCD Display
                if( (adcResult != lastAdcResult) ||
                    (memcmp(&time, &lastTime, sizeof(time)) != MEMCMP_VALUES_IDENTICAL) )
                {                  
                    printf("\f");
                    allowScreenUpdate = false;
                    printf( "Time %02d:%02d:%02d   PWM = %.1f PCT\r\n", 
                            time.hour, 
                            time.minute, 
                            time.second, 
                            pwm
                          );
                    lastAdcResult = adcResult;
                    memcpy(&lastTime, &time, sizeof(time));
                }   
            } 
//////////////////////////////////////////////////////////////////////////////////////////
////////////////// PAGE 0 - RHE SENSOR /////////////////////////////////////////////////////
            
            else if (bPage == 0) 
            {
                adcResult = ADC_Read12bit( ADC_CHANNEL_RHE_SENSOR );
                if( (adcResult != lastAdcResult) ||
                   (memcmp(&time, &lastTime, sizeof(time)) != MEMCMP_VALUES_IDENTICAL) )
                {   
                    // Calculations to convert adcResult to degrees of rotation
                    float voltage = (adcResult / 4095.0) * 5;
                    float degree = (voltage * 28.04) - 4.6;   
 ////////////// LCD Display
                    printf("\f");
                    allowScreenUpdate = false;
                    printf( "Time %02d:%02d:%02d   RHE = %.2f\r\n", 
                            time.hour, 
                            time.minute, 
                            time.second, 
                            degree
                          );
                    lastAdcResult = adcResult;
                    memcpy(&lastTime, &time, sizeof(time));
                }        
                
            }
        }
        
//////////////////////////////////////////////////////////////////////////////////////
////////////// BLINK ALIVE //////////////////////////////////////////////////////////
        
        if(toggleBlinkAlive == true)
        {
            LED_Toggle( LED_BLINK_ALIVE );
            toggleBlinkAlive = false;
        }
        
        lastButtonState = currentButtonState;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debounce Event Handler: it prevents the button from debouncing by requiring it to be pressed for 50 milliseconds and for it to be released before being active again
static void DebounceEventHandler(void) 
{
    //c is the counter for the demo button; it counts up to 50 milliseconds, which then triggers the button's output
    static int c = 0;
    static bool lastStableStateDemo = false;
    bool currentButtonStateDemo = BUTTON_IsPressed(BUTTON_DEMO);
    debounceTimerActive_demo = false;
        
    //d is the counter for the demo button; it counts up to 50 milliseconds, which then triggers the button's output
    static int d = 0;
    static bool lastStableStateP88 = false;
    bool currentButtonStateP88 = BUTTON_IsPressed(P88_BUTTON);
    debounceTimerActive_P88 = false;
    
        if (currentButtonStateDemo == true) 
        {
            if (button_release_demo == 1)
            {
                c++;
                if (c >= 50) 
                {
                    bPage = (bPage + 1)%5;
                    c = 0;
                    button_release_demo = 0;
                }    
            }    
        }
    // When the button is not pressed, button_release_----- is set to one, which is required for the button to have an output. After the button is pressed the button_release_----- so set to 0 so that the button needs to be released in order for it to function again
        else
        {
            c = 0;
            button_release_demo = 1;
        }
    // Second button for P88    
    if (bPage == 3)
    {
        if (currentButtonStateP88 == true)
        {
            if(button_release_P88 == 1) 
            {
                d++;
                if (d >= 50) 
                {
                    P88_Toggle (P88_OUTPUT);
                    P88_Active ^= 1;
                    d = 0;
                    button_release_P88 = 0;                  
                }  
            }
        }
        else
        {
           d = 0;
           button_release_P88 = 1;
        } 
    }
    lastStableStateDemo = currentButtonStateDemo;
    lastStableStateP88 = currentButtonStateP88;
}



static void BlinkAliveEventHandler(void)
{    
    toggleBlinkAlive = true;
}

static void ScreenUpdateEventHandler(void)
{
    allowScreenUpdate = true;
}

