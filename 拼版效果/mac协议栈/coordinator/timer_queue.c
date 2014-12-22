#include "timer_queue.h"
//#include "mac.h"
//#include <avr/sfr_defs.h>
//���������� 
void  timer3queue_init(Timer_Queue_pointer queue_head)//��ʼ����������Ҫ�������ú�T1��ʱ���ıȽϼĴ����ͷ�Ƶϵ�����ȽϼĴ�����Ϊ0xffff.����ʼ�����е�һ��Ԫ�ء�
{
	// queue_head=NULL;
}

extern  uint8_t    Timer3_adjustInterval(uint16_t interval); 
extern  uint32_t 	Timer3_acquireInterval(uint16_t interval);
extern  void      Timer3_sethalfsymbol(uint16_t symbols);
extern   void Timer3_intDisable(void);
//extern   Timer_Queue_pointer head_of_timer_queue;

//NOTE:ֻ֧��16λ��ʱ
//�з���ֵ����Ҫ����Ϊû�а�queue_head���ȫ�ֱ���
//���ݽ�����queue_head���ܱ��޸�
Timer_Queue_pointer   timer3queue_start(Timer_Queue_pointer queue_head, BYTE timertype, BYTE framedsn, BYTE timer_reason, WORD ticks)//��ʼһ���µĶ�ʱ������Ҫ�����ǽ���ǰ������뵽��ʱ�����У������ú���Ӧ��ֵ 
{
	//����ʱ�����в���һ���µĶ�ʱ������Ҫ���ö�ʱ����Ԫ�صĸ��ṹ��
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
	              //���뵽��ʱ������ȥ����temp->ticks��˳��
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
				//Ϊͷ�ڵ㣬��Ҫ��������ʱ��
				{
				     Timer3_adjustInterval(temp->ticks);
				     temp->next=queue_head;
				     queue_head=temp;
				}
		}
	return queue_head;
	
}

//���ݽ�����queue_head���ܱ��޸�
Timer_Queue_pointer  timer3quere_cancel(Timer_Queue_pointer queue_head, BYTE framedsn,BYTE timer_reason)//ȡ��һ����ʱ����
{
	//����ȡ����framehandle,����Ƕ���ͷ������Ҫ�������ñȽϼĴ�����ֵ������ֻ������Ӷ�����ժ�����ɡ�
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
			     	     //�������ö�ʱ��
			            Timer3_adjustInterval(p_temp3->ticks);
			     	}
			     //ȡ��head�ڵ�,�ͷſռ�
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

//���õ�MAC.h�еĺ���,������Ӧ�Ĵ���
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


//T1ʱ���жϷ�����򡣵��ȽϼĴ������õ�ֵ�ͼ�������ǰֵ���ʱ�򣬲����жϷ������
/*void __attribute((interrupt))   __vector_12(void)
{
   //Ӳ���Ĵ��������ã��Լ���ʱ���еĴ����������ñȽϼĴ�������ʱ��ժ������ͷԪ�أ���Ϊ�������д��ݣ���mac_timer_task�н���ժ��
   
	 //����mac_timer_task��ʼ���д���,ͨ��post�������ʽ���á�
	 
}
*/
