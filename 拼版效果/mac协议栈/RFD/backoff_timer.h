#ifndef backoff_timer_H
#define  backoff_timer_H

#include "top.h"
#include "cc2420.h"


bool backoff_timer_Set;
void (*backoff_call)(void);

//-------------------------------backofftime setup------------------added by zhouqiang in 2005.12.03

uint8_t backoff_timer_stop(void);
void     backoff_timer_fire(void);
void     backoff_timer_setOneShot(uint8_t _jiffy,void (*fun)(void));
	
   	
#endif
