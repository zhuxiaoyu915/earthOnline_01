#include "backoff_timer.h"

//----------------------------------------------------------------
//timer2 setup is added by zhouqiang in 2005.12.03
//----------------------------------------------------------------

//-------------------------------------------------------------------------------
inline   void timer2_intDisable(void)
{
  		  TIMSK &= ~(1 << 7);
}

uint8_t  timer2_setIntervalAndScale(uint8_t interval, uint8_t scale)
{

		  if (scale > 7) 
		  	{
		    		return FAIL;
		    	}
		  scale |= 0x8;
		  { os_atomic_t _nesc_atomic = os_atomic_start();
			    {
				   	      TIMSK &= ~(1 << 7);
				   	      TIMSK &= ~(1 << 6);
				   	      TCCR2 = scale;
				   	      TCNT2 = 0;
				   	      OCR2 = interval;
				   	      TIMSK |= 1 << 7;
			    }
		    os_atomic_end(_nesc_atomic); }
		    return SUCCESS;
}


//----------------------------------------------------------------
//backoff_timer setup is added by zhouqiang in 2005.12.03,timer2 is used
//----------------------------------------------------------------

uint8_t backoff_timer_stop(void)
{
	{ os_atomic_t _nesc_atomic = os_atomic_start();

	{
	      backoff_timer_Set = FALSE;
	      timer2_intDisable();
	}

	      os_atomic_end(_nesc_atomic); }
	      return SUCCESS;
}

void  backoff_timer_fire(void)

{
       
	void (*temp)();
	temp=backoff_call;
       temp();
      			 
  	             
}

inline  bool backoff_timer_isset(void)
{
	return backoff_timer_Set;
}


void  backoff_timer_setOneShot(uint8_t _jiffy,void (*fun)(void))
{
	{ os_atomic_t _nesc_atomic = os_atomic_start();
       {
      		backoff_call = fun;
      		backoff_timer_Set = TRUE;
        }

       os_atomic_end(_nesc_atomic); }
	       
      	timer2_setIntervalAndScale(_jiffy, 0x4);//presacle 256 //2 symbols
   			
  	return SUCCESS;
}




