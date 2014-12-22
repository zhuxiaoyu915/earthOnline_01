#include "timer_queue.h"
//#include "mac.h"
//#include <avr/sfr_defs.h>
//各函数定义 
void  timer3queue_init(Timer_Queue_pointer queue_head)//初始化函数，主要用于设置好T1定时器的比较寄存器和分频系数，比较寄存器设为0xffff.并初始化队列第一个元素。
{
	// queue_head=NULL;
}

extern  uint8_t    Timer3_adjustInterval(uint16_t interval); 
extern  uint32_t 	Timer3_acquireInterval(uint16_t interval);
extern  void      Timer3_sethalfsymbol(uint16_t symbols);
extern   void Timer3_intDisable(void);
//extern   Timer_Queue_pointer head_of_timer_queue;

//NOTE:只支持16位定时
//有返回值，主要是因为没有把queue_head设成全局变量
//传递进来的queue_head可能被修改
Timer_Queue_pointer   timer3queue_start(Timer_Queue_pointer queue_head, BYTE timertype, BYTE framedsn, BYTE timer_reason, WORD ticks)//开始一个新的定时任务，主要功能是将当前任务插入到定时队列中，并设置好相应的值 
{
	//往定时队列中插入一个新的定时任务，需要设置定时队列元素的各结构域
       Timer_Queue_pointer   temp;
	Timer_Queue_pointer   p_temp1,p_temp2;
	DWORD  compare_reg;
	    temp=(Timer_Queue_pointer)malloc(sizeof(struct TimerM_timer_queue));
	    temp->next=NULL;
	    temp->timer_reason=timer_reason;
	    temp->timer_type=timertype;
	    temp->framedsn=framedsn;

	 if(queue_head==NULL)	   
	 	{
	 	     //if() 
 	            Timer3_sethalfsymbol(ticks);
		     compare_reg=(((uint32_t)ticks)<<1);
		     //if(compare_reg)	 
		     temp->ticks=(uint16_t)((compare_reg>65535)? (compare_reg-65535):compare_reg);
                   temp->ticksLeft=compare_reg;
 	    	     queue_head=temp;	 
		     //printf("\n***the ticks is %08x\n",temp->ticks);	 
		     
		     //printf("\n***the ticksleft  is %08lx\n",temp->ticksLeft);		 
		}
          else
		{
	              //插入到定时队列中去，按temp->ticks的顺序
                   compare_reg=Timer3_acquireInterval(ticks);
		     temp->ticks=(uint16_t)((compare_reg>65535)? (compare_reg-65535):compare_reg);
                   temp->ticksLeft=compare_reg;
	            p_temp1=queue_head;
                   p_temp2=queue_head;
			while((p_temp2!=NULL)&&((temp->ticksLeft)>(p_temp2->ticksLeft)))
				{
				    p_temp1=p_temp2;
				    p_temp2=p_temp1->next;	 
				}
			if(p_temp2!=queue_head)
				{
			           p_temp1->next=temp;
			           temp->next=p_temp2;
				}
			else
				//为头节点，需要调整物理定时器
				{
				     Timer3_adjustInterval(temp->ticks);
				     temp->next=queue_head;
				     queue_head=temp;
				}
		}
	return queue_head;
	
}

//传递进来的queue_head可能被修改
Timer_Queue_pointer  timer3quere_cancel(Timer_Queue_pointer queue_head, BYTE framedsn,BYTE timer_reason)//取消一个定时任务
{
	//依据取消的framehandle,如果是队列头，则需要重新设置比较寄存器的值，否则只需把它从队列中摘掉即可。
       Timer_Queue_pointer  p_temp1,p_temp2,p_temp3;
	p_temp1=queue_head;
	p_temp2=queue_head;
	p_temp3=queue_head;
	//printf("\ntimer head->framedsn is %d----\n",queue_head->framedsn);
	if(timer_reason!=WAIT_FOR_RESPONSE)
	{
	     while(((p_temp2->framedsn!=framedsn)||(p_temp2->timer_reason!=timer_reason))&&(p_temp2!=NULL))
		 {
		    p_temp1=p_temp2;
		    p_temp2=p_temp2->next;	 
		 }
	}
	else
	{
	     while((p_temp2->timer_reason!=timer_reason)&&(p_temp2!=NULL))
		 {
		    p_temp1=p_temp2;
		    p_temp2=p_temp2->next;	 
		 }	      
	}
	if(p_temp2==NULL)  
		error("\ncannot find an element in the timer_queue\n");
	else
		{
		     p_temp3=p_temp2->next;
		     if(p_temp2==queue_head)
		     	{
	                   if(p_temp2->next==NULL)
	                   	{
	                   	 //printf("\n\n");
	                   	 Timer3_intDisable();
				 //printf("\ntimer3 is disabled\n");		 
	                   	}		 
			     else
			     	{
			     	     //重新设置定时器
			            Timer3_adjustInterval(p_temp3->ticks);
			     	}
			     //取下head节点,释放空间
			     queue_head=p_temp3;
			     free(p_temp2); 
		     	}
		     else
		     	{
		     	      p_temp1->next=p_temp3;
		     	      free(p_temp2);
		     	}
		}
	return queue_head;
}

//调用的MAC.h中的函数,进行相应的处理。
extern  void mac_timer_task(void);
extern  void timerqueue_adjust(void);
extern  uint16_t mac_timer3queue_getnextinterval(void);


uint16_t  timer3queue_getnextinterval(void)
{
        return mac_timer3queue_getnextinterval();
}
void  timer3queue_interrupt(void)
{
       timerqueue_adjust();
       OS_post(mac_timer_task);      
}


//T1时钟中断服务程序。当比较寄存器设置的值和计数器当前值相等时候，产生中断服务程序。
/*void __attribute((interrupt))   __vector_12(void)
{
   //硬件寄存器的设置，以及定时队列的处理，重新设置比较寄存器。暂时不摘掉队列头元素，作为参数进行传递，在mac_timer_task中进行摘除
   
	 //调用mac_timer_task开始进行处理,通过post任务的形式调用。
	 
}
*/
