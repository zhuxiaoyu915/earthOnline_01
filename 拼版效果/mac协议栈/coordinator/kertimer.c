#include "kertimer.h"

//----------------------------------------------------------------
//timer0 setup is added by zhouqiang in 2005.12.02
//----------------------------------------------------------------
/** 
   *  set Timer/Counter0 to be asynchronous 
   *  from the CPU clock with a second external 
   *  clock(32,768kHz)driving it
 **/
static void timer0_hardware_init(uint8_t interval, uint8_t scale){
	{ os_atomic_t  _nesc_atomic = os_atomic_start();
		{
			scale &= 0x7;
			scale |= 0x8;
			

			TIMSK &= ((unsigned char)~(1 << (TOIE0)));
			TIMSK &= ((unsigned char)~(1 << (OCIE0)));
			//!< Disable TC0 interrupt
		   	
			ASSR |= (1 << (AS0));         
			TCCR0 = scale;    
			TCNT0 = 0;
			OCR0 = interval;
			TIMSK |= (1 << (OCIE0));
		}
	os_atomic_end(_nesc_atomic); }
	
}   

static void timer0_setInterval(uint8_t val)
{
	uint8_t interval;
	if ((ASSR & (_BV(OCR0UB) | _BV(TCN0UB))) != 0) {
	  return;
	}
	if(val > 250) {interval = 250;}
	else if(val <= 2) {interval = 2;}
	else {interval = (uint8_t)val - 1;}
	TCNT0 = 0;
	OCR0 = interval;
	
}

inline uint8_t timer0_getInterval()   {return OCR0 + 1;}

inline uint8_t timer0_hardware_get_counter()
{
	return TCNT0;
}

//----------------------------------------------------------------
//kertimer setup is added by zhouqiang in 2005.12.02,timer0 is used
//----------------------------------------------------------------

void kertimer_init(void)
{
	 mState=0;
        setIntervalFlag = 0;
        queue_head = queue_tail = -1;
        queue_size = 0;
        mScale = 0x05;//prescale time0 with 128
        mInterval = maxTimerInterval;
	 timer0_hardware_init(mInterval,mScale);

}

uint8_t  kertimer_start (uint8_t  id,uint8_t type, uint32_t  interval,void (*fun)(void)) 
{
	 uint8_t diff;
        if (id >= KER_TIMERS) return FAIL;
        if (type>1) return FAIL;
        mTimerList[id].ticks = interval ;
        mTimerList[id].type = type;
	 mTimerList[id].tp=fun;
	
        { os_atomic_t _nesc_atomic = os_atomic_start();
	     		{
			            diff = timer0_hardware_get_counter();
			            interval += diff;
			            mTimerList[id].ticksLeft = interval;
			            mState|=(0x1L<<id);
			            if (interval < mInterval)
					{
			                mInterval=interval;
			                timer0_setInterval(mInterval);
			                setIntervalFlag = 0;
			                
			              }
			} 
	os_atomic_end(_nesc_atomic); }
       return SUCCESS;
}

void kertimer_stop(uint8_t id)
{
	if (id>=KER_TIMERS) return FAIL;
       if (mState&(0x1L<<id)) 
	   	{ // if the timer is running 
			    { os_atomic_t _nesc_atomic = os_atomic_start();
				{mState &= ~(0x1L<<id);
				if (!mState)
					{
			        		setIntervalFlag = 1;
			    		} 	
		              return SUCCESS;}
			       os_atomic_end(_nesc_atomic); }
        	}
       
	 return FAIL; //timer not running
}

static void adjustInterval(void) {
        uint8_t i, val = maxTimerInterval;
        if ( mState) 
			{
			            for (i=0;i<KER_TIMERS;i++)
					{
			                if ((mState&(0x1L<<i)) && (mTimerList[i].ticksLeft <val )) 
								{
			                   				 val = mTimerList[i].ticksLeft;
								}
			              }
						
				        { os_atomic_t _nesc_atomic = os_atomic_start();
						{
				                mInterval =  val;
				                timer0_setInterval(mInterval);
				                setIntervalFlag = 0;
				               }
					    os_atomic_end(_nesc_atomic);
					 }

		        } 
	else 
		        {
					{ os_atomic_t _nesc_atomic = os_atomic_start();
						{
				              mInterval=maxTimerInterval;
				              timer0_setInterval(mInterval);
				              setIntervalFlag = 0;
			             }
				      os_atomic_end(_nesc_atomic); }

		        }
			
		    }

static void   enqueue(uint8_t value) 
{
      if (queue_tail == KER_TIMERS - 1)
	  	queue_tail = -1;
      queue_tail++;
      queue_size++;
      queue[(uint8_t)queue_tail] = value;
}
static uint8_t dequeue() 
{
      if (queue_size == 0)
            return KER_TIMERS;
      if (queue_head == KER_TIMERS - 1)
            queue_head = -1;
      queue_head++;
      queue_size--;
      return queue[(uint8_t)queue_head];
 }
static void KERTimer_fired(void)
{
      void (*func)(void );
      uint8_t itimer = dequeue();
      //printf("\n------1----\n");
      if (itimer < KER_TIMERS)
	  	{ 
	  	       //printf("\n------2-----\n");
	  		func=mTimerList[itimer].tp;
                 	func();
      	       }
     
}

void KERTimer_Fire(void) 
{
        uint8_t i; 
	 setIntervalFlag = 1;
        if (mState) 
	{
            for (i=0;i<KER_TIMERS;i++) 
	     {
                if (mState&(0x1L<<i)) 
		  {
                    mTimerList[i].ticksLeft -= (mInterval+1) ; 
                    if (mTimerList[i].ticksLeft<=2) 
					{
                       			if (mTimerList[i].type==KERTIMER_REPEAT) 
									{
                            						mTimerList[i].ticksLeft += mTimerList[i].ticks;
                       						 } 
						else 
									{// one shot timer 
                           					 		mState &=~(0x1L<<i); 
                        						}
                       			 enqueue(i);
			  			 OS_post (&KERTimer_fired);
						 
                   			 }
                }
            }
        }
	adjustInterval();
}

