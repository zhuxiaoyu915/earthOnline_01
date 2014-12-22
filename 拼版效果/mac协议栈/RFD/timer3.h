#ifndef _TIMER3_H
#define _TIMER3_H
#include "top.h"
      
	//uint8_t Timer3_mscale;
	//uint8_t Timer1_nextScale;
	//uint16_t Timer1_maxinterval;



void       Timer3_intDisable(void);

uint8_t  Timer3_fire(void);

uint8_t  Timer3_setIntervalAndScale(uint16_t interval, uint8_t scale);

void      Timer3_sethalfsymbol(uint16_t symbols);
	
uint32_t 	Timer3_acquireInterval(uint16_t interval);

uint8_t    Timer3_adjustInterval(uint16_t interval);

uint16_t  Timer3_getnextInterval(void);
		
#endif

