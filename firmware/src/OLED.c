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

OLED_DATA oledData, oledAnimation;
struct Message incomingQueueMessage;

int coin_y = 0, coin_x = 15, game_x=0, game_y=0;
int score_counter = 0, coin_sensor_trigger = 3, PREVIOUS_SCORE = 0, delay_counter = 0;

int gameStart = 0, centiseconds = 0, seconds = 0, minutes = 0; 

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
    oledData.state = OLED_STATE_INIT;
    
    oledData.OLEDTimer = xTimerCreate("OLED Timer", 50 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, oledTimerCallback);
}

void OLED_Write_Score( int score )
{
    int score_1 = score/10, score_2 = score%10;
    
    char snum[3];
    char time[5];

    snum[0] = ':';
    snum[1] = controlData.scoreCeiling/10 + '0';
    snum[2] = controlData.scoreCeiling%10 + '0';
    snum[3] = '\0';

    time[0] = minutes/10+'0';
    time[1] = minutes%10+'0';
    time[2] = '<';
    time[3] = seconds/10+'0';
    time[4] = seconds%10+'0';
    time[5] = '\0';
    
    OledClearBuffer();
    
    OLED_Write_Num(3, OLED_NUM[score_1]);
    OLED_Write_Num(6, OLED_NUM[score_2]);
    
    OledSetCursor(9, 3);
    OledPutString(snum);
    OledSetCursor(11, 0);
    OledPutString(time);     
    OledUpdate();
    
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
}

/******************************************************************************
  Function:
    void OLED_Tasks ( void )

  Remarks:
    See prototype in oled.h.
 */

void OLED_Tasks ( void )
{
    switch ( oledData.state )
    {
        case OLED_STATE_INIT:
        {
            PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_9);
            PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_E, 0xFF);
            PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_E, 0x0); 
            
            DelayInit();
            OledInit(); 
            
            oledData.state = OLED_STATE_BEFORE_START;    
            
            oledData.OLEDQueue = xQueueCreate(CONTROLQUEUE_SIZE, sizeof( unsigned int ) );
            xTimerStart(oledData.OLEDTimer, 20);
                               
            break;
        }
        
        case OLED_STATE_CHECK_SENSOR:
        {
            if (gameStart == 1) {
                
                char time[5];
                
                time[0] = minutes/10+'0';
                time[1] = minutes%10+'0';
                time[2] = '<';
                time[3] = seconds/10+'0';
                time[4] = seconds%10+'0';
                time[5] = '\0';
                
                OledSetCursor(11, 0);
                OledPutString(time);     
                OledUpdate();
            }
            
            unsigned int receivedValue;
            xQueueReceive( oledData.OLEDQueue, &receivedValue, portMAX_DELAY ); //blocks until there is a character in the queue
            incomingQueueMessage = decode(receivedValue);
            
            if (incomingQueueMessage.from == CONTROL_TASK && (incomingQueueMessage.purpose == CONTROL_PURPOSE_START || incomingQueueMessage.purpose == CONTROL_PURPOSE_RESTART)){
                oledData.state = OLED_STATE_CHECK_SENSOR;
                gameStart = 1;
                score_counter = 0;
                centiseconds = 0;
                seconds = 0;
                minutes = 0;
                OledClearBuffer();
                OLED_Write_Score(score_counter);
            }
            else if (incomingQueueMessage.from == CONTROL_TASK && incomingQueueMessage.purpose == CONTROL_PURPOSE_STOP){
                oledData.state = OLED_STATE_BEFORE_START;
                gameStart = 0;
                score_counter = 0;
                centiseconds = 0;
                seconds = 0;
                minutes = 0;
                delay_counter = 0;
                //stop timer
            }
            else if(incomingQueueMessage.from == SENSOR_TASK && incomingQueueMessage.purpose == COIN_SENSOR_DATA && gameStart == 1){

                if (incomingQueueMessage.message == 1)  {
                    coin_sensor_trigger += 1;
                    oledData.state = OLED_STATE_CHECK_SENSOR;
                }
                else if (incomingQueueMessage.message == 0) {
                    if (coin_sensor_trigger >= 5)   {
                        oledData.state = OLED_STATE_DISPLAY_SCORE;
                        coin_sensor_trigger = 0;
                    }
                }
            }
            break;
        }
        case OLED_STATE_DISPLAY_SCORE:
        {
            score_counter += 1;
            
            OledClearBuffer();
            OLED_Write_Score(score_counter);
            oledData.state = OLED_STATE_CHECK_SENSOR;
            
            if (score_counter == controlData.scoreCeiling)    {
                gameStart = 2;
                //display winning message
                unsigned int gameover = 1;
                gameover = encode(OLED_TASK, OLED_GAMEOVER, gameover);
                xQueueSend(roverData.roverQueue, &gameover, pdTRUE);
                
                delay_counter = 0;
                game_x = 0;
                game_y = 0;
                oledAnimation.state = OLED_STATE_GAMEOVER;
                
            }
        }
        case OLED_STATE_BEFORE_START:
        {
            oledAnimation.state = OLED_STATE_RUNNING;
            //oledAnimation.state = OLED_STATE_GAMEOVER;
            oledData.state = OLED_STATE_CHECK_SENSOR;
            break;
        }
        default:
        {
            break;
        }
    }
}

