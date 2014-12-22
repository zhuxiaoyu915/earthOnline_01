#include "systime.h"

extern  MAC_PIB  mac_pib_data;
extern MAC_STATE_TYPE mac_current_state;
extern bool         Is_received_beacon;
extern  uint8_t     loss_sync_number;
//extern  uint8_t  test_variable;
extern   bool  IS_NETWORK_FORMED;
union time_u
{
	struct
	{
		uint16_t low;
		uint16_t high;
	};
	uint32_t full;
};
uint16_t systime16L(void)
{
	return TCNT1;
}
uint16_t systime16H(void)
{
	return currentTime;
}
uint32_t systime32(void)
{
        union time_u time;
	
	{ os_atomic_t _nesc_atomic = os_atomic_start();
	{
		time.low = TCNT1;
		time.high = currentTime;

		
		if( bit_is_set(TIFR, TOV1) && ((int16_t)time.low) >= 0 )
			++time.high;
	}
	os_atomic_end(_nesc_atomic); }

	return time.full;
}
void systime_init(void)
{
	uint8_t timsk;
	head_of_systimer_queue=NULL;
	currentTime=0x0000;
	TCNT1=0x0000;
	TCCR1A = 0x00;
	TCCR1B = 0x00;
       
	{ os_atomic_t _nesc_atomic = os_atomic_start();
	{
		timsk = TIMSK;
	  	timsk |= (1<<TOIE1);//enable overflow interrupt
		timsk |=(1<<TICIE1);//enable input capture interrupt
		TIMSK = timsk;
		//---------------------------------------
		//  add some codes for timequeue inital
		//---------------------------------------
		
		       
	}
	 os_atomic_end(_nesc_atomic); }
	 //-------------------------------
	 // add some codes for timestamp initial
	 //-------------------------------
		
	TCCR1B =0xc0|0x05;//! start th timer with 1/1024 prescaler, 0.5 symbol on gainz,normal model,rising edge on sfd  will trigger the capture
}
void systime_stop(void)
{
	
	TCCR1B = 0x00;// stop the timer
}


//---------------------------------------------------------------------------
//      some timer1 drivers for timequeue added by zhouqiang in 2005.12.04
//---------------------------------------------------------------------------

void Timer1_OCR_intDisable(void)//disable the ocr1 interrupt
 {
      { os_atomic_t _nesc_atomic = os_atomic_start();
       	{
	 		TIMSK &= 0xef;
		}
      os_atomic_end(_nesc_atomic); }        	
 }

uint8_t  Timer1_setInterval(uint16_t interval)
{
   

     { os_atomic_t _nesc_atomic = os_atomic_start();
         {
                     TIFR|=1<<OCF1A;
		     	OCR1A = interval;//this is the scale
		     	//OCR1B = 0x0000;
		     	//OCR1C = 0x0000;
		     	//TCNT1 = 0x0000; //this is the counter
		     	//TCCR1A = 0x00;//Output: A: Disconnected, B: Disconnected, C: Disconnected,WGM[1..0]=00
		     	//scale |= 0x08;//
		     	//TCCR1B |= scale;//clk/(scale),WGM[3..2]=01/CTC
		     	//TCCR1C = 0x00;//
		     	TIMSK |= 0x10;//OCIE1A is enable 
     	
         }
         os_atomic_end(_nesc_atomic); }
		 //printf("*****the interval is %04x",interval);
         return SUCCESS;
}


void      Timer1_sethalfsymbol(uint16_t symbols)
{
      uint16_t halfsymbols;
      halfsymbols=symbols*2;
      Timer1_setInterval(symbols);
}

//供高层调用获得下一次定时的比较寄存器应当设置的值
uint32_t 	Timer1_acquireInterval(uint32_t interval)
{
       uint32_t temp;
	uint16_t  tcnt_1;
	//tcnt_1=TCNT1;
	//printf("\nthe current TCNT1 is %04x",tcnt_1);
	//printf("\nthe current interval is %04x",interval);
       //temp=(uint32_t)interval;
	temp=(interval<<1)+systime32();   
	//printf("\nthe current temp is %08lx",systime32());   
	// (temp>=65535) temp=temp-65535;
	return (temp);
}

uint8_t    Timer1_adjustInterval(uint16_t interval)
{
   { os_atomic_t _nesc_atomic = os_atomic_start();
     {
	//if(interval<65536)
	  	OCR1A = interval;//this is the scale
	//else
	//  	OCR1A =interval-65535;
     }
    os_atomic_end(_nesc_atomic); }    return 1;
}
//供底层调用获得定时器比较寄存器应当设置的值，下一个定时任务
uint16_t  Timer1_getnextInterval(void)
{
     //返回定时器下一个定时值
     //如果没有，则返回零
     systimer_queue_pointer  temp;
     temp=head_of_systimer_queue->next;
     if(temp!=NULL) 
     	{//printf("temp is %04x\n",temp->ticks);
	 return (temp->ticks);
     	}
     else
	 return 0;
}

