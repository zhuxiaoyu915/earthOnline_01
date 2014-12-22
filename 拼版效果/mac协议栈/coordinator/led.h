#ifndef LED_H
#define LED_H

#include "top.h"
 
	
	//------------------------led
	extern uint8_t Leds_init(void);
	uint8_t Leds_yellowOn(void);
	uint8_t Leds_yellowOff(void);
	uint8_t Leds_yellowToggle(void);
	uint8_t Leds_greenOn(void);
	uint8_t Leds_greenOff(void);
	uint8_t Leds_greenToggle(void);
	uint8_t Leds_redOn(void);
	uint8_t Leds_redOff(void);
	uint8_t Leds_redToggle(void);
 
   
#endif


