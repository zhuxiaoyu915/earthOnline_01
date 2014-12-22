
#include "top.h"

//--------------------------------------------------------

demo_beaconpacket * demopacket;
//--------------------------------------------------------


//---------------------------------------------------------------
inline uint8_t hardwareInit(void)

{
  OS_SET_PIN_DIRECTIONS();
  return SUCCESS;
}



uint8_t demo_ieee_address[8];
uint16_t shortaddress;
bool       DATA_READY;


//bool  IS_FIRST_TIME_SYN_REQ;
//---------------------------------------------------------
extern  MLME_SCAN_CONFIRM  scan_result_info;
extern  MAC_PIB  mac_pib_data;
extern  MAC_STATE_TYPE mac_current_state;
extern  bool IS_NETWORK_FORMED;
extern  RX_BUFFER  * head_of_rx_buffer;
extern  TX_BUFFER  * head_of_tx_buffer;
extern  Timer_Queue_pointer  head_of_timer_queue;
extern  INDIRECT_TX_PENDING_QUEUE * head_of_indirect_pending_buffer;
extern  LONG_ADDR  default_long_addr;
//extern  void read_data_task(void);
//extern  uint8_t  adc_init(void);
//extern  uint8_t sensor_init(void);
void memory_init(void)
{
     uint16_t  index;
     BYTE      *p;
	 p=NULL;
     for(index=0;index<4096;index++)
	 	*(p+index)=0x00;
}

void GET_ADC_value(void)
{
  //printf("\n--timer0 interrupt----\n");
    read_data_task();
    DATA_READY=TRUE; 	
    mlmeSyncRequest(ppib.phyCurrentChanel,TRUE);

}	