//---------------------------------------------------------------------------
//   change timer1 to be systimer,time3 for other usage.  modified by zhouqiang in2005.12.04
//             add a timequeue here for tasks,such as receive or transmit a beacon 
//---------------------------------------------------------------------------
systimer_queue_pointer  systimer_start_task(systimer_queue_pointer queue_head,uint8_t  timer_reason, uint32_t  ticks) 
{
	 systimer_queue_pointer   temp;
	 systimer_queue_pointer   p_temp1,p_temp2;
	 uint32_t  compare_reg;
	 uint32_t  temp_time;
	    temp=(systimer_queue_pointer)malloc(sizeof(struct systimer_queue));
	    temp->next=NULL;
	    temp->timer_reason=timer_reason;
      	 if(queue_head==NULL)	   
	 	{
	 	     compare_reg=Timer1_acquireInterval(ticks);
		     //printf("\n******* the compare reg is %08lx",compare_reg);	 
		     if(compare_reg<65535)
			 	Timer1_setInterval((uint16_t)compare_reg);
		     else  
		     	{
		     	   temp_time=compare_reg&0x0000ffff;
			   //printf("\n******* the real compare reg is %04x",(uint16_t)(temp_time));	   
			   Timer1_setInterval((uint16_t)(temp_time));
		     	}
		     temp->ticks=(uint16_t)((compare_reg>65535)? ((uint16_t)(temp_time)):compare_reg);
                   temp->ticksLeft=compare_reg;
		     //temp->high_left=compare_reg&0xc0000000;
			 //printf("\n high left is %08lx",temp->high_left);
		     //temp->high_ticks=compare_reg&0xc0000000;
 	    	     queue_head=temp;	
		     //printf("\nhead ticks is %04x,ticksleft is %08lx,reason is %02x,head->reason is %02x\n",temp->ticks,temp->ticksLeft,timer_reason,temp->timer_reason);
		}
        else
		{
	              //插入到定时队列中去，按temp->ticks的顺序
                   compare_reg=Timer1_acquireInterval(ticks);
		     
		     //printf("\n----compare-reg is %08lx\n",compare_reg);
		     temp_time=compare_reg&0x0000ffff;
		     temp->ticks=(uint16_t)((compare_reg>65535)? ((uint16_t)(temp_time)):compare_reg);
                   temp->ticksLeft=compare_reg;
		     //temp->high_left=compare_reg&0xc0000000;
		     //temp->high_ticks=compare_reg&0xc0000000;
	            p_temp1=queue_head;
                   p_temp2=queue_head;
			/*while(((temp->high_left)>=(p_temp2->high_left))&&(p_temp2!=NULL))
				{
				    if((((temp->ticksLeft)&0x3fffffff)<=((p_temp2->ticksLeft)&0x3fffffff))&&((temp->high_left)==(p_temp2->high_left)))
				    	{
				    	    break;
				    	}
				   else*/
			while((p_temp2!=NULL)&&((temp->ticksLeft)>=(p_temp2->ticksLeft))&&(p_temp2!=NULL))	   
				      {
				          p_temp1=p_temp2;
				          p_temp2=p_temp1->next;	
				    	}		  
			  
				//}
			if(p_temp2!=queue_head)
				{
 				    Leds_redToggle();
 				    //printf("\nticks is %04x,ticksleft is %08lx,reason is %02x,temp->reason is %02x\n",temp->ticks,temp->ticksLeft,timer_reason,temp->timer_reason);
				    p_temp1->next=temp;
			           temp->next=p_temp2;
				}
			else
				//为头节点，需要调整物理定时器
				{
					
					 Timer1_adjustInterval(temp->ticks);
				    // printf("\nbecome head ticks is %04x,ticksleft is %08lx,reason is %02x,temp->reason is %02x\n",temp->ticks,temp->ticksLeft,timer_reason,temp->timer_reason);
				     temp->next=queue_head;
				     queue_head=temp;
				}
		}
	return queue_head;
}

