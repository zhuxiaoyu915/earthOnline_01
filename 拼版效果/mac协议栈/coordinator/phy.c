#include "phy.h"


void  phy_init(void)
{
     
     ppib.phyCurrentChanel=cc2420_DEF_CHANNEL;
     ppib.phyTransmitPower=0x1f;
     ppib.phyCCAMode=0x03;
     systime_init();
     cc2420_init();
     	 

     //--------------------------
     //other initial process
     //--------------------------
     
}


uint32_t PD_DATA_request(uint8_t psdulength,uint8_t * psdu)//added by zhouqiang in 2005.11.30//wait for test
{	

	//may be some bugs,because no radio state detect 
	cc2420_writetxfifo(psdulength,psdu);//因为不包括长度字节本身，需要在其前面先送进长度字节
	cc2420_sendcmd(cc2420_STXON);
	while(OS_READ_CC_SFD_PIN()==0);//first ,read the radio state from sfd
	while(OS_READ_CC_SFD_PIN()==1);
	send_timestamp=get_timestamp32();
	//printf("\nsend time is %08lx",send_timestamp);
	return send_timestamp;
		
	
}

uint8_t PLME_CCA_request(void)//added by zhouqiang in 2005.11.28//wait for test
{//

     uint16_t tempstate=0;
     tempstate=cc2420_readreg(cc2420_FSMSTATE);//first ,read the radio state from cc2420
     if (tempstate==1)//if rxoff 
	 	return TRX_OFF;
     else if ((tempstate==32)||((tempstate>=34)&&(tempstate<=39))||(tempstate==56))//if tx is going
     	 	return  TX_ON;
     	       
     else if (OS_READ_CC_CCA_PIN()==1)//when chanel is clear ,cca is 0
	       return  IDLE;
     else 
	 	return BUSY;
       
	 
};

ED_STATE PLME_ED_request(void)//added by zhouqiang in 2005.11.28//wait for test
{	
	ED_STATE temp;
	temp.energylevel=0;
	uint8_t tempstate;
	if ((tempstate=cc2420_readreg(cc2420_FSMSTATE))==1)//read the radio state,if trx_off
		      temp.state= TRX_OFF;
				
       else if ((tempstate==32)||((tempstate>=34)&&(tempstate<=39))||(tempstate==56))//if tx is going 
	             temp.state=TX_ON;
	else
		{    //after rx_on 8 sybmol,can read the rssi, have some bugs ,may read last value
		      temp.state= PHY_SUCCESS;
		      //----------------------------------
		      //can read the rssi state bit,then wait  here
		      //while((cc2420_sendcmd(cc2420_SNOP)&0x01)==0);//added by zhouqiang in 2005.12.1 
		      //----------------------------------
	             temp.energylevel=(int8_t) (cc2420_readreg(cc2420_RSSI)&0x00ff);
		};
	return temp;
}


uint8_t PLME_SET_TRX_STATE_request(uint8_t state)//added by zhouqiang in 2005.11.29//wait for test
{      
       uint16_t radiostate=0;
	radiostate=cc2420_readreg(cc2420_FSMSTATE); //first ,read the radio state
	switch (state)
		{case FORCE_TRX_OFF://强制关闭
			{
			cc2420_sendcmd(cc2420_SRFOFF);
			return PHY_SUCCESS;
			}
		 case  TRX_OFF://根据状态关闭，同时返回当时值
			{if (radiostate==1)//is idle
		             return  TRX_OFF;
	              else if (((radiostate>=34)&&(radiostate<=39))||((radiostate>=48)&&(radiostate<=54)))//is send a packet or send a ack
                           return  BUSY_TX;			
			else if (((radiostate>=3)&&(radiostate<=6))||(radiostate==16)||(radiostate==40))//is rx now 
			      return  BUSY_RX;
			else 
			      {cc2420_sendcmd(cc2420_SRFOFF);
			       return  PHY_SUCCESS;
				}
		 	}
		case  TX_ON://实际上cc2420芯片tx后会自动转成rx
			{
                      if  (((radiostate>=3)&&(radiostate<=6))||(radiostate==16)||(radiostate==40))//is rx_on
				return BUSY_RX;
			 else if (((radiostate>=34)&&(radiostate<=39))||((radiostate>=48)&&(radiostate<=54)))//last tx is going
			 	{
			 	 return TX_ON;
			 	}
			 else
			 	{
			 	cc2420_sendcmd(cc2420_STXON);	//tx
				return PHY_SUCCESS;
			 	}
			}
		case  RX_ON:
			{
			 if  (((radiostate>=34)&&(radiostate<=39))||((radiostate>=48)&&(radiostate<=54)))//tx is going
				return BUSY_TX;
			 else
			 	{
			 	cc2420_sendcmd(cc2420_SRXON);	
				return PHY_SUCCESS;
			 	}
			}
		default:
			//--------------------------
			//add error reports here
			//--------------------------
			return  PHY_UNSUPPORTED_ATTRIBUTE;//这是我自己加的，如果参数不对，返回该值
		}
			
}

