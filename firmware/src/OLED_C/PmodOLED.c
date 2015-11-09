/************************************************************************/
/*																		*/
/*	oled.c	--	Graphics Driver Library for OLED Display				*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): Created											*/
/*																		*/
/************************************************************************/

#define _SUPPRESS_PLIB_WARNING 1

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <p32xxxx.h>
#include <plib.h>

#include "OLED_H/PmodOLED.h"
#include "OLED_H/OledChar.h"
#include "OLED_H/OledGrph.h"

#include "OLED_H/delay.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern BYTE		rgbOledFont0[];
extern BYTE		rgbOledFontUser[];
extern BYTE		rgbFillPat[];

extern int		xchOledMax;
extern int		ychOledMax;

/* Coordinates of current pixel location on the display. The origin
** is at the upper left of the display. X increases to the right
** and y increases going down.
*/
int		xcoOledCur;
int		ycoOledCur;

BYTE *	pbOledCur;			//address of byte corresponding to current location
int		bnOledCur;			//bit number of bit corresponding to current location
BYTE	clrOledCur;			//drawing color to use
BYTE *	pbOledPatCur;		//current fill pattern
int		fOledCharUpdate;

int		dxcoOledFontCur;
int		dycoOledFontCur;

BYTE *	pbOledFontCur;
BYTE *	pbOledFontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* This array is the offscreen frame buffer used for rendering.
** It isn't possible to read back frome the OLED display device,
** so display data is rendered into this offscreen buffer and then
** copied to the display.
*/
BYTE	rgbOledBmp[cbOledDispMax];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	OledHostInit();
void	OledDevInit();
void	OledDvrInit();
BYTE	Spi2PutByte(BYTE bVal);
void	OledPutBuffer(int cb, BYTE * rgbTx);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	OledInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display subsystem.
*/

void
OledInit()
	{

	/* Init the PIC32 peripherals used to talk to the display.
	*/
	OledHostInit();

	/* Init the memory variables used to control access to the
	** display.
	*/
	OledDvrInit();

	/* Init the OLED display hardware.
	*/
	OledDevInit();

	/* Clear the display.
	*/
	OledClear();

}

/* ------------------------------------------------------------ */
/***	OledHostInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Perform PIC32 device initialization to prepare for use
**		of the OLED display.
**		This is currently hard coded for the Cerebot 32MX4 and
**		SPI2. This needs to be generalized.
*/

void
OledHostInit()
	{
	unsigned int	tcfg;

	/* Initialize SPI port 2.
	*/
	SPI2CON = 0;
	SPI2BRG = SPI2BRG;				
	SPI2STATbits.SPIROV = 0;
	SPI2CONbits.CKP = 1;
	SPI2CONbits.MSTEN = 1;
	SPI2CONbits.ON = 1;

	/* Make pins RF4, RF5, and RF6 be outputs.
	*/
    //PORTSetBits(IOPORT_D, bitDataCmd);
	PORTSetBits(IOPORT_E, bitVddCtrl|bitVbatCtrl|bitDataCmd);
	PORTSetPinsDigitalOut(prtDataCmd, bitDataCmd);		//Data/Command# select
	PORTSetPinsDigitalOut(prtVddCtrl, bitVddCtrl);		//VDD power control (1=off)
	PORTSetPinsDigitalOut(prtVbatCtrl, bitVbatCtrl);		//VBAT power control (1=off)

	/* Make the RG9 pin be an output. On the Basic I/O Shield, this pin
	** is tied to reset.
	*/
	PORTSetBits(prtReset, bitReset);
	PORTSetPinsDigitalOut(prtReset, bitReset);

}

/* ------------------------------------------------------------ */
/***	OledDvrInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED software system
*/

void
OledDvrInit()
	{
	int		ib;

	/* Init the parameters for the default font
	*/
	dxcoOledFontCur = cbOledChar;
	dycoOledFontCur = 8;
	pbOledFontCur = rgbOledFont0;
	pbOledFontUser = rgbOledFontUser;

	for (ib = 0; ib < cbOledFontUser; ib++) {
		rgbOledFontUser[ib] = 0;
	}

	xchOledMax = ccolOledMax / dxcoOledFontCur;
	ychOledMax = crowOledMax / dycoOledFontCur;

	/* Set the default character cursor position.
	*/
	OledSetCursor(0, 0);

	/* Set the default foreground draw color and fill pattern
	*/
	clrOledCur = 0x01;
	pbOledPatCur = rgbFillPat;
	OledSetDrawMode(modOledSet);

	/* Default the character routines to automaticall
	** update the display.
	*/
	fOledCharUpdate = 1;

}

/* ------------------------------------------------------------ */
/***	OledDspInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display controller and turn the display on.
*/

