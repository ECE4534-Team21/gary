/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    sensor.c

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

#include "sensor.h"

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

//SENSOR_DATA sensorData;

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
    void SENSOR_Initialize ( void )

  Remarks:
    See prototype in sensor.h.
 */

void SENSOR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    sensorData.state = SENSOR_STATE_INIT;
    //PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
    initDebug();
    //Setup 50ms timer
    sensorData.sensorTimer = xTimerCreate("Sensor Timer", 50 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, sensorTimerCallback);
    initLineSensor();
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void SENSOR_Tasks ( void )

  Remarks:
    See prototype in sensor.h.
 */

void SENSOR_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( sensorData.state )
    {
        /* Application's initial state. */
        case SENSOR_STATE_INIT:
        {
            sensorData.state = SENSOR_STATE_RUNNING;
            xTimerStart(sensorData.sensorTimer, 100);
            PLIB_ADC_Enable (DRV_ADC_INDEX_0);
            //PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
            //PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_12);
            break;
        }

        /* TODO: implement your application state machine.*/
        case SENSOR_STATE_RUNNING:
        {
            //debug(SENSOR_TASK);
        }
        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
void initLineSensor(){

    //Set the inputs and outputs for the line sensor
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, LINE_SENSOR_RIGHT_PORT, LINE_SENSOR_RIGHT_BIT);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, LINE_SENSOR_MIDDLE_PORT, LINE_SENSOR_MIDDLE_BIT);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, LINE_SENSOR_LEFT_PORT, LINE_SENSOR_LEFT_BIT);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, LED4_PORT, LED4_BIT);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, LED5_PORT, LED5_BIT);
    
}
char readLineSensor(){
    unsigned char sensorValue = 0x00;
    sensorValue = (sensorValue << 1) | LINE_SENSOR_LEFT_VALUE;
    sensorValue = (sensorValue << 1) | LINE_SENSOR_MIDDLE_VALUE;
    sensorValue = (sensorValue << 1) | LINE_SENSOR_RIGHT_VALUE;
    switch(sensorValue){
        //1 = BLACK, 0 = WHITE
        case 0x05: //101 ON TRACK
            SET_LED4;
            SET_LED5;
            break;
        case 0x06: //110 LEFT OF CENTER, GO RIGHT
            SET_LED5;
            CLEAR_LED4;
            break;
        case 0x04: //100 LEFT OF CENTER, GO RIGHT
            SET_LED5;
            CLEAR_LED4;
            break;
        case 0x03: //011 RIGHT OF CENTER, GO LEFT
            CLEAR_LED5;
            SET_LED4;
            break;
        case 0x01: //001 RIGHT OF CENTER, GO LEFT
            CLEAR_LED5;
            SET_LED4;
            break;
        default: //else
            CLEAR_LED4;  
            CLEAR_LED5;      
            break;
    }
    return sensorValue;
}

void sensorTimerCallback(TimerHandle_t timer) {
    PLIB_ADC_SampleAutoStartEnable(DRV_ADC_INDEX_0);
    readLineSensor();
    /*if(PLIB_PORTS_PinGet (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_12))
        PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
    else
        PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);*/
    
    //debug(SENSOR_TIMER_CALLBACK);
}
 

/*******************************************************************************
 End of File
 */
