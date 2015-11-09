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
#include "message.h"
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

OLED_DATA oledData, oledNext;
struct Message incomingQueueMessage;

int coin_y = 0, coin_x = 15;
int animation_counter = 0, score_counter = 1, coin_sensor_trigger = 0;

int OLED_NUM [10] = {OLED_0, OLED_1, OLED_2, OLED_3, OLED_4, OLED_5, OLED_6, OLED_7, OLED_8, OLED_9};

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
    oledData.OLEDTimer = xTimerCreate("OLED Timer", 50 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, oledTimerCallback);
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

void OLED_Write_Score( int score )
{
    int score_1 = 0, score_2 = score%10;
    if (score/10 >= 1)
        score_1 = score/10;
   
    
    OLED_Write_Num(7, OLED_NUM[score_1]);
    OLED_Write_Num(10, OLED_NUM[score_2]);
    
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
            
            oledData.OLEDQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            CONTROLQUEUE_SIZE, //number of slots in the queue
                            /* The size of each item the queue holds. */
                            sizeof( unsigned int ) );

            xTimerStart(oledData.OLEDTimer, 200);
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
            OledClearBuffer();
            OledSetCursor(2, 2);
            //display shot glass
            OledPutString("\x22\x23");
            OledSetCursor(2, 3);
            OledPutString("\x24\x25");
            OledUpdate();
            
            coin_x = 15;
            coin_y = 0;
            
            oledNext.state = OLED_STATE_ANIMATION_1;
            oledData.state = OLED_STATE_CHECK_SENSOR;
            DelayMs(400);
            
            /*OledClearBuffer();
            OLED_Write_Score(8);
            OledUpdate();
            oledData.state = OLED_STATE_HOLD;*/
            
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
            DelayMs(75);
            
            oledData.state = OLED_STATE_ANIMATION_INIT;
            
            /*if (animation_counter == 4) {
                animation_counter = 0;
                oledData.state = OLED_STATE_DISPLAY_SCORE;
            }
            else    {
                animation_counter += 1;
                oledData.state = OLED_STATE_ANIMATION_INIT;
            }*/
           
            break;              
        }
        case OLED_STATE_DISPLAY_SCORE:
        {
            OledClearBuffer();
            OLED_Write_Score(score_counter);
            OledUpdate();
            oledData.state = OLED_STATE_CHECK_SENSOR;
            oledNext.state = OLED_STATE_CHECK_SENSOR;
            //DelayMs(1000);
            
            if (score_counter == 21)    {
                DelayMs(1000);
                score_counter = 1;
                oledData.state = OLED_STATE_RUNNING;
            }
            else
                score_counter += 1;
        }
        case OLED_STATE_CHECK_SENSOR:
        {
            unsigned int receivedValue;
            xQueueReceive( oledData.OLEDQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            incomingQueueMessage = decode(receivedValue);
            /*if(incomingQueueMessage.from == SENSOR_TASK && incomingQueueMessage.purpose == LINE_SENSOR_DATA){
                    //this means the message came from the SENSOR_TASK, and is LINE_SENSOR data
                    //do things in here accordingly
                    //incomingQueueMessage.message ...
                    //An example of how to decode/use the line sensor values can be found in
                    //the adjustMotorsFromLineSensor() function in ROVER.c
            }*/
            if(incomingQueueMessage.from == SENSOR_TASK && incomingQueueMessage.purpose == COIN_SENSOR_DATA){
                if (incomingQueueMessage.message == 1)  {
                    coin_sensor_trigger = 1;
                    oledData.state = oledNext.state;
                }
                else if (incomingQueueMessage.message == 0) {
                    if (coin_sensor_trigger == 1)   {
                        oledData.state = OLED_STATE_DISPLAY_SCORE;
                        coin_sensor_trigger = 0;
                    }
                }

                //this means the message came from the SENSOR_TASK, and is COIN_SENSOR data
                //do things in here accordingly
                //Look in SourceFiles/system_config/default/system_interrupt.c if you want to see the coin sensor code
                //if incomingQueueMessage.message == 1 -> the coin sensor is seeing light (no coin in cup)
                //if incomingQueueMessage.message == 0 -> the coin sensor is NOT seeing light (coin is in cup)
            }
        }
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
    //oledData.state = OLED_STATE_ANIMATION_INIT;
    printf("In callback ");
}

 

/*******************************************************************************
 End of File
 */
