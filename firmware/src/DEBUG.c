
#include "debug.h"

//Initialize the debugger
void initOLED() {
    //Set Port E to output
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_E, 0xFF);
    //PLIB_PORTS_PinSet (PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_E, 0x0);
}

//Write a debug msg over Port E
//See debug.h for constants
void debug(unsigned char msg) {
    //PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_E, msg);
}