int   main(void)
{

  uint8_t return_state;
  uint8_t  index;
  Node_Capability  temp_node;
  ADDRESS  temp_addr;
  
  //memory_init();
  hardwareInit();
  Leds_greenOn();
  Leds_redOn();
  Leds_yellowOn();
  IoInit();
 
  OS_sched_init();

  phy_init();
  //--------------------------------
  //MAC_init();
  os_atomic_enable_interrupt();

  DATA_READY=FALSE;
  if(IS_RFD)
  	{
          kertimer_init();
          adc_init();
          sensor_init();
		  
  	}	  
 // kertimer_start(2, KERTIMER_REPEAT, 0x27f, &Leds_yellowToggle);
  //kertimer_start(1, KERTIMER_REPEAT, 0x07f, &Leds_greenToggle);
  //kertimer_start(0, KERTIMER_REPEAT, 0x04f, &Leds_redToggle);
  
 // backoff_timer_setOneShot(0x20,&Leds_greenOn);
 cc2420_enableAddrDecode();
 MAC_init();
 router_init();
 wdt_disable();
 printf("start timer is %08lx,head of systime queue is %04x\n",systime32(),head_of_systimer_queue);

 //mlmeResetRequest(TRUE);
 /*   
 //用于测试ED SCAN的代码
 return_state=PLME_SET_TRX_STATE_request(RX_ON);
  phy_channel_state=INACTIVE_PERIOD;
 if(return_state==PHY_SUCCESS)
 	{
 	       //scan_result_info.resultListSize=16;
 	     //printf("\nstart to do ED SCAN\n");  
              mlmeScanRequest(ED_SCAN, 0x0000ffff, 6);
		PLME_SET_TRX_STATE_request(TRX_OFF);	  
	       for(index=0;index<scan_result_info.resultListSize;index++)		  
            //printf("%8d\n",scan_result_info.pEnergyDetectList[index]);
 	}
  //*/
 //选中一批合适的信道并进行设置，然后进行active_passive_scan
//PLME_SET_request(phyCurrentChanel_id, 3);

//用于测试ACTIVE_PASSIVE_SCAN的代码
//----------------------------------
	//wdt_enable(WDTO_2S);
       
//----------------------------------
//mlmeScanRequest(ACTIVE_PASSIVE_SCAN, 0x00000008, 6);


  //while(mac_current_state!=MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED);
 /*
  //选中新的网络地址
  cc2420_bcn_accept();
  cc2420_setshortaddress((uint16_t)mac_pib_data.macShortAddress.Val);
  cc2420_setpanid((uint16_t)mac_pib_data.macPANId.Val);
  phy_channel_state=WAITING_FOR_BEACON;
  PLME_SET_request(phyCurrentChanel_id, 11);
  //-------------------------
  //uint16_t tempprint;
  //tempprint=cc2420_readreg(cc2420_FSCTRL);
  //printf("tempprintf is %04x",tempprint);
  //-------------------------
   return_state=PLME_SET_TRX_STATE_request(RX_ON);
   if(return_state!=PHY_SUCCESS)  
   	{
   	   error("\nopen the RF failure\n");
	   return;
   	}
*/
  //测试信标帧的发送
  //mlmeStartRequest(mac_pib_data.macPANId, ppib.phyCurrentChanel, mac_pib_data.macBeaconOrder, mac_pib_data.macSuperframeOrder, TRUE, mac_pib_data.macBattLifeExt, FALSE, FALSE, 0);
  
  //head_of_systimer_queue=systimer_start_task(head_of_systimer_queue,RECEIVE_BEACON, 1000) ;
  //head_of_systimer_queue=systimer_start_task(head_of_systimer_queue,CAP_OVER, 5000) ;




  //cc2420_setrxmode();
  
  //cc2420_setpanid(0x0001);
  //cc2420_setshortaddress(0x0001);
  cc2420_enablefifop();
  while(1) {
  	//wdt_reset();
  	//OS_run_task();
	//network layer management
	switch(mac_current_state)
		{
		    case MAC_STATE_UNSTARTED:
			    //printf("\n----START TO FORM NETWORK----\n");	
			      MAC_init();	  
			      mlmeResetRequest(TRUE);
			      IS_NETWORK_FORMED=FALSE; 	  
			      mlmeScanRequest(ED_SCAN, 0x00000008, 6);  
			      //for(index=0;index<scan_result_info.resultListSize;index++)		  
                           //  //printf("%8d\n",scan_result_info.pEnergyDetectList[index]);	  
			      break;
            case MAC_STATE_ENERGY_SCAN_FINISHED:
			     printf("\n---energy scan finished---\n");
			      mlmeScanRequest(ACTIVE_PASSIVE_SCAN, 0x00000008, 6);  	
			      break; 	  
		    case MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED:
				printf("\n---active scan finished----\n");
				if(!IS_COORDINATOR)
				mlmeSyncRequest(ppib.phyCurrentChanel,TRUE);
				//mac_current_state=MAC_STATE_SYNING;
				else
				{
				  cc2420_setshortaddress((uint16_t)mac_pib_data.macShortAddress.Val);
                              cc2420_setpanid((uint16_t)mac_pib_data.macPANId.Val);	  
				  mlmeStartRequest(mac_pib_data.macPANId,3, mac_pib_data.macBeaconOrder, mac_pib_data.macSuperframeOrder, TRUE, mac_pib_data.macBattLifeExt, FALSE, FALSE, 0);	
				}
			       break;	
            case MAC_STATE_SYNING_END:
			       if(!IS_NETWORK_FORMED)
			       {
			           //if(phy_channel_state==PHY_IDLE)	  	
			           printf("\nstart to do the associate\n");
				    //mac_current_state=MAC_STATE_IDLE;
				    temp_node.bytes=0x88;
				    temp_addr.Short=mac_pib_data.macCoordShortAddress;
				    mlmeAssociateRequest(ppib.phyCurrentChanel, 0x02, mac_pib_data.macPANId,temp_addr , temp_node, FALSE);
				    //IS_FIRST	
			       }
				    else
				   {
				       if(DATA_READY)
				    	{
					   //printf("\n---sensor data sent--\n");	
					   //---------------------------------------
					   //---------------------------------------
					   NODE_INFO dst;
					   NODE_INFO src;
					   uint8_t        msdulength;
					   BYTE  *       pMsdu;
					   BYTE           msduhandle;
					   BYTE           txOptions;
					   dst.addrMode=0x02;
					   dst.shortAddr=mac_pib_data.macCoordShortAddress;
					   dst.panID=mac_pib_data.macPANId;
					   src.addrMode=0x03;
					   src.longAddr=mac_pib_data.macExtAddress;
					   src.panID=mac_pib_data.macPANId;
					   msdulength=20;
					   pMsdu=(BYTE *)SENSOR_DATA_VALUE;
					   msduhandle=0;
					   txOptions=0x01;
					   mcpsDataRequest(dst, src, msdulength, pMsdu, msduhandle, txOptions);
					   //mac_current_state=MAC_STATE_START_TO_SLEEP;
				    	}	
				    }
				    break;
             case MAC_STATE_ASSOC_FINISHED:
				//网络已经建立完成,可以启动ADC传感器采样  
				if((!head_of_rx_buffer)&&(!head_of_tx_buffer)&&(!head_of_timer_queue))
				 //printf("\nnetwork successful formed\n");	
				IS_NETWORK_FORMED=TRUE;
				mac_current_state=MAC_STATE_IDLE;
				//break;
				//test orphan scan & unicast coordinator realignment frame
	                     //mlmeScanRequest(ORPHAN_SCAN, 0x00000008, 0);
	                     
	                     //test PANID conflict & broadcast coordinator realignment frame
	                     //panid_confilct_frame_send();

				//test Disassociation from the RFD
				//mlmeDisassociateRequest(default_long_addr, 0x02, FALSE);
				mac_current_state=MAC_STATE_START_TO_SLEEP;
				break;
             case  MAC_STATE_START_TO_SLEEP:
				//ADC 任务启动
				PLME_SET_TRX_STATE_request(FORCE_TRX_OFF);
				 DATA_READY=FALSE;
                            kertimer_start(0, KERTIMER_ONE_SHOT, 256, GET_ADC_value);
				//printf("\n----the current state is MAC_STATE_START_TO_SLEEP---\n");
				Leds_greenOff();
				Leds_yellowOff();
				Leds_redOff();
				cc2420_sendcmd(cc2420_SFLUSHRX);
				cc2420_sendcmd(cc2420_SFLUSHRX);
				head_of_systimer_queue=systimer_stop_task(head_of_systimer_queue, CAP_OVER);
				head_of_systimer_queue=systimer_stop_task(head_of_systimer_queue, RECEIVE_BEACON);
				while(head_of_systimer_queue!=NULL)
					{
					    head_of_systimer_queue=systimer_stop_task(head_of_systimer_queue, head_of_systimer_queue->timer_reason);
					  //printf("\n---error1--\n");
					}	
				while(head_of_timer_queue!=NULL)
					{
					    head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, head_of_timer_queue->framedsn, head_of_timer_queue->timer_reason);
					  //printf("\n---error2--\n");	
					}
				while(head_of_rx_buffer!=NULL)
					{
					    remove_from_rx_buffer(head_of_rx_buffer);
					  //printf("\n---error3--\n");		
					}
				while(head_of_tx_buffer!=NULL)
					{
					    remove_tx_queue(head_of_tx_buffer->FrameSeq);
				         //printf("\n---error4--\n");				
					}
				while(head_of_indirect_pending_buffer!=NULL)
					{
					    remove_pending_queue(head_of_indirect_pending_buffer);
					  //printf("\n---error5--\n");			
					}
				printf("\n----SYSTEM SLEEPING----\n");
				mac_current_state=MAC_STATE_INACTIVE;
				OS_sleep();
				break;
       	     default: break;
       	    	//开始作associate
       	    	
       	
		}
	       //printf("\nthe current state is %d---\n",mac_current_state);
	      // wdt_reset();
	OS_run_task();
		   
	//if(mac_current_state==MAC_STATE_INACTIVE)
	//	OS_sleep();
  	}
  
}
