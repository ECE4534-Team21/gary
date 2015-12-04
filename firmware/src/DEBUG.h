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
#define USART_SEND_ACK ((unsigned char)0x22)  
#define USART_RECIEVE_MESSAGE ((unsigned char)0x23)
#define USART_RECEIVE_ACK ((unsigned char)0x24)      
#define USART_CALLBACK_EVENT ((unsigned char) 0x25)    
#define USART_TASK ((unsigned char) 0x26)
#define USART_START_SIGNAL ((unsigned char) 0x1)
#define USART_RESTART_SIGNAL ((unsigned char) 0x2)
#define USART_GAMEPLAY_SIGNAL ((unsigned char) 0x3)
#define USART_SCORE_CEILING_SIGNAL ((unsigned char) 0x4)
#define USART_STOP_SIGNAL ((unsigned char) 0x5)
    
#define SENSOR_READ_ADC ((unsigned char) 0x31) //sensor has 0x20-0x2f
#define SENSOR_TIMER_CALLBACK ((unsigned char) 0x32)

#define CONTROL_TASK ((unsigned char) 0x50)
//Purposes and messages
#define CONTROL_PURPOSE_TEST ((unsigned char) 0x51)
#define CONTROL_PURPOSE_RESTART ((unsigned char) 0x52)
#define CONTROL_PURPOSE_START ((unsigned char) 0x53)
#define CONTROL_PURPOSE_STOP ((unsigned char) 0x54)
    
#define OLED_RECEIVED_MESSAGE_ON_QUEUE ((unsigned char) 0x60)
#define OLED_TIMER_CALLBACK ((unsigned char) 0x61)
    
#define ROVER_RECEIVED_MESSAGE_ON_QUEUE ((unsigned char) 0x40)
#define ROVER_TIMER_CALLBACK ((unsigned char) 0x41)
#define ROVER_TASK ((unsigned char) 0x42)
    
/*These are the declarations for the from/purpose for encoding*/
#define SENSOR_TASK ((unsigned char) 0x30)
#define COIN_SENSOR_DATA ((unsigned char) 0x0)
#define LINE_SENSOR_DATA ((unsigned char) 0x1)
    
/*Messages defined for testing*/
#define MSG_TESTS_INIT ((unsigned char) 0x90)
#define MSG_TESTS_START ((unsigned char) 0x91)
#define MSG_TESTS_READY ((unsigned char) 0x92)
#define MSG_TESTS_DONE ((unsigned char) 0x93)

#include "system_config.h"
#include "system_definitions.h"
    
void initDebug();
void debug(unsigned char msg);

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

