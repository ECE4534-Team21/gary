/* 
 * File:   motorAPI.h
 * Author: Chris Poole
 *
 * Created on October 6, 2015, 11:07 AM
 */

#ifndef MOTORAPI_H
#define	MOTORAPI_H
/*
 * Includes
 */
#include "peripheral/tmr/plib_tmr.h"
#include "peripheral/oc/plib_oc.h"
#include "system_config.h"
#include "system_definitions.h"

/*
 * Structs and variables
 */
//The max PWM value we are aiming for
//int MOTOR_MAX =  5000;

//Pin struct
typedef struct
{
   /*Port Module ID*/
   PORTS_MODULE_ID portID;
    
   /*Channel*/
   PORTS_CHANNEL channel;
   
   /*Bit position*/
   PORTS_BIT_POS bitPos;
    
} mPIN;

//Used for motor direction
typedef enum {
    FORWARD=0,
    BACKWARD=1,
    STOP=2
} motorDir;

typedef struct {
    
    //ID of the OC module for the motor
    OC_MODULE_ID ocID;
    
    //ID of the timer for this motor PWM
    TMR_MODULE_ID tmrID;
    
    //Frequency of the PWM
    int pwmFreq;
    
    //current value of the PWM
    int pwmVal;
    
    //Direction of the motor
    motorDir direction;
    
    //Direction PIN
    mPIN pinDirection;
    
    //Enable PIN
    mPIN pinEnable;
    
} mMotor;

//The motors
mMotor motors[2];

/* 
 * Setup / Initialization functions
 */
//Makes a PIN an output
void makePINOutput(mPIN pin);
//used to set a PIN as 0 or 1
void setPIN(mPIN pin, unsigned char val);

//Initialize the motors, left and right
void initMotors(mMotor * left, mMotor * right);

//Used to initialize a single motor
void initMotor(mMotor motor);

//Used to get a motor
mMotor getMotor(int motor);

//set motor information up
void setMotorOC(mMotor * motor, OC_MODULE_ID oc);
void setMotorTimer(mMotor * motor, TMR_MODULE_ID timer);
void setMotorEnablePin(mMotor * motor, PORTS_MODULE_ID module, PORTS_CHANNEL channel,
        PORTS_BIT_POS bitPos);
void setMotorDirPin(mMotor * motor, PORTS_MODULE_ID module, PORTS_CHANNEL channel,
        PORTS_BIT_POS bitPos);

//Used to control motors
void motorSetDir(mMotor motor, motorDir dir);
//PWM is 0 - 100
void motorSetPWM(mMotor motor, double pwm);

#endif	/* MOTORAPI_H */

