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

#define USART_BLOCK_FOR_QUEUE ((unsigned char) 0x20) //uart has 0x00-0x0f
#define USART_SEND_MESSAGE ((unsigned char)0x21)  

#define SENSOR_TASK ((unsigned char) 0x30)
#define SENSOR_READ_ADC ((unsigned char) 0x31) //sensor has 0x20-0x2f
#define SENSOR_TIMER_CALLBACK ((unsigned char) 0x32)

#define CONTROL_RECEIVED_MESSAGE_ON_QUEUE ((unsigned char) 0x50)
    
#define OLED_RECEIVED_MESSAGE_ON_QUEUE ((unsigned char) 0x60)
#define OLED_TIMER_CALLBACK ((unsigned char) 0x61)

#include "system_config.h"
#include "system_definitions.h"
    
void initDebug();
void debug(unsigned char msg);

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

