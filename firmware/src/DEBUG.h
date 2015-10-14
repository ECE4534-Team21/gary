/* 
 * File:   debug.h
 * Author: Fushitsu
 *
 * Created on September 22, 2015, 11:21 AM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define USART_BLOCK_FOR_QUEUE ((unsigned char) 0x20) 
#define USART_SEND_MESSAGE ((unsigned char)0x02)   

#include "system_config.h"
#include "system_definitions.h"

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

