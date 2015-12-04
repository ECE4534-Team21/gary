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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

//Flags
char reading = 0;
int bufferLoc = 0;

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
    usartData.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READ);
    
    /* Check if the handle is valid */
    if(DRV_HANDLE_INVALID == usartData.usartHandle)
    {
        /* The driver was not opened successfully. The client can try opening it again */
    }
    DRV_USART_BufferEventHandlerSet( usartData.usartHandle, usartCallback, (uintptr_t)&usartData);
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
            //initDebug();
            //PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
            //PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
            usartData.state = USART_STATE_RUN;
            usartData.sentAck = false;
            usartData.receivedAck = false;
            usartData.messageTimer = xTimerCreate("message timer", 2000 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, messageTimerCallback);
            usartData.usartMsgQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            QUEUE_LENGTH, //defined in USART.h
                            /* The size of each item the queue holds. */
                            sizeof( char ) );
            
            //Queue up a read
            DRV_USART_BufferAddRead(usartData.usartHandle, 
                                    &usartData.bufferReadHandle,
                                    usartData.usartBuffer, 
                                    1);
            //Nop();
            break;
        }        
        
        case USART_STATE_RUN:
        {  
            //debug(USART_BLOCK_FOR_QUEUE);
            char receivedValue = NULL;
            xQueueReceive(usartData.usartMsgQueue, &receivedValue, portMAX_DELAY);
            //Nop();
            PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
            //Check what is on the queue
            
            if (receivedValue == DONE_READ) /*Finished reading a message*/ {
                //decode the message
                Nop();
                decodeMessage();
                //If the decoded message was an ack "<0>" then ackFlag will be true
                //We don't want to send an ack once if we received an ack
                Nop();
                if(!usartData.receivedAck) {
                    usartData.sentAck = true;
                    Nop();
                    char ack[3] = "<0>";
                    sendMessage(ack, 3);
                }
                
                        
                //queue up another read
                DRV_USART_BufferAddRead(usartData.usartHandle, 
                                    &usartData.bufferReadHandle,
                                    usartData.usartBuffer, 
                                    1);
                
            } 
            else if (receivedValue == DONE_WRITE) /*Done writting*/ {
            
            } 
            else if (receivedValue == READY_SEND) /* Ready to send a message */ {
                int size, i;
                char buffer[38] = "";
                encodeMessage(&size, buffer);
                Nop();
                char message[size];
                for (i = 0; i < size; ++i)
                {
                    message[i] = buffer[i];
                }
                strcpy(usartData.lastMessage, buffer);
                usartData.lastMessageSize = size;
                usartData.sentAck = false;
                sendMessage(message, size);
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
 
void sendMessage(char buffer[], int size){
    Nop();
    //debug(USART_SEND_MESSAGE);
    int i;
    char message[size];
    for(i = 0; i < size; i++){
        message[i] = buffer[i];
        //debug(message[i]);
    }
    Nop();
    DRV_USART_BufferAddWrite(usartData.usartHandle, &usartData.bufferWriteHandle, message, size);
}

void encodeMessage(int * size, char buffer[]){
    Nop();
    char receivedValue = NULL;
    buffer[0] = '<';
    int i=1;
    while(receivedValue != '>') {
        xQueueReceive(usartData.usartMsgQueue, &receivedValue, portMAX_DELAY);
        buffer[i] = receivedValue;
        i++;
    }
    *size = (int) i;
}

void decodeMessage(){
    char message[38];
    strcpy(message,usartData.messageBuffer);
	char open,senderID,targetID,messageNumber,checkSum,close;
    targetID = message[2];
    senderID = message[1];
    usartData.receivedAck = false;
    usartData.sentAck = false;
    if(senderID == '0') {
        //debug(USART_RECEIVE_ACK);
        usartData.receivedAck = true;
        xTimerStop(usartData.messageTimer, 10);
    }
    else {
        switch(targetID){
            case '5':
            {
                //unsigned int messageData;
                //sscanf(message, "%c%c%c%c%c%u%c", &open,&senderID,&targetID,&messageData,&close);
                if(message[3] == 'S'){
                    unsigned int messageToControl;
                    messageToControl = encode(USART_TASK,USART_START_SIGNAL,0);
                    xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                }
                
                else if(message[3] == 'R'){
                    unsigned int messageToControl;
                    messageToControl = encode(USART_TASK,USART_RESTART_SIGNAL,0);
                    xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                }
                
                else if(message[3] == 'E'){
                    unsigned int messageToControl;
                    messageToControl = encode(USART_TASK,USART_STOP_SIGNAL,0);
                    xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                }
                
                else if(message[3] == 'G'){ //0 is continuous mode, 1 is segmented
                    if(message[4] == '0'){
                        //unsigned int messageToControl;
                        //messageToControl = encode(USART_TASK,USART_GAMEPLAY_SIGNAL,0);
                        //xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                        controlData.gameplay = 0;
                        
                    }
                    else if(message[4] == '1'){
                        //unsigned int messageToControl;
                        controlData.gameplay = 1;
                        //messageToControl = encode(USART_TASK,USART_GAMEPLAY_SIGNAL,1);
                        //xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                    }
                }
                
                else if(message[3] == 'C'){ //Point ceiling
                    unsigned int messageToControl = 21;
                    char numberBuffer [2] = {message[4], message[5]};
                    controlData.scoreCeiling = atoi(numberBuffer);
                    //messageToControl = encode(USART_TASK,USART_SCORE_CEILING_SIGNAL,0);
                    //xQueueSend(controlData.controlQueue, &messageToControl, pdTRUE);
                }
                
                break;
            }
            case '4':
            {
                unsigned int messageData;
                sscanf(message, "%c%c%c%c%c%u%c", &open,&senderID,&targetID,&messageData,&close);
                xQueueSend(roverData.roverQueue, &messageData, pdFALSE);
                break;
            }
            case '3':
            {
                unsigned int messageData;
                sscanf(message, "%c%c%c%c%c%u%c", &open,&senderID,&targetID,&messageData,&close);
                xQueueSend(oledData.OLEDQueue, &messageData, pdFALSE);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void usartCallback(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE handle, uintptr_t context)
{
    Nop();
    char notify = '\0';
    //debug(USART_CALLBACK_EVENT);
    switch(event)
    {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
            //Check if we are reading or need to start to
            if(handle == usartData.bufferReadHandle){
                if (reading) {
                    usartData.messageBuffer[bufferLoc] = usartData.usartBuffer[0];
                    debug(usartData.usartBuffer[0]);
                    if (usartData.messageBuffer[bufferLoc] == 0x3e) { // '>'
                        BaseType_t * xHigherPriorityTaskWoken = pdFALSE; 
                        reading = 0;
                        notify = DONE_READ;
                        //Send DONE to the queue
                        xQueueSendFromISR(usartData.usartMsgQueue, &notify, xHigherPriorityTaskWoken);
                    } else {
                        DRV_USART_BufferAddRead(usartData.usartHandle, 
                                        &usartData.bufferReadHandle,
                                        usartData.usartBuffer, 
                                        1);
                    }
                    bufferLoc++;

                } else if (usartData.usartBuffer[0] == 0x3c) { // '<'
                    reading = 1;
                    usartData.messageBuffer[0] = 0x3c;
                    //debug(usartData.messageBuffer[0]);
                    bufferLoc = 1;
                    DRV_USART_BufferAddRead(usartData.usartHandle, 
                                        &usartData.bufferReadHandle,
                                        usartData.usartBuffer, 
                                        1);
                } else {
                    DRV_USART_BufferAddRead(usartData.usartHandle, 
                                        &usartData.bufferReadHandle,
                                        usartData.usartBuffer, 
                                        1);
                }
            }
            else if(handle == usartData.bufferWriteHandle){
                //ack flag would be true if we sent an ack
                //We don't want to start the timer 
                if(!usartData.sentAck) {
                    Nop();
                    usartData.receivedAck = false;
                    xTimerStartFromISR(usartData.messageTimer, pdFALSE);
                }
            }
            break;

        case DRV_USART_BUFFER_EVENT_ERROR:

            // Error handling here.

            break;

        default:
            break;
    }
}

void messageTimerCallback(TimerHandle_t timer) {
    Nop();
    if(!usartData.receivedAck) {
        Nop();
        sendMessage(usartData.lastMessage, usartData.lastMessageSize);
        xTimerResetFromISR(usartData.messageTimer, pdFALSE);
    }
}

/*******************************************************************************
 End of File
 */