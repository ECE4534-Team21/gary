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
    oledData.OLEDTimer = xTimerCreate("OLED Timer", 200 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, oledTimerCallback);
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
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
            //xTimerStart(oledData.OLEDTimer, 200);
            //oledData.state = OLED_STATE_RUNNING;
            break;
        }
        
        case OLED_STATE_RUNNING:
        {
            OledClearBuffer();
            OledSetCursor(0, 0);
            OledPutString("ECE 4534");
            OledSetCursor(0, 2);
            OledPutString("Team 21 -GARY");
            OledUpdate();
            
            oledData.state = OLED_STATE_HOLD;
            
            //char receivedValue = NULL;
            //xQueueReceive( oledData.OLEDQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            //debug(OLED_RECEIVED_MESSAGE_ON_QUEUE);
            break;
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
}

 

/*******************************************************************************
 End of File
 */
