/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    rover.c

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

#include "rover.h"
#include "motorAPI.h"

mMotor left_motor;
mMotor right_motor;

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

//ROVER_DATA roverData;

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
    void ROVER_Initialize ( void )

  Remarks:
    See prototype in rover.h.
 */

void ROVER_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    roverData.state = ROVER_STATE_INIT;
    roverData.roverTimer = xTimerCreate("Rover Timer", 200 / portTICK_PERIOD_MS, pdTRUE, (void *) 1, roverTimerCallback);
    left_motor = getMotor(0);
    right_motor = getMotor(1);
    
    initMotors(&left_motor, &right_motor);
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void ROVER_Tasks ( void )

  Remarks:
    See prototype in rover.h.
 * 
 */

bool enableMotors = true; //ROVER GLOBAL PARAMETER - choose to run the motors or not
bool enableLineSensorRoverControl = true; //ROVER GLOBAL PARAMTER - choose to use the LED4 and LED5

void turnRight(){
    //leftMotorPWM = leftMotorPWM + 100;
    //rightMotorPWM = rightMotorPWM - 100;
    if(enableMotors){
        motorSetDir(left_motor, FORWARD);
        motorSetDir(right_motor, FORWARD);
        motorSetPWM(right_motor, 300);
        motorSetPWM(left_motor, 1000);
    }
};
void turnLeft(){
    /*leftMotorPWM = leftMotorPWM - 100;
    rightMotorPWM = rightMotorPWM + 100;*/
    if(enableMotors){
        motorSetDir(left_motor, FORWARD);
        motorSetDir(right_motor, FORWARD);
        motorSetPWM(right_motor, 1000);
        motorSetPWM(left_motor, 300);
    }
};

void turnRightFast(){
    if(enableMotors){
        motorSetDir(left_motor, FORWARD);
        motorSetDir(right_motor, BACKWARD);
        motorSetPWM(right_motor, 1000);
        motorSetPWM(left_motor, 1000);
    }
}

void turnLeftFast(){
    if(enableMotors){
        motorSetDir(left_motor, BACKWARD);
        motorSetDir(right_motor, FORWARD);
        motorSetPWM(right_motor, 1000);
        motorSetPWM(left_motor, 1000);
    }
}
void go(){
    if(enableMotors){
        motorSetDir(left_motor, FORWARD);
        motorSetDir(right_motor, FORWARD);
        motorSetPWM(right_motor, 1000);
        motorSetPWM(left_motor, 1000);
    }
};

void stop(){
    if(enableMotors){
        motorSetPWM(right_motor, 0);
        motorSetPWM(left_motor, 0);
    }
    //motorSetDir(left_motor, STOP);
    //motorSetDir(right_motor, STOP);
};

bool backOnLine(unsigned int lineSensorValue){
    if(lineSensorValue == 0x0005){
        return true;
    }
    return false;
}


void adjustMotorsFromLineSensor(unsigned int lineSensorValue){
    switch(lineSensorValue){
        //1 = BLACK, 0 = WHITE
        case 0x0000: //000 Time to Turn
            //turn();
            stop();
            roverData.state = ROVER_STATE_TURNING;
            break;
        case 0x0005: //101 ON TRACK
            SET_LED4;
            SET_LED5;
            go();
            break;
        case 0x0006: //110 LEFT OF CENTER, GO RIGHT
            SET_LED5;
            CLEAR_LED4;
            turnRightFast();
            break;
        case 0x0004: //100 LEFT OF CENTER, GO RIGHT
            SET_LED5;
            CLEAR_LED4;
            turnRight();
            break;
        case 0x0003: //011 RIGHT OF CENTER, GO LEFT
            CLEAR_LED5;
            SET_LED4;
            turnLeftFast();
            break;
        case 0x0001: //001 RIGHT OF CENTER, GO LEFT
            CLEAR_LED5;
            SET_LED4;
            turnLeft();
            break;
        case 0x0007: //111 STOP because on black
            CLEAR_LED5;
            CLEAR_LED4;
            stop();
            break;
        default: //else
            //CLEAR_LED4;  
            //CLEAR_LED5;
            break;
    }
}

void ROVER_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( roverData.state )
    {
        /* Application's initial state. */
        case ROVER_STATE_INIT:
        {
            roverData.roverQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            ROVERQUEUE_SIZE, //number of slots in the queue
                            /* The size of each item the queue holds. */
                            sizeof( unsigned int ) );
            xTimerStart(roverData.roverTimer, 200);
            if(enableMotors){
                motorSetDir(left_motor, FORWARD);
                motorSetDir(right_motor, FORWARD);
                motorSetPWM(right_motor, 500);
                motorSetPWM(left_motor, 500);
            }
            roverData.state = ROVER_STATE_RUNNING;
            break;
        }
        
        case ROVER_STATE_RUNNING:
        {
            unsigned int lineSensorValue;
            xQueueReceive(roverData.roverQueue, &lineSensorValue, portMAX_DELAY );
            //go();
            if(enableLineSensorRoverControl){
                adjustMotorsFromLineSensor(lineSensorValue);
            }
            break;
        }
        
        case ROVER_STATE_TURNING:
        {   
            TOGGLE_LED5;
            TOGGLE_LED4;
            unsigned int lineSensorValue;
            if(enableMotors){
                motorSetDir(left_motor, FORWARD);
                motorSetDir(right_motor, BACKWARD);
                motorSetPWM(right_motor, 1000);
                motorSetPWM(left_motor, 1000);
            }
            xQueueReceive(roverData.roverQueue, &lineSensorValue, portMAX_DELAY );
            if(backOnLine(lineSensorValue)){
                roverData.state = ROVER_STATE_RUNNING;
                if(enableMotors){
                    motorSetDir(left_motor, FORWARD);
                    motorSetDir(right_motor, FORWARD);
                    motorSetPWM(right_motor, 1000);
                    motorSetPWM(left_motor, 1000);
                }
            }
            else{
                roverData.state = ROVER_STATE_TURNING;
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
 
void roverTimerCallback(TimerHandle_t timer){
    //debug(ROVER_TIMER_CALLBACK);
}

/*******************************************************************************
 End of File
 */
