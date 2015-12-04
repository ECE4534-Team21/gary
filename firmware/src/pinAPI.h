/* 
 * File:   pinAPI.h
 * Author: Chris Poole
 *
 * Created on November 12, 2015, 9:26 AM
 */

#ifndef PINAPI_H
#define	PINAPI_H

#include "system_config.h"
#include "system_definitions.h"

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

/*
 * Functions
 */
//Makes a PIN an output
void makePINOutput(mPIN pin);
//Makes a PIN an input
void makePINInput(mPIN pin);
//used to set a PIN as 0 or 1
void setPIN(mPIN pin, unsigned char val);
//Used to get a PIN value
int getPIN(mPIN pin);

#endif	/* PINAPI_H */

