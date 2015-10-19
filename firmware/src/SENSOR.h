/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    sensor.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _SENSOR_H
#define _SENSOR_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "DEBUG.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

#define DRV_ADC_INDEX_0 0
 
/*#define LINE_SENSOR_RIGHT 3
#define LINE_SENSOR_MIDDLE 4
#define LINE_SENSOR_LEFT 5*/
   
//ONSENSOR
//WHITE - 5v
//RED - 5V
//BROWN - GROUND
//YELLOW - PIN 3
//GREEN - PIN 4
//BLUE - PIN 5
#define LINE_SENSOR_RIGHT_PORT PORT_CHANNEL_C //YELLOW - CHIPKIT #11 - SENSOR #3
#define LINE_SENSOR_MIDDLE_PORT PORT_CHANNEL_A //GREEN CHIPKIT #12 - SENSOR #4
#define LINE_SENSOR_LEFT_PORT PORT_CHANNEL_D //BLUE- CHIPKIT #10 SENSOR #5
    
#define LINE_SENSOR_RIGHT_BIT PORTS_BIT_POS_4
#define LINE_SENSOR_MIDDLE_BIT PORTS_BIT_POS_2
#define LINE_SENSOR_LEFT_BIT PORTS_BIT_POS_4

#define LED4_PORT PORT_CHANNEL_A
#define LED4_BIT PORTS_BIT_POS_3
    
#define LED5_PORT PORT_CHANNEL_C
#define LED5_BIT PORTS_BIT_POS_1
    
#define LINE_SENSOR_RIGHT_VALUE (int)PLIB_PORTS_PinGet (PORTS_ID_0, LINE_SENSOR_RIGHT_PORT, LINE_SENSOR_RIGHT_BIT)
#define LINE_SENSOR_MIDDLE_VALUE (int)PLIB_PORTS_PinGet (PORTS_ID_0, LINE_SENSOR_MIDDLE_PORT, LINE_SENSOR_MIDDLE_BIT)
#define LINE_SENSOR_LEFT_VALUE (int)PLIB_PORTS_PinGet (PORTS_ID_0, LINE_SENSOR_LEFT_PORT, LINE_SENSOR_LEFT_BIT)

#define SET_LED4 PLIB_PORTS_PinSet(PORTS_ID_0, LED4_PORT, LED4_BIT);
#define CLEAR_LED4 PLIB_PORTS_PinClear(PORTS_ID_0, LED4_PORT, LED4_BIT);
    
#define SET_LED5 PLIB_PORTS_PinSet(PORTS_ID_0, LED5_PORT, LED5_BIT);
#define CLEAR_LED5 PLIB_PORTS_PinClear(PORTS_ID_0, LED5_PORT, LED5_BIT);
 
typedef enum
{
	/* Application's state machine's initial state. */
	SENSOR_STATE_INIT=0,
            SENSOR_STATE_RUNNING=1,

	/* TODO: Define states used by the application state machine. */

} SENSOR_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    TimerHandle_t sensorTimer;
    QueueHandle_t sensorQueue;
    SENSOR_STATES state;

    /* TODO: Define any additional data used by the application. */


} SENSOR_DATA;

SENSOR_DATA sensorData;
void sensorTimerCallback(TimerHandle_t timer);
void initLineSensor();
char readLineSensor();


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SENSOR_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SENSOR_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void SENSOR_Initialize ( void );


/*******************************************************************************
  Function:
    void SENSOR_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SENSOR_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void SENSOR_Tasks( void );


#endif /* _SENSOR_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

