#include "motorAPI.h"

/*
 * Used to make a mPIN an output
 */
void makePINOutput(mPIN pin) {
    PLIB_PORTS_PinDirectionOutputSet(pin.portID, pin.channel, pin.bitPos);
}

/*
 * Function to set a PIN to a value, '0' or '1'
 * Anything >= 1 is counted as '1'
 */
void setPIN(mPIN pin, unsigned char val) {
    //Handle '1'
    if (val > 0) {
        PLIB_PORTS_PinSet(pin.portID, pin.channel, pin.bitPos);
    } else { //Handle '0'   
        PLIB_PORTS_PinClear(pin.portID, pin.channel, pin.bitPos);
    }
}

/*
 * This function initializes the left and right motors
 */
void initMotors(mMotor * left, mMotor * right) {
    
    //Left Motor
    setMotorOC(left, OC_ID_1);
    setMotorTimer(left, TMR_ID_2);
    setMotorEnablePin(left, PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    setMotorDirPin(left, PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    
    //Right Motor
    setMotorOC(right, OC_ID_2);
    setMotorTimer(right, TMR_ID_2);
    setMotorEnablePin(right, PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    setMotorDirPin(right, PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    
    initMotor(*left);
    initMotor(*right);
    
    //Start timers and OC drivers
    DRV_OC0_Start();
    DRV_OC1_Start();
    DRV_TMR0_Start();
}

/*
 * This function is used to initialize a motor
 */
void initMotor(mMotor motor) {
    
    //Setup enable and direction pins
    makePINOutput(motor.pinEnable);
    makePINOutput(motor.pinDirection);
    
    Nop();
    
    //Stop motor
    motorSetDir(motor, STOP);
    
}

/*
 * Functions used to set up the motor information
 */
void setMotorOC(mMotor * motor, OC_MODULE_ID oc) {
    
    motor->ocID = oc;
    
}
void setMotorTimer(mMotor * motor, TMR_MODULE_ID timer) {
    motor->tmrID = timer;
}
void setMotorEnablePin(mMotor * motor, PORTS_MODULE_ID module, PORTS_CHANNEL channel,
        PORTS_BIT_POS bitPos) {
    motor->pinEnable.portID = module;
    motor->pinEnable.channel = channel;
    motor->pinEnable.bitPos = bitPos;
}
void setMotorDirPin(mMotor * motor, PORTS_MODULE_ID module, PORTS_CHANNEL channel,
        PORTS_BIT_POS bitPos) {
    motor->pinDirection.portID = module;
    motor->pinDirection.channel = channel;
    motor->pinDirection.bitPos = bitPos;
}

/*
 * Functions used to control the motors
 */

void motorSetDir(mMotor motor, motorDir dir) {
    
    if (dir == STOP) {
        PLIB_OC_PulseWidth16BitSet(motor.ocID, 0);
    } else {
        setPIN(motor.pinDirection, dir);
    }
    
}
//PWM is 0 - 100
void motorSetPWM(mMotor motor, double pwm) {
    /*double percent = (double)100 / pwm;
    int val = percent * 5000;
    Nop();
    PLIB_OC_PulseWidth16BitSet(motor.ocID, val);*/
    Nop();
    PLIB_OC_PulseWidth16BitSet(motor.ocID, 1000+pwm);
}

/*unsigned int getMotorPWM(mMotor motor){
    
}*/

//Used to get a motor
mMotor getMotor(int motor) {
    return motors[motor];
}