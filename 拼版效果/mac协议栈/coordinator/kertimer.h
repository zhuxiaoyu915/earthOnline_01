#ifndef _TIMER0_H
#define _TIMER0_H
#include <avr/signal.h>


#include "top.h"
//kertimer use timer0
/** 
   *  set Timer/Counter0 to be asynchronous 
   *  from the CPU clock with a second external 
   *  clock(32,768kHz)driving it
 **/

#define  KER_TIMERS  3
//-------------------------------kertime setup------------------added by zhouqiang in 2005.12.02
uint32_t mState;		// each bit represent a timer state 
uint8_t   setIntervalFlag; 
uint8_t   mScale, mInterval;
int8_t     queue_head;
int8_t     queue_tail;
uint8_t   queue_size;
uint8_t   queue[KER_TIMERS];

struct timer_s {
        uint8_t type;		// one-short or repeat timer
        int32_t ticks;		// clock ticks for a repeat timer 
        int32_t ticksLeft;	// ticks left before the timer expires
        void (*tp)(void);//fire to what
    } mTimerList[KER_TIMERS];

enum {
		maxTimerInterval = 230
          };

enum kertimer_type_value {
  KERTIMER_REPEAT = 0, 
  KERTIMER_ONE_SHOT = 1, 
  
};
//------------------------------------------------------------added by zhouqiang in 2005.12.02
void kertimer_init(void);
uint8_t  kertimer_start (uint8_t  id,uint8_t type, uint32_t  interval,void (* fun)(void)) ;
void kertimer_stop(uint8_t id);
void KERTimer_Fire(void) ;
#endif 