systimer_queue_pointer systimer_stop_task(systimer_queue_pointer  queue_head,uint8_t timer_reason)
{
	//依据取消的framehandle,如果是队列头，则需要重新设置比较寄存器的值，否则只需把它从队列中摘掉即可。
       systimer_queue_pointer  p_temp1,p_temp2,p_temp3;
	p_temp1=queue_head;
	p_temp2=queue_head;
	p_temp3=queue_head;
	while((p_temp2->timer_reason!=timer_reason)&&(p_temp2!=NULL))
		 {
		    p_temp1=p_temp2;
		    p_temp2=p_temp2->next;	 
		 }
	if(p_temp2==NULL)  
		error("cannot find an element in the timer_queue");
	else
		{
		     p_temp3=p_temp2->next;
		     if(p_temp2==queue_head)
		     	{
	                   if(p_temp2->next==NULL)
	                   	{
	                   	 //printf("\n***timer 1_OCR  intrrput is diabled\n");
	                   	 Timer1_OCR_intDisable();
	                   	}
			     else
			     	{
			     	     //重新设置定时器
			            Timer1_adjustInterval(p_temp3->ticks);
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

extern MAC_STATE_TYPE  mac_current_state_saved;

void   systimer_task_fire(char timer_reason)
{
	switch(timer_reason)
		{
			case(BEACON_SEND):
				//call beacon_send
				Leds_yellowOn();
				Leds_greenOn();
				//Leds_redOn();
				phy_channel_state=TRANSMITTING_FRAME;
				
				//mlmeStartRequest(mac_pib_data.macPANId, ppib.phyCurrentChanel, mac_pib_data.macBeaconOrder, mac_pib_data.macSuperframeOrder, TRUE, mac_pib_data.macBattLifeExt, FALSE, FALSE, 0);
				beacon_frame_send(TRUE);
				printf("\n***system awake***\n");
				break;
			case(RECEIVE_BEACON):
				//wait for beacon receive
				//Leds_redOff();
				//printf("\nsystimer interrupt");
				if(!IS_COORDINATOR)
					{
					     Leds_yellowOn();
				            Leds_greenOn();
				            Leds_redOn();
					     //if(mac_current_state_saved==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE)		
					     //	 	mac_current_state=MAC_STATE_ASSOC_WAIT_FOR_RESPONSE;
					     //else	 
					     mac_current_state=MAC_STATE_IDLE;
					     phy_channel_state=WAITING_FOR_BEACON;
					     PLME_SET_TRX_STATE_request(RX_ON);
					     Is_received_beacon=FALSE;
					     //printf("\n***system waking***\n");	 
					}
				
				//Leds_greenToggle();
				
				//head_of_systimer_queue=systimer_start_task(head_of_systimer_queue,RECEIVE_BEACON, 500) ;
				//mlmeStartRequest(mac_pib_data.macPANId, ppib.phyCurrentChanel, mac_pib_data.macBeaconOrder, mac_pib_data.macSuperframeOrder, TRUE, mac_pib_data.macBattLifeExt, FALSE, FALSE, 0);
				
				break;
			case(CAP_OVER):
				//inactive the system
				phy_channel_state=INACTIVE_PERIOD;
				//对于采用间接通信的response帧可能要跨超帧操作
				mac_current_state_saved=mac_current_state;
				mac_current_state=MAC_STATE_INACTIVE;
				PLME_SET_TRX_STATE_request(FORCE_TRX_OFF);
				//Is_received_beacon=FALSE;
				Leds_greenOff();
				Leds_yellowOff();
				//Leds_redOff();
				if((Is_received_beacon==FALSE)&&(!IS_COORDINATOR))
					{
					   OS_post(synrequest_task);
					   //mac_current_state=MAC_STATE_SYNING;
					   //mac_current_state_saved=MAC_STATE_SYNING;
					}
				printf("\n***system sleeping***\n");
				break;
			case(SYN_WAITTING_FOR_BEACON):
				printf("\nloss syn\n");
                            if(!IS_NETWORK_FORMED)
					mac_current_state=MAC_STATE_UNSTARTED;
				else  			
				{
				       loss_sync_number++;
					if(loss_sync_number==10)
						{
						  //printf("\ncannot syn to the network\n");	
						    //向网络层报告同步失效信息	
						    //mlmeSyncLossIndication(MAC_ENUM lossReason);
				                  loss_sync_number=0;
						}
					else	
					mlmeSyncRequest(ppib.phyCurrentChanel,TRUE);   
				}
			        /*
				loss_sync_number++;
				if(loss_sync_number==10)
				   //--------------------------------------
				   //-------------加代码---------------
					{
				 //printf("\ncannot syn to the network\n");	
				   loss_sync_number=0;
					}
				else
					{
				   //OS_post(synrequest_task);
				   mac_current_state=MAC_STATE_ENERGY_SCAN_FINISHED;
				   //mlmeScanRequest(ACTIVE_PASSIVE_SCAN, 0x00000008, 6);
				   Is_received_beacon=FALSE;
				   //mac_current_state=MAC_STATE_SYNING;
					}*/
				break;
			case(CSMA_TIMING):
				//test_variable++;
				
				OS_post(CSMA_CA_DOING);
				break;
			default:
				//Leds_greenToggle();
				error("wrong systime reason!\n");
		}
	
}


//---------------------------------------------------------------------------
//   need some codes for  timestamp,added by zhouqiang in 2005.12.04
//---------------------------------------------------------------------------
uint16_t get_timestamp16H(void)
{
	return timestamp_H;
}
uint16_t get_timestamp16L(void)
{
	return ICR1;
}
uint32_t get_timestamp32(void)
{
	register union time_u time;
	{ os_atomic_t _nesc_atomic = os_atomic_start();
	{
		time.low =  ICR1;
		time.high = timestamp_H;
	}
	os_atomic_end(_nesc_atomic); }

	return time.full;
}

