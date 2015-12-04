/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    control.c

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

#include "CONTROL.h"
//#include "basicIO.h"

#include "message.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

struct Message recievedMessage;

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

//CONTROL_DATA controlData;

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
    void CONTROL_Initialize ( void )

  Remarks:
    See prototype in control.h.
 */

void CONTROL_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    controlData.state = CONTROL_STATE_INIT;
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void CONTROL_Tasks ( void )

  Remarks:
    See prototype in control.h.
 */

void CONTROL_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( controlData.state ) {
        /* Application's initial state. */
        case CONTROL_STATE_INIT:
        {
            controlData.controlQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            CONTROLQUEUE_SIZE, //number of slots in the queue
                            /* The size of each item the queue holds. */
                            sizeof( unsigned long ) );
            controlData.state = CONTROL_STATE_WAITING;
            break;
        }
        case CONTROL_STATE_WAITING:
        {
            unsigned int receivedValue;
            xQueueReceive(controlData.controlQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            recievedMessage = decode(receivedValue);
            
            //Make sure message from proper source
            unsigned int message = 0;
            if (recievedMessage.from == USART_TASK) { //From PI / USART
                
                /*Maybe test the purpose of the message?*/
                
                //Check the purpose, 0 = debug, 1 = run, 2 = restart, USART_START_SIGNAL (0x1)
                switch (recievedMessage.purpose) {
                    case 0:
                        controlData.state = CONTROL_STATE_TESTING;
                        break;
                    case USART_START_SIGNAL:
                        message = encode(CONTROL_TASK,CONTROL_PURPOSE_START,0);
                        xQueueSend(roverData.roverQueue, &message, pdTRUE);
                        break;
                    case 2:
                        break;
                    default:
                        break;
                }
                
            }
            
        }
        case CONTROL_STATE_TESTING:
        {
            //Test everything in no particular order
            //CONTROL_runTests();
            break;
        }
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/*void CONTROL_runTests() {
    unsigned int recievedValue;
    
    //Tell the ROVER task to get ready for testing
    unsigned int msg = encode(CONTROL_TASK, CONTROL_PURPOSE_TEST, MSG_TESTS_INIT);
    xQueueSend(roverData.roverQueue, &msg, pdTRUE);
    
    //Wait for the ROVER task to tell us it's ready for testing
    recievedMessage.from = CONTROL_TASK;
    while (recievedMessage.from != ROVER_TASK || recievedMessage.purpose != ROVER_PURPOSE_TESTING 
            || recievedMessage.message != MSG_TESTS_READY) {
        xQueueReceive( controlData.controlQueue, &recievedValue, portMAX_DELAY ); //blocks until there is a character in the queue
        recievedMessage = decode(recievedValue);
    }
    
    //Now that it's ready, start the testing
    //setPIN(basic_io_data.LEDs[7], 1);
    msg = encode(CONTROL_TASK, CONTROL_PURPOSE_TEST, MSG_TESTS_START);
    xQueueSend(roverData.roverQueue, &msg, pdTRUE);
    
    //Wait for the ROVER task to be done testing
    recievedMessage.from = CONTROL_TASK;
    while (recievedMessage.from != ROVER_TASK || recievedMessage.purpose != ROVER_PURPOSE_TESTING
            || recievedMessage.message != MSG_TESTS_DONE) {
        xQueueReceive( controlData.controlQueue, &recievedValue, portMAX_DELAY ); //blocks until there is a character in the queue
        recievedMessage = decode(recievedValue);
    }
    
    /*
     * Now run the tests for the next TASK/THREAD
     */
    //setPIN(basic_io_data.LEDs[6], 1);
    
    //while (1) {}*/
//}
 
/*******************************************************************************
 End of File
 */
