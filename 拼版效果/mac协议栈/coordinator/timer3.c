#include "timer3.h"

extern  uint16_t  timer3queue_getnextinterval(void);
extern  void  timer3queue_interrupt(void);

 void Timer3_intDisable(void)
 {
      { os_atomic_t _nesc_atomic = os_atomic_start();
       	{
	 ETIMSK &= 0xef;
	 //TCCR3B = 5;
	 //OCR3A=0;
	 //TCNT3 = 0x0000;
	 //TCCR3B=0x00;
	ETIFR |=(1<<OCF3A);
	 
	}
      os_atomic_end(_nesc_atomic); }        	
 }

uint8_t  Timer3_fire(void)
{
          timer3queue_interrupt();
         //OS_post(timer3queue_interrupt);
}

uint8_t  Timer3_setIntervalAndScale(uint16_t interval, uint8_t scale)
{
    if (scale > 7) {
          error("\nscale is out of scope\n");
          return 0;
    }
//uint16_t  temp_test_tnt3;
//uint32_t  sys_time;
{ os_atomic_t _nesc_atomic = os_atomic_start();
    {
       //temp_test_tnt3=TCNT3 ;
	ETIFR |=(1<<OCF3A);   
	OCR3A = interval;//this is the scale
	OCR3B = 0x0000;
	OCR3C = 0x0000;
	TCNT3 = 0x0000; //this is the counter
	TCCR3A = 0x00;//Output: A: Disconnected, B: Disconnected, C: Disconnected,WGM[1..0]=00
	//scale |= 0x08;//
	TCCR3B = scale;//clk/(scale),WGM[3..2]=01
	TCCR3C = 0x00;//
	ETIMSK |= 0x10;//OCIE1A is enable 
	//sys_time=systime32();
    }
    os_atomic_end(_nesc_atomic); }
   //printf("\n**TCNT3 is %04x\n",temp_test_tnt3);	 	
   //printf("sys time is %08lx",sys_time);
  return SUCCESS;
}

//供高层调用的定时开始函数。
void      Timer3_sethalfsymbol(uint16_t symbols)
{
      uint32_t halfsymbols;
      halfsymbols=((uint32_t)symbols)<<1;
      if(halfsymbols<=65535)	  
      Timer3_setIntervalAndScale(halfsymbols, 0x5);
      else
      Timer3_setIntervalAndScale(halfsymbols-65535, 0x5);  	
}
//供高层调用获得下一次定时的比较寄存器应当设置的值
uint32_t 	Timer3_acquireInterval(uint16_t interval)
{
       uint32_t temp;
	uint16_t  tcnt_3;
	tcnt_3=TCNT3;
       temp=(uint32_t)tcnt_3+(((uint32_t)interval)<<1);
	// (temp>=65535) temp=temp-65535;
	return (temp);
}
uint8_t    Timer3_adjustInterval(uint16_t interval)
{
   { os_atomic_t _nesc_atomic = os_atomic_start();
     {
	/*if(interval<65536)*/
	  OCR3A = interval;//this is the scale
	/* else
	  OCR3A =interval-65535;*/
     }
    os_atomic_end(_nesc_atomic); }    return 1;
}
//供底层调用获得定时器比较寄存器应当设置的值，下一个定时任务
uint16_t  Timer3_getnextInterval(void)
{
      return timer3queue_getnextinterval();
}