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
	WAIT_FOR_ACK  =0,        //��ʱ���ȶ�Ӧ��macAckWaitDuration��symbol
	WAIT_FOR_RESPONSE  =1,   //��ʱ���ȶ�Ӧ��aResponseWaitTime��symbol
	WAIT_FOR_DATA      =2,   //��ʱ���ȶ�Ӧ��aMaxFrameResponseTime��symbol
	WAIT_SCAN            =3 ,  //��ʱ���ȶ�Ӧ��aBaseSuperframeDuring*(2*n+1)��symbol
	WAIT_FOR_BEACON      =4 ,  //��ʱ���ȶ�Ӧ�ڳ�֡���ڳ���
  WAIT_FOR_MAX_INTERVAL =5, //��ʱ���ȶ�Ӧ��MAX_TIMER_INTERVAL
};

#define  MAX_TIMER_INTERVAL  0xffff
#define  PRE_DEFINED_SCALE   1024

#define  NULL    0x0000


//�Ǹ���ʱ�Ķ���
struct TimerM_timer_queue {
  BYTE    timer_type;      //�������ԵĻ���һ���Եġ�
  BYTE    framedsn;     //���ָö�ʱ������ĸ�֡�ġ�
  BYTE    timer_reason;    //ָ����ʱԭ����ack��ʱ����ӦӦ��ʱ, ��������Ӧ��ʱ,��ʱΪ0xffff
  WORD   ticks;           //�ӵ�ǰʱ�����𣬶�ʱ���ٸ�ʱ��clock
  DWORD   ticksLeft;       //��Ҫ���õıȽϼĴ�����ֵ��
  struct  TimerM_timer_queue *   next;  //ָ����һ����ʱ������Ԫ��
} ;
typedef struct TimerM_timer_queue *  Timer_Queue_pointer ;
//���߲���õ���غ�������
void  timer3queue_init(Timer_Queue_pointer queue_head);//��ʼ����������Ҫ�������ú�T1��ʱ���ıȽϼĴ����ͷ�Ƶϵ�����ȽϼĴ�����Ϊ0xffff.����ʼ�����е�һ��Ԫ�ء�
Timer_Queue_pointer timer3queue_start(Timer_Queue_pointer queue_head, BYTE timertype, BYTE framedsn, BYTE timer_reason, WORD ticks);//��ʼһ���µĶ�ʱ������Ҫ�����ǽ���ǰ������뵽��ʱ�����У������ú���Ӧ��ֵ 
Timer_Queue_pointer timer3quere_cancel(Timer_Queue_pointer queue_head, BYTE framedsn, BYTE timer_reason);//ȡ��һ����ʱ����

//���ײ���õĺ�������
uint16_t  timer3queue_getnextinterval(void);
void  timer3queue_interrupt(void);

//T1ʱ���жϷ�����򡣵��ȽϼĴ������õ�ֵ�ͼ�������ǰֵ���ʱ�򣬲����жϷ������
/*void __attribute((interrupt))   __vector_12(void);*/
//�жϷ���������һ���µ�����
#endif