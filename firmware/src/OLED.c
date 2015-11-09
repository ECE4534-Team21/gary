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

OLED_DATA oledData;
struct Message incomingQueueMessage;

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
            oledData.OLEDQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            CONTROLQUEUE_SIZE, //number of slots in the queue
                            /* The size of each item the queue holds. */
                            sizeof( unsigned int ) );
            xTimerStart(oledData.OLEDTimer, 200);
            oledData.state = OLED_STATE_RUNNING;
            break;
        }
        
        case OLED_STATE_RUNNING:
        {
            unsigned int receivedValue;
            xQueueReceive( oledData.OLEDQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            incomingQueueMessage = decode(receivedValue);
            if(incomingQueueMessage.from == SENSOR_TASK && incomingQueueMessage.purpose == LINE_SENSOR_DATA){
                    //this means the message came from the SENSOR_TASK, and is LINE_SENSOR data
                    //do things in here accordingly
                    //incomingQueueMessage.message ...
                    //An example of how to decode/use the line sensor values can be found in
                    //the adjustMotorsFromLineSensor() function in ROVER.c
            }
            
            else if(incomingQueueMessage.from == SENSOR_TASK && incomingQueueMessage.purpose == COIN_SENSOR_DATA){
                //this means the message came from the SENSOR_TASK, and is COIN_SENSOR data
                //do things in here accordingly
                //Look in SourceFiles/system_config/default/system_interrupt.c if you want to see the coin sensor code
                //if incomingQueueMessage.message == 1 -> the coin sensor is seeing light (no coin in cup)
                //if incomingQueueMessage.message == 0 -> the coin sensor is NOT seeing light (coin is in cup)
            }
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
    //put Bri's code here to do every 1000 ms
}

 

/*******************************************************************************
 End of File
 */
