/************************************************************************/
/*																		*/
/*	oled.h	--	Interface Declarations for OLED Display Driver Module	*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(PMODOLED_INC)
#define	PMODOLED_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */


#define	cbOledChar		8		//font glyph definitions is 8 bytes long
#define	chOledUserMax	0x20	//number of character defs in user font table
#define	cbOledFontUser	(chOledUserMax*cbOledChar)

/* Graphics drawing modes.
*/
#define	modOledSet		0
#define	modOledOr		1
#define	modOledAnd		2
#define	modOledXor		3

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */

/* Pin definitions for access to OLED control signals on Cerebot 32MX4
*/
/*#define prtSelect	IOPORT_G
#define	prtDataCmd	IOPORT_D
#define	prtReset	IOPORT_D
#define	prtVbatCtrl IOPORT_D
#define	prtVddCtrl	IOPORT_C

#define bitSelect	BIT_9
#define bitDataCmd	BIT_12
#define	bitReset	BIT_3
#define	bitVbatCtrl	BIT_4
#define	bitVddCtrl	BIT_4*/

/* ------------------------------------------------------------ */
/* Pin definitions for access to OLED control signals on chipKIT Uno32
*/

#define	prtVddCtrl	IOPORT_E
#define	prtVbatCtrl	IOPORT_E
#define	prtDataCmd	IOPORT_E
#define	prtReset	IOPORT_E

#define	bitVddCtrl	BIT_0
#define	bitVbatCtrl	BIT_2
#define	bitDataCmd	BIT_6
#define	bitReset	BIT_4

typedef unsigned char BYTE;

/* ------------------------------------------------------------ */
/* Symbols describing the geometry of the display. */

#define	cbOledDispMax	512		//max number of bytes in display buffer

#define	ccolOledMax		128		//number of display columns
#define	crowOledMax		32		//number of display rows
#define	cpagOledMax		4		//number of display memory pages

/* ------------------------------------------------------------ */

/* This array is the off-screen frame buffer used for rendering.
** It isn't possible to read back from the OLED display device,
** so display data is rendered into this off-screen buffer and then
** copied to the display.
*/
BYTE	rgbOledBmp[cbOledDispMax];

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	OledInit();
void	OledClear();
void	OledClearBuffer();
void	OledUpdate();

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
