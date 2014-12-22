#ifndef _SYSTIME_H
#define _SYSTIME_H

#include "top.h"
uint16_t currentTime;
void systime_init(void);
void systime_stop(void);
uint32_t systime32(void);
uint16_t systime16L(void);
uint16_t systime16H(void);
//---------------------------------------------------------------------------
//   change timer1 to be systimer,timer 3 for other usage  modified by zhouqiang in2005.12.04
//   this timer1 is used only for system time(timestamp,globle time)
//---------------------------------------------------------------------------
enum systimer_reason {

	BEACON_SEND  =0,        
	RECEIVE_BEACON  , 
       CAP_OVER,
       CSMA_TIMING,
       SYN_WAITTING_FOR_BEACON
           
};
struct systimer_queue {
	  uint8_t      timer_reason;   
	  uint16_t    ticks;           
	  uint32_t    ticksLeft;  
	  uint32_t      high_left;
	  uint32_t    high_ticks;
	  struct  systimer_queue *   next;
} ;

typedef struct systimer_queue *  systimer_queue_pointer ;
systimer_queue_pointer head_of_systimer_queue;
systimer_queue_pointer  systimer_start_task (systimer_queue_pointer queue_head, uint8_t  timer_reason, uint32_t  ticks) ;
systimer_queue_pointer      systimer_stop_task(systimer_queue_pointer  queue_head,uint8_t timer_reason);
void      systimer_task_fire(char timer_reason);

//---------------------------------------------------------------------------
//   need some codes for  timestamp,added by zhouqiang in 2005.12.04
//---------------------------------------------------------------------------

volatile uint16_t timestamp_H;
uint16_t get_timestamp16H(void);
uint16_t get_timestamp16L(void);
uint32_t get_timestamp32(void);

#endif

