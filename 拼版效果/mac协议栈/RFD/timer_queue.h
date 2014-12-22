#ifndef _TIMER_QUEUE_H
#define _TIMER_QUEUE_H

#include "generic.h"
//#include "project.h"

enum timer_type_value {
  TIMER_REPEAT = 0, 
  TIMER_ONE_SHOT = 1, 
  NUM_TIMERS = 3
};

enum timer_reason {
	WAIT_FOR_ACK  =0,        //定时长度对应于macAckWaitDuration个symbol
	WAIT_FOR_RESPONSE  =1,   //定时长度对应于aResponseWaitTime个symbol
	WAIT_FOR_DATA      =2,   //定时长度对应于aMaxFrameResponseTime个symbol
	WAIT_SCAN            =3 ,  //定时长度对应于aBaseSuperframeDuring*(2*n+1)个symbol
	WAIT_FOR_BEACON      =4 ,  //定时长度对应于超帧周期长度
  WAIT_FOR_MAX_INTERVAL =5, //定时长度对应于MAX_TIMER_INTERVAL
};

#define  MAX_TIMER_INTERVAL  0xffff
#define  PRE_DEFINED_SCALE   1024

#define  NULL    0x0000


//是个定时的队列
struct TimerM_timer_queue {
  BYTE    timer_type;      //是周期性的还是一次性的。
  BYTE    framedsn;     //区分该定时是针对哪个帧的。
  BYTE    timer_reason;    //指明定时原因，是ack定时，响应应答定时, 数据请求应答定时,或定时为0xffff
  WORD   ticks;           //从当前时刻算起，定时多少个时钟clock
  DWORD   ticksLeft;       //需要设置的比较寄存器的值。
  struct  TimerM_timer_queue *   next;  //指向下一个定时器队列元素
} ;
typedef struct TimerM_timer_queue *  Timer_Queue_pointer ;
//供高层调用的相关函数声明
void  timer3queue_init(Timer_Queue_pointer queue_head);//初始化函数，主要用于设置好T1定时器的比较寄存器和分频系数，比较寄存器设为0xffff.并初始化队列第一个元素。
Timer_Queue_pointer timer3queue_start(Timer_Queue_pointer queue_head, BYTE timertype, BYTE framedsn, BYTE timer_reason, WORD ticks);//开始一个新的定时任务，主要功能是将当前任务插入到定时队列中，并设置好相应的值 
Timer_Queue_pointer timer3quere_cancel(Timer_Queue_pointer queue_head, BYTE framedsn, BYTE timer_reason);//取消一个定时任务

//供底层调用的函数声明
uint16_t  timer3queue_getnextinterval(void);
void  timer3queue_interrupt(void);

//T1时钟中断服务程序。当比较寄存器设置的值和计数器当前值相等时候，产生中断服务程序。
/*void __attribute((interrupt))   __vector_12(void);*/
//中断服务程序产生一个新的任务
#endif