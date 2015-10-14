/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    usart.c

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

#include "usart.h"

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

USART_DATA usartData;

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
    void USART_Initialize ( void )

  Remarks:
    See prototype in usart.h.
 */

void USART_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    usartData.state = USART_STATE_INIT;

    /* Open the USART driver and get a handle*/
    usartData.usartReadHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
    //usartData.usartWriteHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_WRITE);
    
    /* Check if the handle is valid */
    Nop();
    if(DRV_HANDLE_INVALID == usartData.usartReadHandle || DRV_HANDLE_INVALID == usartData.usartWriteHandle)
    {
        /* The driver was not opened successfully. The client can try opening it again */
        Nop();
    }
    DRV_USART_BufferEventHandlerSet( usartData.usartReadHandle, usartCallback, (uintptr_t)&usartData);
    //DRV_USART_BufferEventHandlerSet( usartData.usartWriteHandle, usartCallback, (uintptr_t)&usartData);
}


/******************************************************************************
  Function:
    void USART_Tasks ( void )

  Remarks:
    See prototype in usart.h.
 */

void USART_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( usartData.state )
    {
        /* Application's initial state. */
        case USART_STATE_INIT:
        {
            initDebug();
            usartData.state = USART_STATE_RUN;
            usartData.usartRxMsgQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            rxQUEUE_LENGTH, //defined in app_public.h
                            /* The size of each item the queue holds. */
                            sizeof( char ) );
            usartData.usartMsgQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            txQUEUE_LENGTH, //defined in app_public.h
                            /* The size of each item the queue holds. */
                            sizeof( char ) );
            break;
        }
        
        case USART_STATE_RUN:
        {  
            debug(0x20);
            char receivedValue = NULL;
            DRV_USART_BufferAddRead(usartData.usartReadHandle, 
                                    &usartData.bufferHandle,
                                    usartData.usartBuffer, 
                                    12);
            xQueueReceive( usartData.usartRxMsgQueue, &receivedValue, portMAX_DELAY );   
            Nop();
            if (receivedValue == 'R'){
                int numBytes = 0;
                Nop();
                if(usartData.usartBuffer[0] == 0x3c){ //0xc = '<'
                    char currentChar = 0x3c;
                    usartData.messageBuffer[0] = currentChar;
                    debug(currentChar);
                    do {
                        Nop();
                        DRV_USART_BufferAddRead(usartData.usartReadHandle, 
                                    &usartData.bufferHandle,
                                    usartData.usartBuffer, 
                                    1);
//                        currentChar = usartData.usartBuffer[0];
//                        usartData.messageBuffer[++numBytes] = currentChar;
                        //debug(currentChar);
                    }
                    while(currentChar != 0x3e);
                    int i=0;
                    while(usartData.messageBuffer[i] != 0x3e){
                        debug(usartData.messageBuffer[i]);
                        i++;
                    }
                    debug(usartData.messageBuffer[i]);
                }
                Nop();
                //decodeMessage(usartData.myBuffer);
            }
            else if(receivedValue == 'W') {
                Nop();
            }
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
 
void decodeMessage(char * message){
    char * a = message;
}

void usartCallback(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE handle, uintptr_t context)
{
    // The context handle was set to an application specific
    // object. It is now retrievable easily in the event handler.
    switch(event)
    {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
        {
            char notify = 'R';
            BaseType_t * xHigherPriorityTaskWoken = pdFALSE; 
            Nop();
            if(usartData.usartBuffer[0] == 0x3c) {
                Nop();
                xQueueSendFromISR(usartData.usartRxMsgQueue, &notify, xHigherPriorityTaskWoken);
            }
            break;
        }
            
        case DRV_USART_BUFFER_EVENT_ERROR:
            Nop();
            break;
        default:
            break;
    }
}

/*******************************************************************************
 End of File
 */
