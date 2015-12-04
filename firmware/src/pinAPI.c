#include "pinAPI.h"

/*
 * Used to make a mPIN an output
 */
void makePINOutput(mPIN pin) {
    PLIB_PORTS_PinDirectionOutputSet(pin.portID, pin.channel, pin.bitPos);
}

/*
 * Used to make a mPIN an input
 */
void makePINInput(mPIN pin) {
    PLIB_PORTS_PinDirectionInputSet(pin.portID, pin.channel, pin.bitPos);
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
 * Function to get the value of a pin, return 1 or 0
 */
int getPIN(mPIN pin) {
    return (int)PLIB_PORTS_PinGet(pin.portID, pin.channel, pin.bitPos);
}