GET_STATE PLME_GET_request(uint8_t pibattribute)//added by zhouqiang in 2005.11.29//wait for test
{
	GET_STATE temp;
	temp.state=PHY_SUCCESS;
	temp.pibattributevalue=0;
	temp.pibattribute=pibattribute;
	switch (pibattribute)  //may be some bugs.
		{
			case phyCurrentChanel_id:
					{
						temp.pibattributevalue=ppib.phyCurrentChanel;
						return temp;
						}
			case phyTransmitPower_id:
				       {
					   	temp.pibattributevalue=ppib.phyTransmitPower;
						return temp;
					}
			case phyCCAMode_id:
					{
						temp.pibattributevalue=ppib.phyCCAMode;
						return temp;
				       }
			default:
					{
						temp.state=PHY_UNSUPPORTED_ATTRIBUTE;
						return temp;
					}
		}
}

SET_STATE PLME_SET_request(uint8_t pibattribute,uint8_t pibattributevalue)//added by zhouqiang in 2005.11.28//wair fo test
{
	SET_STATE temp;
	temp.state=PHY_SUCCESS;
	temp.pibattribute=pibattribute;
	switch (pibattribute)//保证芯片中的实际情况与pib表中同步
		{
			case phyCurrentChanel_id:
					{
						ppib.phyCurrentChanel=pibattributevalue;
						cc2420_setchnl((int8_t)pibattributevalue);
						return temp;
						}
			case phyTransmitPower_id:
				       {
					   	ppib.phyTransmitPower=pibattributevalue;
						cc2420_currentparameters[CP_TXCTRL]=(cc2420_currentparameters[CP_TXCTRL] &0xfff0)\
							|(pibattributevalue<<cc2420_TXCTRL_PAPWR);
						cc2420_writereg(cc2420_TXCTRL,cc2420_currentparameters[CP_TXCTRL]);
						return temp;
					}
			case phyCCAMode_id:
					{
						ppib.phyCCAMode=pibattributevalue;
						cc2420_currentparameters[CP_MDMCTRL0]=(cc2420_currentparameters[CP_TXCTRL] &0xff3f) \
							|(pibattributevalue<<cc2420_MDMCTRL0_CCAMODE);
						cc2420_writereg(cc2420_MDMCTRL0,cc2420_currentparameters[CP_MDMCTRL0]);
						return temp;
				       }
			default:
					{
						temp.state=PHY_UNSUPPORTED_ATTRIBUTE;
						return temp;
					}
		}
			
		
}

