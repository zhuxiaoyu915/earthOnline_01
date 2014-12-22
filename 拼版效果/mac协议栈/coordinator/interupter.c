#include "interupter.h"

//---------------------------------------------------------fifop中断(pe6)
void __attribute((signal))   __vector_7(void)
{
	
               cc2420_fifopintr();
	
}

//----------------------------------------------------------定时器1 比较a 中断
void __attribute((interrupt))   __vector_12(void)
{
   uint16_t  temp_interval;
   uint8_t fire_reason;
   systimer_queue_pointer temp;
   uint32_t test;
   test=systime32();
   if(((head_of_systimer_queue->ticksLeft)&(0xffff0000))==(test&(0xffff0000)))
   	{
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
	   //if(((head_of_systimer_queue->ticksLeft)&&(0xffff0000))==((systime32())&&(0xffff0000)))	
  	   fire_reason=head_of_systimer_queue->timer_reason;
	   temp=head_of_systimer_queue;
	   //test=systime32();
	   //printf("\nthe current timer is %08x",test);
          temp_interval=Timer1_getnextInterval();
	   head_of_systimer_queue=head_of_systimer_queue->next;
	   //temp_interval=TCNT1+100;
	   free(temp);
          if(temp_interval!=0)
	      OCR1A=temp_interval;
	   else
	   	{    
	   	     //printf("\ntimer1 disable");
	            Timer1_OCR_intDisable();
	   	}
	    
      }
    os_atomic_end(_nesc_atomic); }
    	
    systimer_task_fire(fire_reason);
   	}
   /*
   else
   	{ 
   	if(((head_of_systimer_queue->ticksLeft)&(0x3fff0000))==(test&(0x3fff0000)))
   	      head_of_systimer_queue->high_left=head_of_systimer_queue->high_left-0x40000000;
   	OCR1A=head_of_systimer_queue->ticks;
   	}
   	*/
}
void __attribute((interrupt))   __vector_11(void)//定时器1-- input capture interrupt
{
   
  { os_atomic_t _nesc_atomic = os_atomic_start();
      {      
          timestamp_H=systime16H();
      }
    os_atomic_end(_nesc_atomic); }
    
}

void __attribute((interrupt))   __vector_14(void)//定时器1-- overflow interrupt
{
   
  { os_atomic_t _nesc_atomic = os_atomic_start();
      {      
         //++currentTime;
	  /*if(currentTime==0x3fffffff)
	  	currentTime=0;
	  else*/
	  	++currentTime;
      }
    os_atomic_end(_nesc_atomic); }
    
}

//----------------------------------------------------------定时器2中断
void __attribute((interrupt))   __vector_9(void)
{
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
     		timer2_intDisable();
      		backoff_timer_Set = FALSE;
    }
    
    os_atomic_end(_nesc_atomic); }
    backoff_timer_fire();
}

//----------------------------------------------------------定时器3  溢出 中断
/*void __attribute((interrupt))   __vector_29(void)
{

  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      
         
      }
       
    os_atomic_end(_nesc_atomic); }
   
}*/
//------------------------------------------------------定时器3  比较a 中断
//uint32_t  sys_time_1;
void __attribute((interrupt))   __vector_26(void)
{
   uint16_t  temp_interval;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    { 
	   //sys_time_1=systime32();
          temp_interval=Timer3_getnextInterval();
          if(temp_interval!=0)
	      OCR3A=temp_interval;
	   else
	      Timer3_intDisable();
      }
    os_atomic_end(_nesc_atomic); }
	
    Timer3_fire();
    //printf("\n^^^timer3 interrupt occur^^OCR3A is %04x^^^systime is %08lx\n",OCR3A,sys_time_1);	
}
//---------------------------------------------------------定时器0中断
void __attribute((interrupt))   __vector_15(void)
{
  { os_atomic_t _nesc_atomic = os_atomic_start();{
    
    OS_post(&KERTimer_Fire);
    }
    os_atomic_end(_nesc_atomic); }
    //printf("\n--timer0 interrupt----\n");
}