void
OledDspInit()
	{

	/* We're going to be sending commands, so clear the Data/Cmd bit
	*/
	PORTClearBits(prtDataCmd, bitDataCmd);

	/* Start by turning VDD on and wait a while for the power to come up.
	*/
	PORTClearBits(prtVddCtrl, bitVddCtrl);
	DelayMs(1);

	/* Display off command
	*/
	Spi2PutByte(0xAE);

	/* Bring Reset low and then high
	*/
	PORTClearBits(prtReset, bitReset);
	DelayMs(1);
	PORTSetBits(prtReset, bitReset);

	/* Send the Set Charge Pump and Set Pre-Charge Period commands
	*/
	Spi2PutByte(0x8D);
	Spi2PutByte(0x14);

	Spi2PutByte(0xD9);
	Spi2PutByte(0xF1);

	/* Turn on VCC and wait 100ms
	*/
	PORTClearBits(prtVbatCtrl, bitVbatCtrl);
	DelayMs(100);

	/* Send the commands to invert the display. This puts the display origin
	** in the upper left corner.
	*/
	Spi2PutByte(0xA1);			//remap columns
	Spi2PutByte(0xC8);			//remap the rows

	/* Send the commands to select sequential COM configuration. This makes the
	** display memory non-interleaved.
	*/
	Spi2PutByte(0xDA);			//set COM configuration command
	Spi2PutByte(0x20);			//sequential COM, left/right remap enabled

	/* Send Display On command
	*/
	Spi2PutByte(0xAF);

}

/* ------------------------------------------------------------ */
/***	OledDevInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display controller and turn the display on.
*/

void
OledDevInit()
	{

	/* We're going to be sending commands, so clear the Data/Cmd bit
	*/
	PORTClearBits(prtDataCmd, bitDataCmd);

	/* Start by turning VDD on and wait a while for the power to come up.
	*/
	PORTClearBits(prtVddCtrl, bitVddCtrl);
	//DelayMs(1);

	/* Display off command
	*/
	Spi2PutByte(0xAE);

	/* Bring Reset low and then high
	*/
	PORTClearBits(prtReset, bitReset);
	//DelayMs(1);
	PORTSetBits(prtReset, bitReset);

	/* Send the Set Charge Pump and Set Pre-Charge Period commands
	*/
	Spi2PutByte(0x8D);
	Spi2PutByte(0x14);

	Spi2PutByte(0xD9);
	Spi2PutByte(0xF1);

	/* Turn on VCC and wait 100ms
	*/
	PORTClearBits(prtVbatCtrl, bitVbatCtrl);
	//DelayMs(100);

	/* Send the commands to invert the display.
	*/
	Spi2PutByte(0xA1);			//remap columns
	Spi2PutByte(0xC8);			//remap the rows

	/* Send the commands to select sequential COM configuration
	*/
	Spi2PutByte(0xDA);			//set COM configuration command
	Spi2PutByte(0x20);			//sequential COM, left/right remap enabled

	/* Send Display On command
	*/
	Spi2PutByte(0xAF);

}

/* ------------------------------------------------------------ */
/***	OledClear
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display. This clears the memory buffer and then
**		updates the display.
*/

void
OledClear()
	{

	OledClearBuffer();
	OledUpdate();

}

/* ------------------------------------------------------------ */
/***	OledClearBuffer
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display memory buffer.
*/

void
OledClearBuffer()
	{
	int			ib;
	BYTE *		pb;

	pb = rgbOledBmp;

	/* Fill the memory buffer with 0.
	*/
	for (ib = 0; ib < cbOledDispMax; ib++) {
		*pb++ = 0x00;
	}

}

/* ------------------------------------------------------------ */
/***	OledUpdate
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Update the OLED display with the contents of the memory buffer
*/

void
OledUpdate()
	{
	int		ipag;
	int		icol;
	BYTE *	pb;

	pb = rgbOledBmp;

	for (ipag = 0; ipag < cpagOledMax; ipag++) {

		PORTClearBits(prtDataCmd, bitDataCmd);

		/* Set the page address
		*/
		Spi2PutByte(0x22);		//Set page command
		Spi2PutByte(ipag);		//page number

		/* Start at the left column
		*/
		Spi2PutByte(0x00);		//set low nybble of column
		Spi2PutByte(0x10);		//set high nybble of column

		PORTSetBits(prtDataCmd, bitDataCmd);

		/* Copy this memory page of display data.
		*/
		OledPutBuffer(ccolOledMax, pb);
		pb += ccolOledMax;

	}

}

/* ------------------------------------------------------------ */
/***	OledPutBuffer
**
**	Parameters:
**		cb		- number of bytes to send/receive
**		rgbTx	- pointer to the buffer to send
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Send the bytes specified in rgbTx to the slave.

*/

void
OledPutBuffer(int cb, BYTE * rgbTx)
	{
	int		ib;
	BYTE	bTmp;

	/* Write/Read the data
	*/
	for (ib = 0; ib < cb; ib++) {
		/* Wait for transmitter to be ready
		*/
		while (SPI2STATbits.SPITBE == 0);

		/* Write the next transmit byte.
		*/
		SPI2BUF = *rgbTx++;

		/* Wait for receive byte.
		*/
		while (SPI2STATbits.SPIRBF == 0);
		bTmp = SPI2BUF;

	}

}

/* ------------------------------------------------------------ */
/***	Spi2PutByte
**
**	Parameters:
**		bVal		- byte value to write
**
**	Return Value:
**		Returns byte read
**
**	Errors:
**		none
**
**	Description:
**		Write/Read a byte on SPI port 2
*/

BYTE
Spi2PutByte(BYTE bVal)
	{
	BYTE	bRx;

	/* Wait for transmitter to be ready
	*/
	while (SPI2STATbits.SPITBE == 0);

	/* Write the next transmit byte.
	*/
	SPI2BUF = bVal;

	/* Wait for receive byte.
	*/
	while (SPI2STATbits.SPIRBF == 0);

	/* Put the received byte in the buffer.
	*/
	bRx = SPI2BUF;
	
	return bRx;

}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