//uint8_t   ack_dsn;
void phy_handlepacket(void)  //added bu zhouqiang in 2005.11.30 //wait for test
//attention:upper layer must free the rx_buf
{
       uint8_t packetlength=0,packettype=0,linkquality=0;
	rxbuf_flag=(cc2420_rxflag+cc2420_rxbufuse)&0x01;
	cc2420_rxbuf_now=cc2420_rxbuf[rxbuf_flag];
	packettype=cc2420_rxbuf_now[1]&0x03;
	packetlength=cc2420_rxbuf_now[0];
	linkquality=cc2420_rxbuf_now[packetlength-1];
	//printf("\nsucess received a  frame  phystate=%d\n",phy_channel_state);                                                            
	switch (phy_channel_state)//开始预滤包过程added by zhouqiang in 2005.11.30
		{
			
			case WAITING_FOR_BEACON://在waitbeacon期间收到其他包就扔掉
					{
						if (packettype!=0x00)
							{
							       //printf("\n--------wrong beacon frame\n");
						 		free(cc2420_rxbuf[rxbuf_flag]);
	                       		              cc2420_rxbufuse--;
							}
						else 
                                                 {
                                                        //printf("\nreceived a physical frame,the type is %02x",packettype);
						 		//free(cc2420_rxbuf[rxbuf_flag]);
								cc2420_rxbufuse--;
	                       		              //Leds_redToggle();
								PD_DATA_indication(packetlength, (cc2420_rxbuf_now), linkquality,receive_timestamp);
								phy_channel_state=PHY_IDLE;			  
							}
							
						break;
					}
			case WAITING_FOR_ACK://在waitack期间收到其他包就扔掉
					{
						if (packettype!=0x02)
							{
							     //printf("\nsucess received a non ack frame\n");
						 		free(cc2420_rxbuf[rxbuf_flag]);
	                       		              cc2420_rxbufuse--;
							}
						else
							{
                                                        //printf("\nsucess received a ACK frame\n");
						 		//free(cc2420_rxbuf[rxbuf_flag]);
	                       		              cc2420_rxbufuse--;
								head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, cc2420_rxbuf_now[3],WAIT_FOR_ACK);						
								PD_DATA_indication(packetlength, (cc2420_rxbuf_now), linkquality,receive_timestamp);
								phy_channel_state=PHY_IDLE;
							}				   
						     //PD_DATA_indication(packetlength, (cc2420_rxbuf_now+1), linkquality);
						break;
					}
				
		      	case PHY_IDLE:	
					{
						Leds_redToggle();
						cc2420_rxbufuse--;
						if(cc2420_rxbuf_now[1]&0x20)
						{
						   //if((cc2420_rxbuf_now[1]&0x03)&&())//是数据请求帧
						   //OS_post(ack_send_handle_pending);
						   //else
						   //ack_dsn=cc2420_rxbuf_now[3];

						   
						   (ack_send_handle_no_pending());
						}
						PD_DATA_indication(packetlength, (cc2420_rxbuf_now), linkquality,receive_timestamp);
						break;
					}	   	
			default:
					{
						//printf("\nwrong beacon frame\n"); 
						free(cc2420_rxbuf[rxbuf_flag]);
	                       		cc2420_rxbufuse--;
						break;
					}
			
									
		}
	//may have some bug----------------------------------------------------
	{ os_atomic_t _nesc_atomic = os_atomic_start();//added by zhouqiang in 2005.12.01 晚
		{
			if  ((cc2420_rxbufuse==0) && (rfpackernum!=0))//if rxfifo still have packets ,read rxfifo again
				{
		   			cc2420_rxbufuse++;
		   			OS_post(&cc2420_readrxfifo);
			       }
		}
	   os_atomic_end(_nesc_atomic); }
	

	//---------------------------------------------------------------------
	//-----------------------------------------------------
	//for (i=0;i<num;i++)
		//printf("%02x",cc2420_rxbuf_now[i]);
	
	//if ((cc2420_rxbuf_now[1]&0x02)!=0)//printf("\nis a ack frame!\n");
	
	//free(cc2420_rxbuf[flag]);
	//cc2420_rxbufuse--;
	//if  ((cc2420_rxbufuse==0) && (rfpackernum!=0))
	//	{
	//	   cc2420_rxbufuse++;
	//	   OS_post(&cc2420_readrxfifo);
	//	}
	//cc2420_rxbuf[flag]=NULL;
	//printf("释放rxbuf,now point is %04x\n",cc2420_rxbuf[flag]);
	//------------------------------------------------------
}
	
void ack_send_handle_no_pending(void)
{
       //wait_for_boundary();
       //printf("\nack frame send\n");
	uint32_t   phy_current_ticks;
	uint32_t  object_boundary_ticks;
	uint32_t   delta_time;
	phy_current_ticks=systime32();


	
	//uint8_t  buffer_ack[3];
	
       //buffer_ack[0]=0x02;
	//buffer_ack[1]=0x00;
	//buffer_ack[2]=ack_dsn;

	
	delta_time=((phy_current_ticks-MAC_OBJECT_CAP_START_TICKS)>>1);
	
	object_boundary_ticks=((((uint32_t)aUnitBackoffPeriod-(delta_time%aUnitBackoffPeriod))<<1)+phy_current_ticks);
	while(systime32()<(object_boundary_ticks));
       cc2420_sendcmd(cc2420_SACK);
       //PD_DATA_request(3, buffer_ack);
	while(OS_READ_CC_SFD_PIN()==0);//first ,read the radio state from sfd
	while(OS_READ_CC_SFD_PIN()==1);   
	send_timestamp=get_timestamp32();
	
	//printf("\nsendtimstamp is  %08lx\n",send_timestamp);
	//printf("\nphy_current_ticks is %08lx\n",phy_current_ticks);
	//printf("\nMAC cap start is %0l8x\n",MAC_OBJECT_CAP_START_TICKS);
	//printf("\ndelta_time is %0l8x\n",delta_time);
	//printf("\nMAC cap end is %0l8x\n",MAC_OBJECT_CAP_END_TICKS);
	//printf("\nobject boundary  is %08lx\n",object_boundary_ticks);
}
void ack_send_handle_pending(void)
{
        //wait_for_boundary();
        
        //cc2420_sendcmd(cc2420_SACKPEND);
}
