/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    oled.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "oled.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

OLED_DATA oledData;
int coin_y = 0, coin_x = 15;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback funtions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void OLED_Initialize ( void )

  Remarks:
    See prototype in oled.h.
 */

void OLED_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    oledData.state = OLED_STATE_INIT;
    oledData.OLEDTimer = xTimerCreate("OLED Timer", 100 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, oledTimerCallback);
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

void OLED_Write_Score( int score )
{
    int x = 0, score_1 = 0, score_2 = score%10;
    if (score/10 >= 1)  {
        score_1 = score/10;
        x = 0;
    }   
    
    OLED_Write_Num(x, OLED_0);
    x += 3;
    OLED_Write_Num(x, OLED_1);
    x += 3;
    OLED_Write_Num(x, OLED_2);
    x += 3;
    OLED_Write_Num(x, OLED_3);
    x += 3;
    OLED_Write_Num(x, OLED_4);
    
    DelayMs(750);
    x = 0;
    
    OledClearBuffer();
    OLED_Write_Num(x, OLED_5);
    x += 3;
    OLED_Write_Num(x, OLED_6);
    x += 3;
    OLED_Write_Num(x, OLED_7);
    x += 3;
    OLED_Write_Num(x, OLED_8);
    x += 3;
    OLED_Write_Num(x, OLED_9);
    
    DelayMs(750);
    //x += 3;
    
        
    /*if (score_2 == 8) {
            OledSetCursor(x, 0);
            OledPutString("\x2a\x2b\x2c");
            OledSetCursor(x, 1);
            OledPutString("\x2a\x2f\x2c");
            OledSetCursor(x, 2);
            OledPutString("\x2a\x2e\x2c");
            OledSetCursor(x, 3);
            OledPutString("\x2a\x2d\x2c");
    }*/
}

void OLED_Write_Num ( int x, int num_display )
{
    if (num_display & 1)  {
        OledSetCursor(x+1, 0);
        OledPutString("\x2b");
    }
    if (num_display & 2)  {
        OledSetCursor(x+2, 0);
        OledPutString("\x2c");
        OledSetCursor(x+2, 1);
        OledPutString("\x2c");
    }
    if (num_display & 4)  {
        OledSetCursor(x+2, 2);
        OledPutString("\x2c");
        OledSetCursor(x+2, 3);
        OledPutString("\x2c");
    }
    if (num_display & 8)  {
        OledSetCursor(x+1, 4);
        OledPutString("\x2d");
    }
    if (num_display & 16)  {
        OledSetCursor(x, 2);
        OledPutString("\x2a");
        OledSetCursor(x, 3);
        OledPutString("\x2a");
    }
    if (num_display & 32)  {
        OledSetCursor(x, 0);
        OledPutString("\x2a");
        OledSetCursor(x, 1);
        OledPutString("\x2a");
    }
    if (num_display & 64)  {
        OledSetCursor(x+1, 1);
        OledPutString("\x2f");
        OledSetCursor(x+1, 2);
        OledPutString("\x2e");
    }
    
    OledUpdate();
}

/******************************************************************************
  Function:
    void OLED_Tasks ( void )

  Remarks:
    See prototype in oled.h.
 */

void OLED_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( oledData.state )
    {
        /* Application's initial state. */
        case OLED_STATE_INIT:
        {
            DelayInit();
            OledInit();
            
            oledData.state = OLED_STATE_RUNNING;
            
            //oledData.OLEDQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            //CONTROLQUEUE_SIZE, //number of slots in the queue
                            /* The size of each item the queue holds. */
                            //sizeof( char ) );
            //xTimerStart(oledData.OLEDTimer, 75);
            //oledData.state = OLED_STATE_RUNNING;
            break;
        }
        
        case OLED_STATE_RUNNING:
        {
            OledClearBuffer();
            OledSetCursor(0, 0);
            OledPutString("ECE 4534");
            OledSetCursor(0, 2);
            OledPutString("Team 21 GARY");
            OledUpdate();
            
            oledData.state = OLED_STATE_ANIMATION_INIT;
            DelayMs(750);
            
            //char receivedValue = NULL;
            //xQueueReceive( oledData.OLEDQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            //debug(OLED_RECEIVED_MESSAGE_ON_QUEUE);
            break;
        }
        case OLED_STATE_ANIMATION_INIT:
        {
            /*OledClearBuffer();
            OledSetCursor(2, 2);
            //display shot glass
            OledPutString("\x22\x23");
            OledSetCursor(2, 3);
            OledPutString("\x24\x25");
            OledUpdate();
            
            coin_x = 15;
            coin_y = 0;
            
            oledData.state = OLED_STATE_ANIMATION_1;
            DelayMs(400);*/
            
            OledClearBuffer();
            OLED_Write_Score(8);
            OledUpdate();
            //oledData.state = OLED_STATE_HOLD;
            
            break;
        }
        case OLED_STATE_ANIMATION_1:
        {
            if (coin_x < 8 && coin_x > 3)    {
                OledSetCursor(coin_x, coin_y);
                OledPutString("  ");
                coin_y -= 1;
            }
                        
            OledSetCursor(coin_x, coin_y);
            OledPutString("\x27 ");
            OledUpdate();

            coin_x -= 1;
            if (coin_x == 2)
                oledData.state = OLED_STATE_ANIMATION_3;
            else
                oledData.state = OLED_STATE_ANIMATION_2;

            DelayMs(75);
            
            break;
        }
        case OLED_STATE_ANIMATION_2:
        {

            if (coin_x > 8) {
                OledSetCursor(coin_x, coin_y);
                OledPutString("  ");
                coin_y += 1;
            }

            OledSetCursor(coin_x, coin_y);
            OledPutString("\x26 ");  
            coin_x -= 1;           
            
            OledUpdate();
            oledData.state = OLED_STATE_ANIMATION_1;
            DelayMs(75);
            break;
        }
        case OLED_STATE_ANIMATION_3:
        { 
            OledSetCursor(coin_x, coin_y);
            OledPutString("\x28\x29");
            OledUpdate();
            oledData.state = OLED_STATE_ANIMATION_INIT;
            DelayMs(75);
            break;              
        }
        /*case OLED_STATE_ANIMATION_4:
        {
            //14
            OledSetCursor(coin_x, coin_y);
            OledPutString("  ");
            coin_y += 1;
            OledSetCursor(coin_x, coin_y);
            OledPutString("' ");
            OledUpdate();
            
            oledData.state = OLED_STATE_ANIMATION_5;
            coin_x -= 1;
            DelayMs(75);
            break;
        }
        case OLED_STATE_ANIMATION_5:
        {
            //13
            OledSetCursor(coin_x, coin_y);
            OledPutString("( ");
            OledUpdate();
            
            oledData.state = OLED_STATE_ANIMATION_6;
            coin_x -= 1;
            DelayMs(75);
            break;
        }
        case OLED_STATE_ANIMATION_6:
        {
            //12
            OledSetCursor(coin_x, coin_y);
            OledPutString("  ");
            coin_y += 1;
            OledSetCursor(coin_x, coin_y);
            OledPutString("' ");
            OledUpdate();
            
            oledData.state = OLED_STATE_ANIMATION_1;
            coin_x = 15;
            coin_y = 0;
            DelayMs(75);
            break;
        }*/
        case OLED_STATE_HOLD:
        {
            break;
        }
        /* TODO: implement your application state machine.*/

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void oledTimerCallback(TimerHandle_t timer){
    //debug(OLED_TIMER_CALLBACK);
}

 

/*******************************************************************************
 End of File
 */