void oledTimerCallback(TimerHandle_t timer){
        
    if (gameStart == 1) {
        oledAnimation.state = OLED_STATE_HOLD;
        centiseconds += 1;
        if (centiseconds == 13)  {
            centiseconds = 0;
            seconds += 1;
            if (seconds == 60)   {
                seconds = 0;
                minutes += 1;
            }
        }
            
    }
    else if (gameStart == 2)
        oledAnimation.state = OLED_STATE_GAMEOVER;
             
    switch ( oledAnimation.state )
    {      
        case OLED_STATE_RUNNING:
        {
            if (delay_counter == 0) {
                OledClearBuffer();
                OledSetCursor(0, 0);
                OledPutString("ECE 4534");
                OledSetCursor(0, 2);
                OledPutString("Team 21 GARY");
                OledUpdate();

                oledAnimation.state = OLED_STATE_RUNNING;
                delay_counter += 1;
            }
            else if (delay_counter < 15)   {
                delay_counter += 1;
                oledAnimation.state = OLED_STATE_RUNNING;
            }
            else    {
                delay_counter = 0;
                oledAnimation.state = OLED_STATE_ANIMATION_INIT;
            }
                        
            break;
        }
        case OLED_STATE_ANIMATION_INIT:
        {
            if (delay_counter == 0) {
                OledClearBuffer();
                OledSetCursor(2, 2);
                OledPutString("\x22\x23");
                OledSetCursor(2, 3);
                OledPutString("\x24\x25");
                OledUpdate();

                coin_x = 15;
                coin_y = 0;

                oledAnimation.state = OLED_STATE_ANIMATION_INIT;
                delay_counter += 1;
            }
            else if (delay_counter < 7)   {
                delay_counter += 1;
                oledAnimation.state = OLED_STATE_ANIMATION_INIT;
            }
            else    {
                delay_counter = 0;
                oledAnimation.state = OLED_STATE_ANIMATION_1;
            }
                                    
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
                oledAnimation.state = OLED_STATE_ANIMATION_3;
            else
                oledAnimation.state = OLED_STATE_ANIMATION_2;
                            
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
            oledAnimation.state = OLED_STATE_ANIMATION_1;

            break;
        }
        case OLED_STATE_ANIMATION_3:
        { 
            OledSetCursor(coin_x, coin_y);
            OledPutString("\x28\x29");
            OledUpdate();
            
            oledAnimation.state = OLED_STATE_ANIMATION_INIT;
                       
            break;              
        }
        case OLED_STATE_GAMEOVER:
        {
            if  (delay_counter > 15 && delay_counter % 2 == 1)   {
                int x2 = game_x+2, y2 = game_y+1;
                OledClearBuffer();
                OledSetCursor(game_x, game_y);
                OledPutString("Game Over");
                
                if (x2 >= 16)   {
                    x2 -= 16;
                    y2 += 1;
                }
                if (y2 >= 4)    {
                    y2 -= 4;
                }
                
                char time[5];
                
                time[0] = minutes/10+'0';
                time[1] = minutes%10+'0';
                time[2] = '<';
                time[3] = seconds/10+'0';
                time[4] = seconds%10+'0';
                time[5] = '\0';
                
                OledSetCursor(x2, y2);
                OledPutString(time);     
                OledUpdate();
                                
                game_x += 1;

                if (game_x == 16) {
                    game_x = 0;
                    game_y += 1;
                }
                if (game_y == 4)
                    game_y = 0;
            }
                
            delay_counter += 1;
            if (delay_counter == 750)  {
                oledAnimation.state = OLED_STATE_RUNNING;
                delay_counter = 0;
                gameStart = 0;
            } else  {
                oledAnimation.state = OLED_STATE_GAMEOVER;
            }
            break;
        }
        case OLED_STATE_HOLD:
        {
            oledAnimation.state = OLED_STATE_HOLD;
            break;
        }
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
    
 }

 

/*******************************************************************************
 End of File
 */
