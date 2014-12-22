#include "mac.h"
#include "timer_queue.h"
#include "project.h"
#include "router.h"



//uint8_t  test_variable;
extern systimer_queue_pointer head_of_systimer_queue;


//һЩƵ��ʹ�õ�ȫ�ֱ���
MAC_PIB  mac_pib_data;
phy_pib  ppib;

uint8_t  scan_channel_index;
uint32_t  scan_channel;
uint16_t  scan_time_period;  

BYTE     mac_working_state;
MAC_FRAME_FORMAT   mac_tx_current_frame;
MAC_FRAME_FORMAT    mac_rx_current_frame;
BEACON_FRAME_FORMAT  mac_tx_beacon;

MAC_STATE_TYPE     mac_current_state;
MAC_STATE_TYPE  mac_current_state_saved;
bool               Is_mac_associated_successful;
//PHY_CHANEL_STATE  phy_channel_state;


//��ʱ�����У�ʵ����һ��ֻ��һ������Ԫ�أ�������ͬʱ�ж������
Timer_Queue_pointer  head_of_timer_queue;
Timer_Queue_pointer  current_timer_elem;	

//���Ͷ��У��������Ŵ����͵İ�
TX_BUFFER  *     head_of_tx_buffer;
TX_BUFFER  *     tail_of_tx_buffer;
parameter_tx_task  Parameter_list;

//���ն��У��������Ž��յİ�
RX_BUFFER  *     head_of_rx_buffer;
RX_BUFFER  *     tail_of_rx_buffer;

//pending���У��������ż��ͨ�ŵİ�
INDIRECT_TX_PENDING_QUEUE  * head_of_indirect_pending_buffer;
INDIRECT_TX_PENDING_QUEUE  * tail_of_indirect_pending_buffer;
uint8_t                                         current_pending_num;

bool TRACKBEACON;
bool IS_MAC_SYNING;

MLME_SCAN_CONFIRM  scan_result_info;
MCPS_DATA_INDICATION mcps_data_indication_info;
MLME_BEACON_NOTIFY_INDICATION  beacon_info;
MLME_ASSOCIATE_INDICATION   mlme_associate_indication_info;

SHORT_ADDR  default_short_addr;
SHORT_ADDR  no_short_addr;
LONG_ADDR    default_long_addr;
NODE_INFO      default_node;

//timing variable used to synchronize
uint32_t   MAC_SUPERFRAME_PERIOD;
uint32_t   last_beacon_tx_timestamp;
uint32_t   last_beacon_rx_timestamp;
uint32_t   MAC_CAP_PERIOD;
//Ԥ�ڵ�CAP����tick�ͳ�֡����tick
uint32_t   MAC_OBJECT_CAP_START_TICKS;
uint32_t   MAC_OBJECT_CAP_END_TICKS;
uint32_t   MAC_OBJECT_SUPERFRAME_TICKS;
uint32_t   MAC_CURRENT_TICKS;

bool         Is_received_beacon;
uint8_t     loss_sync_number;

bool         IS_NETWORK_FORMED;


//MAC��ʼ����������Ҫ�Ƕ�һЩ���ݽṹ�ĳ�ʼ����802.15.4��û�ж�����غ���
void MAC_init(void)
{
	//��ʱ�����еĳ�ʼ��������timerqueue_init����
	uint8_t  index;
	head_of_timer_queue=NULL;
	//TRACKBEACON��Ϊ�١�

	mac_current_state=MAC_STATE_UNSTARTED;
       phy_channel_state=PHY_IDLE;
	//���Ͷ���Ϊ��
	head_of_tx_buffer=NULL;
	tail_of_tx_buffer=NULL;
	Parameter_list.top=0;
	Parameter_list.tail=0;
	//���ն���Ϊ��
	head_of_rx_buffer=NULL;
	tail_of_rx_buffer=NULL;
       //pending����Ϊ��
	head_of_indirect_pending_buffer=NULL;
	tail_of_indirect_pending_buffer=NULL;
	current_pending_num=0;
       //mac_current_state=MAC_STATE_UNSTARTED;
	IS_MAC_SYNING=FALSE;
       default_short_addr.Val=0xffff;
	default_long_addr.longaddr=0;
	no_short_addr.Val=0xfffe;
	default_node.addrMode=0x00;
	head_of_systimer_queue=NULL;
       //---------------------------------------------------------------
       //------------���������㺯���õ��õ�ַ----------
       //-------------------------�����----------------------------
	mac_pib_data.macExtAddress.longaddr=0;
       scan_result_info.resultListSize=0;
       for(index=0;index<MAC_MAX_ED_MEASUREMENT;index++)
	   	scan_result_info.pEnergyDetectList[index]=0;

	loss_sync_number=0;
	//for(index=0;index<MAC_MAX_PAN_DESCRIPTORS;index++)
	//	scan_result_info.pPANDescriptorList
}

//**************************// 
MAC_ENUM mlmeResetRequest(bool setDefaultPIB)
{
   //����PLME_SET_TRX_STATE.requestԭ�����Ƶ�շ���ΪTRX_OFF
   
   //uint8_t state;
   uint8_t return_state;
   
   //Ȼ����resetMAC�������PIB�����нṹ���ԱԪ��Ϊ��ʼֵ
   mac_pib_data.macAckWaitDuration=54;
   //mac_pib_data.macAckWaitDuration=4;
   if(IS_COORDINATOR||IS_FFD)
       mac_pib_data.macAssociationPermit=TRUE;
   else
       mac_pib_data.macAssociationPermit=FALSE;
   //================================
   uint8_t buffer_addr[8];
   LONG_ADDR long_ieee_addr;
   long_ieee_addr.longaddr=MY_IEEE_ADDR;
   //buffer_addr=(uint8_t )
   for(uint8_t i=0;i<8;i++)
   	{
   	buffer_addr[i]=long_ieee_addr.v[i];
	//printf("\n--ieee addr[%d] is %02x--\n",i,buffer_addr[i]);
   	}
   cc2420_set_ieee(buffer_addr);
   IS_NETWORK_FORMED=FALSE;
   mac_pib_data.macAutoRequest=FALSE;
   mac_pib_data.macBattLifeExt=FALSE;
   mac_pib_data.macBattLifeExtPeriods=6;
   mac_pib_data.pMacBeaconPayload=NULL;
   mac_pib_data.macBeaconPayloadLength=0;
   mac_pib_data.macBeaconOrder=6;
   mac_pib_data.macBeaconTxTime=0x00000000;
   //mac_pib_data.macBSN=0x00;  random value between 0x00-0xff;
   mac_pib_data.macCoordExtendedAddress=default_long_addr;
   mac_pib_data.macCoordShortAddress=default_short_addr;
   //mac_pib_data.macDSN=0x00;  random value between 0x00-0xff;
   mac_pib_data.macGTSPermit=FALSE;
   mac_pib_data.macMaxCsmaBackoffs=2;
   mac_pib_data.macMinBE=3;
   mac_pib_data.macPANId=default_short_addr;
   mac_pib_data.macPromiscuousMode=FALSE;
   //===============�޸�
   mac_pib_data.macRxOnWhenIdle=TRUE;
   mac_pib_data.macShortAddress=default_short_addr;
   mac_pib_data.macSuperframeOrder=5;
   mac_pib_data.macTransactionPersistenceTime=0x0005;
   cc2420_get_ieee(mac_pib_data.macExtAddress.v);
   //for(uint8_t i=0;i<8;i++)
   //printf("\nmacExtaddr is %02x\n",mac_pib_data.macExtAddress.v[i]);
   if(IS_COORDINATOR)
   	{
   	     mac_pib_data.macCoordExtendedAddress=mac_pib_data.macExtAddress;
	     mac_pib_data.macCoordShortAddress.Val=COOR_SHORT_ADDRESS;
	     mac_pib_data.macShortAddress.Val=COOR_SHORT_ADDRESS;
	     mac_pib_data.macPANId.Val=COOR_PAN_ID;
   	}
   else 
   	{
   	     mac_pib_data.macPANId=default_short_addr;
            mac_pib_data.macShortAddress=default_short_addr;
   	}
   //mac_pib_data.
   return_state=PLME_SET_TRX_STATE_request(TRX_OFF);
   if(return_state==SUCCESS)
   	return  SUCCESS;
   else return DISABLE_TRX_FAILURE;
   //���յ�PLME_SET_TRX_STATE.confirm���ҽ��Ϊ�ɹ��󣬽�����MLME_RESET.confirmԭ�ﲢ��Ϊ�ɹ�״̬
   //������ΪDISABLE_TRX_FAILURE״̬��ʵ����confirmԭ����ͨ������ֵ���صġ�
   
} 

//**************************//
MAC_ENUM mlmeSetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue)
{
	 //���ݺ������ݹ����Ĳ����޸�PIB�ṹ�еĳ�Ա����������Ӧ�ĳɹ�ʧ�ܽ��(��confirmԭ��)
}
 
//**************************//
//�б�Ҫ����mlmescanconfirm����,scan request 
//״̬��scan request���أ������Ľ����ȫ�ֽṹMLME_SCAN_CONFIRM����; 
MAC_ENUM mlmeScanRequest(BYTE scanType, DWORD scanChannels, UINT8 scanDuration)
{

	uint16_t  scantime_on_channel;
	uint32_t  object_time;
	uint8_t    index;
	int8_t    temp_max_energy;
	ED_STATE  temp_return_ED;
	SET_STATE  temp_return_SET;
	scantime_on_channel=((((uint16_t)aBaseSuperframeDuration)<<scanDuration)+((uint16_t)aBaseSuperframeDuration));
	//printf("=====scan time is %08x",scantime_on_channel);
	scan_result_info.resultListSize=0;
	switch(scanType)
		{
		    case ED_SCAN:
			   //printf("\n---START TO DO THE ENERGY SCAN----\n");	
	                   mac_current_state=MAC_STATE_ENERGY_SCAN;
			      phy_channel_state=INACTIVE_PERIOD;
	                   for(index=0;index<16;index++)
	                   {
	                       if(((1<<index)&(scanChannels))!=0)
	                       {
	                           object_time=systime32()+(((uint32_t)scantime_on_channel)<<1);
				    //printf("the object time is %08lx",object_time);			   
				      //if(object_time>=0x3fffffff)
				      //	  	object_time=object_time-0x3fffffff;
				      temp_max_energy=-128;
				      temp_return_SET=PLME_SET_request(phyCurrentChanel_id, index);	
				      PLME_SET_TRX_STATE_request(RX_ON);  
				      if(temp_return_SET.state==PHY_SUCCESS)
				      	{
			                  while(systime32()!=object_time)
	                   	           {    
	                   	               temp_return_ED=PLME_ED_request();
						 if((temp_return_ED.energylevel>=temp_max_energy)&&(temp_return_ED.state=PHY_SUCCESS))
						 	temp_max_energy=temp_return_ED.energylevel;
	                   	           }
					    scan_result_info.pEnergyDetectList[scan_result_info.resultListSize++]=temp_max_energy;
				      	}					  
	                       }
				  if(scan_result_info.resultListSize>=MAC_MAX_ED_MEASUREMENT)
				  	break;
	                   }
			     mac_current_state=MAC_STATE_ENERGY_SCAN_FINISHED;
			     phy_channel_state=INACTIVE_PERIOD;
			     PLME_SET_TRX_STATE_request(TRX_OFF);
	                   break;         
	           case ACTIVE_PASSIVE_SCAN:
                           scan_channel_index=0;
                           scan_channel=scanChannels;
                           scan_time_period=scantime_on_channel;   
	                    cc2420_bcn_accept();
	                    cc2420_setshortaddress(default_short_addr.Val);
                           cc2420_setpanid(default_short_addr.Val);
	                    mac_current_state=MAC_STATE_ACTIVE_PASSIVE_SCAN;
                          
			    while((scanChannels&((uint32_t)1<<scan_channel_index))==0)
			    	{
					scan_channel_index++;
					if(scan_channel_index==16)
						return;
			    	}	
			    	
			    PLME_SET_request(phyCurrentChanel_id, scan_channel_index++); 
			    head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, 0, WAIT_SCAN, scan_time_period);	
			    PLME_SET_TRX_STATE_request(RX_ON);			  
			     phy_channel_state=WAITING_FOR_BEACON;		
			   //printf("\nset the %d channel & waitting for beacon\n",scan_channel_index-1);	 
	                   break;
	          case ORPHAN_SCAN:
	//��Ϊorphan SCAN��
	//-----------------------------------***���Կ��ǰ��������������һ������(������),��ʱ�жϷ������Ҳ���Ե��øú���
	//������Ϊ      void orphan_ scan_done(void)
	//���ȶ��ڵ�һ���ŵ�������PLME_SET_requestԭ�����������ŵ���Ȼ�����CSMA��ʼ����orphan notification֡(���и���������ü�pg130��Ҳ����Ҫ�ȴ�ack)
	//Ȼ��������ʱ�����з��񣬶�ʱԭ��ΪWAIT_FOR_RESPONSE�����ж�ʱ����ΪaResponseWaitTime��symbol��ͬʱ����ƵΪ����״̬���ȴ�coordinator realignment֡
	//�Ƴ���ԭ��������ŵ���scan�����ɶ�ʱ�����жϷ���������
	//NOTE1:���и�scan��confirmԭ�������mac���֡���ս��̷������жϷ�����򷢳���
	//NOTE2��orphan scan��ͬ��ǰ������scan����ֻҪ�յ�һ��realgnment����֡������ֹͣscan��
                           mac_current_state=MAC_STATE_ORPHAN_SCAN;
	                    //=====================================================
	                    //=========��������㺯�����������ŵ�============
	                   orphan_notification_frame_send();
			     
			     break;
	     }
}
void mlmeScanConfirm(MLME_SCAN_CONFIRM  scan_result_info)
{
       //����scan_result_info���޸Ķ�Ӧ��mac���PHY���pib��Ϣ
       //�����и����ӵ�ѡ���㷨������򵥴����һ��
     //printf("\n-------it is in scan confirm---------\n");
	if((scan_result_info.status==SUCCESS)&&(scan_result_info.scanType==ORPHAN_SCAN))
	{
	   //printf("\n----SUCCESSFULL ORPHAN SCAN----\n");
	     mac_current_state=MAC_STATE_IDLE;
	     return;	 
	}
       if((scan_result_info.status==SUCCESS)&&(scan_result_info.scanType==ACTIVE_PASSIVE_SCAN))
       {//printf("\n-------it is in scan confirm---------\n");
           mac_current_state=MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED;
           //printf("\n-----the macbeacon is %02x\n",scan_result_info.pPANDescriptorList[0].superframeSpec.SF.bytes.LSB);
	    mac_pib_data.macBeaconOrder=((scan_result_info.pPANDescriptorList[0].superframeSpec.SF.bytes.LSB)&0x0f);
	    mac_pib_data.macSuperframeOrder=((scan_result_info.pPANDescriptorList[0].superframeSpec.SF.bytes.LSB)>>4);
	    mac_pib_data.macPANId.Val=scan_result_info.pPANDescriptorList[0].coordinator.panID.Val; 		
	    if(scan_result_info.pPANDescriptorList[0].coordinator.addrMode==0x02)	
	    	{
	           mac_pib_data.macCoordShortAddress.Val=scan_result_info.pPANDescriptorList[0].coordinator.shortAddr.Val;
	    	}
	    else if(scan_result_info.pPANDescriptorList[0].coordinator.addrMode==0x03)	
	    	{
	    	    mac_pib_data.macCoordExtendedAddress.longaddr=scan_result_info.pPANDescriptorList[0].coordinator.longAddr.longaddr;	
	    	}
	    ppib.phyCurrentChanel=scan_result_info.pPANDescriptorList[0].logicalChannel;
	    //PLME_SET_request(phyCurrentChanel_id, ppib.phyCurrentChanel);
	    //OS_post(synrequest_task);
       }
 	else
 	{
 	   if(!IS_COORDINATOR)
 	   	{
 	   mac_current_state=MAC_STATE_UNSTARTED;
	 //printf("\n--PASSIVE SCAN FAILURE-----\n");
 	   	}
	   else
	   	{
	   mac_current_state=MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED;
	 //printf("\n---doesn't exist any 802.15.4 network ----\n");
	 //printf("---I will be the coordinator----");
	   	}
	   //OS_post(scan_task);	
 	} 
       
}

void scan_task(void)
{
    mlmeScanRequest(ACTIVE_PASSIVE_SCAN, 0x00000008, 6);
}


//**************************//
//��ʼ��֯һ���µ�����,���ͳ�֡(������FFD�豸�ſ��Ե��ø�ԭ�ﺯ��)
//���������Э���ߣ������Ըú�����starttime����
//NOTE:�ڵ��øú���ǰ������Э������˵��Ӧ���Ѿ����ú�����������Ĳ���
//���ڷ�Э���ߵ�FFD(�൱�ڶ��������еĴؽڵ�)��˵��Ӧ���Ѿ�ͬ�����丸�ڵ�(�����������Ľڵ�)���ű�֡��Ȼ��starttime������ڸ��ڵ���ű�֡���ԡ�
//���ڷ�Э���ߵ�FFD�����ű�֡ʱ��ʵ����starttimeҲ����Ϊ���ű�֡���ص�txoffset���ֵ
//PIB�����и���ԱmacBeaconTxtime��¼����һ�η����ű�֡�ķ�����Ŀ�Ƕ��١�
void mlmeStartRequest(SHORT_ADDR panId, uint8_t  logicalChannel, uint8_t beaconOrder, uint8_t superframeOrder, bool panCoordinator, bool batteryLifeExtension, bool coordRealignment, bool securityEnable , WORD StartTime)
{

   SET_STATE  temp_return_SET;
   uint8_t return_state;
   mac_tx_beacon.beacon_payload_length=0;
   mac_tx_beacon.longpendingadd_length=0;
   mac_tx_beacon.shortpendingadd_length=0;
   //printf("\n*******the beacon order is %02x\n",beaconOrder);
   //printf("\n*******the super order is %02x\n",superframeOrder);
   mac_tx_beacon.superframe_field.SF.bytes.LSB=((superframeOrder<<4)|beaconOrder);
   //�������ǲ�����GTS����,������������finalCAPSlotΪ15.
   if(panCoordinator)
   mac_tx_beacon.superframe_field.SF.bytes.MSB=(0x80)|(0x40)|(0x0f);
   else
   mac_tx_beacon.superframe_field.SF.bytes.MSB=(0x80)|(0x0f);
   //printf("\n*******the superframe_field is %02x\n",mac_tx_beacon.superframe_field.SF.bytes.LSB);
   //printf("\n*******the superframe_field is %02x\n",mac_tx_beacon.superframe_field.SF.bytes.MSB);
   mac_pib_data.macPANId=panId;
   mac_pib_data.macBeaconOrder=beaconOrder;
   mac_pib_data.macSuperframeOrder=superframeOrder;
   ppib.phyCurrentChanel=logicalChannel;
   //printf("\n====Base is %d   \n====macBeacon is %d   \n===superframeorder is %d",aBaseSuperframeDuration,mac_pib_data.macBeaconOrder,mac_pib_data.macSuperframeOrder);
   MAC_SUPERFRAME_PERIOD=(uint32_t)(((uint32_t)aBaseSuperframeDuration)<<(mac_pib_data.macBeaconOrder));
   MAC_CAP_PERIOD=(uint32_t)(((uint32_t)aBaseSuperframeDuration)<<(mac_pib_data.macSuperframeOrder));
   //printf("\n ---------cap   time is %08lx",MAC_CAP_PERIOD); 	
   //printf("\n ---------super time is %08lx",MAC_SUPERFRAME_PERIOD);	 
   temp_return_SET=PLME_SET_request(phyCurrentChanel_id, logicalChannel);
   /*return_state=PLME_SET_TRX_STATE_request(TX_ON);
   if(return_state!=PHY_SUCCESS)  
   	{
   	   error("\nopen the RF failure\n");
	 //printf("%d",return_state);
	   return;
   	}*/
   if(!coordRealignment)
   	beacon_frame_send(TRUE);
   else
   	coordinator_realignment_frame_send(FALSE, default_node, default_short_addr);
}

void mlmeStartConfirm(void)
{
	//֪ͨ�߲㷢����ϡ�
	//����ͨ��post�߲���������ɸ߲�ĺ�������
	//�պ������߲�ʵ��
}

//**************************//
void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, SHORT_ADDR coordPANId, ADDRESS pCoordAddress, Node_Capability capabilityInformation, bool securityEnable)
{
	//����ϵͳ��MAC״̬����ΪMAC_STATE_ASSOC_STARTED
	//���÷���������associate request֡��POST()
	NODE_INFO dst;
	SET_STATE  temp_return_SET;
	dst.addrMode=coordAddrMode;
	dst.panID=coordPANId;
	if(coordAddrMode==0x03)
		dst.longAddr=pCoordAddress.Extended;
	else if(coordAddrMode==0x02)
		dst.shortAddr=pCoordAddress.Short;
	//mac_current_state=MAC_STATE_ASSOC_REQ_SENT;
	if(!IS_COORDINATOR)
	{  
          temp_return_SET=PLME_SET_request(phyCurrentChanel_id, logicalChannel);
	   //if(phy_channel_state==PHY_IDLE)
	   	//{
	   	    mac_current_state=MAC_STATE_ASSOC_REQ_SENT;
	   	    associate_request_frame_send(coordPANId, dst, capabilityInformation);
	   	//}	  
	   /*else
	   	{
	   	    mac_current_state=
	   	}*/
	} 
	//else if(IS_FFD)............
}
void mlmeAssociateConfirm(SHORT_ADDR AssocShortAddress, BYTE status)
{
	//printf("\n------success associate------ %d\n",status);
	//printf("\nthe allocated short address is %04x\n",AssocShortAddress.Val);
	if(status==NO_ACK)
		printf("\n----the association request frame is no acked-----\n");
	//�������ΪNO_DATA
	if(status==NO_DATA)
		printf("\n---didn't received the association response frame---\n");
	if(status==SUCCESS)
	{
	mac_current_state=MAC_STATE_ASSOC_FINISHED;
	//mac_current_state=MAC_STATE_IDLE;
	//mac_current_state_saved=MAC_STATE_IDLE;
	phy_channel_state=PHY_IDLE;
	PLME_SET_TRX_STATE_request(RX_ON);
	mac_pib_data.macShortAddress=AssocShortAddress;
	cc2420_setshortaddress((uint16_t)mac_pib_data.macShortAddress.Val);
       cc2420_setpanid((uint16_t)mac_pib_data.macPANId.Val);		
	}
	else 
	{
	   //��Ҫ��������������
	   mac_current_state=MAC_STATE_SYNING_END;
	}
}

//**************************//
void mlmeAssociateIndication(MLME_ASSOCIATE_INDICATION   mlme_associate_indication_info)
{
	//Э�������յ�associate request֡��֪ͨ�߲�
	//����post�߲�����
	BYTE status;
	SHORT_ADDR  shortaddr_alloc;
	//��Ҫ�����ǵ���mlmeassociateresponseԭ��
	//------------------------------------------------------
	//---------------�����------------------------------
	//--------������Ƿ��������-------------------
	//--------�޸�status,����shortaddr--------------------
     //printf("\n------success received an association request frame--------\n");
	//for(uint8_t i=0;i<8;i++)
	//	printf("\nthe addr is v[%d]=%d\n",i,mlme_associate_indication_info.Deviceaddr.v[i]);
	//shortaddr_alloc.Val=RFD_SHORT_ADDRESS;
	shortaddr_alloc=short_address_allocate_req(mlme_associate_indication_info.Deviceaddr);
	printf("\n---allocated short addr is %04x---\n",shortaddr_alloc.Val);
	status=0x00;
	mlmeAssociateResponse(mlme_associate_indication_info.Deviceaddr, shortaddr_alloc,status, FALSE);
}
//**************************//
void mlmeAssociateResponse(LONG_ADDR deviceAddress, SHORT_ADDR assocShortAddress, BYTE status, bool securityEnable)
{
	//Э���ߵĸ߲����յ�associate indicationԭ�����ӦMAC���ԭ��
	//MAC����÷���������associate response֡
	FRAME_CONTROL framectl_field;
	NODE_INFO dst; 
	NODE_INFO src; 
	//���治����.
	dst.addrMode=0x03;
	src.addrMode=0x03;
	dst.panID=mac_pib_data.macPANId;
	src.panID=mac_pib_data.macPANId;
       src.longAddr=mac_pib_data.macExtAddress;	
	dst.longAddr=deviceAddress;
	framectl_field.v[0]=0x23;
	framectl_field.v[1]=0xcc;
	
	//
	if((IS_FFD!=0x00)&&(mac_current_state==MAC_STATE_IDLE)&&(current_pending_num<MAX_PENDING_LIST_SIZE))
		{
		    add_pending_queue(0, framectl_field, dst,src,0, NULL, status, TRUE, deviceAddress,assocShortAddress);
		}
	else if(current_pending_num>=MAX_PENDING_LIST_SIZE)	
		{
		   //printf("\npending full\n");
		     //���ϲ㱨��
		     //mlme_comm_status()
		}
	//--------------------------------------------------
       //In order to test the disassociation frame from the coordinator
       //LONG_ADDR  RFD_LONG_ADDR;

	//test the coordinator want the RFD leave the network
       //mlmeDisassociateRequest(deviceAddress,0x01, FALSE);

      //---------------------------------------------------
	
		
}


//**************************//
void mlmeDisassociateRequest(LONG_ADDR pDeviceAddress, BYTE disassociateReason, bool securityEnable)
{
	//Э���ߺ�һ���豸���ɷ��͸�����
	//MAC����÷���������disassociation notification֡
	//�����Э���߽����ü�ӷ���,����pending���У�������Ҫ����ϵͳMAC���״̬����
	FRAME_CONTROL framectl_field;
	NODE_INFO dst;
	NODE_INFO src;
	//����Ϊ�ظ��Թ���������ɾ��
	dst.addrMode=0x03;
	src.addrMode=0x03;
	dst.panID=mac_pib_data.macPANId;
	src.panID=mac_pib_data.macPANId;
	src.longAddr=mac_pib_data.macExtAddress;
	
	//
	framectl_field.v[0]=0x23;
	if(!IS_COORDINATOR)
	framectl_field.v[1]=0xc8;
	else	
	framectl_field.v[1]=0xcc;
       if((mac_current_state)==MAC_STATE_IDLE)
       {
          if((IS_FFD!=0x00)&&(current_pending_num<=MAX_PENDING_LIST_SIZE))
          	{
          	    dst.longAddr=pDeviceAddress;
		    add_pending_queue(0, framectl_field, dst, src, 0, NULL,disassociateReason,FALSE,pDeviceAddress,default_short_addr);
		
          	}
          else
          	{
          	    dst.longAddr=mac_pib_data.macCoordExtendedAddress;
		    disassociate_notification_frame_send(disassociateReason, mac_pib_data.macCoordExtendedAddress);
          	}	
       }
}

void mlmeDisassociateIndication(LONG_ADDR deviceAddress, BYTE disassociateReason, bool securityUse, BYTE aclEntry)
{
	//Э���߻��ն��豸���յ��Է���������disassociation notification֡��
	//֪ͨ�߲㡣post�߲�����
	//�պ������߲�ʵ��
	//------------------------------------------------------
	//---------------�����------------------------------
	//--------�����ɾ����Ӧ��associate��¼--------
	//-------------------------------------------------------
       if(!IS_COORDINATOR)
       {
	 //printf("\n--the RFD received the disassociation indiation----\n");
	   mac_current_state=MAC_STATE_IDLE;
       }  
	else
	printf("\n----the coordinator received the disassociation indication----\n");	
	
}
void mlmeDisassociateConfirm(MAC_ENUM status)
{
	//��MAC�㷢�����(���۳ɹ���ʧ��)�������ø�ԭ��֪ͨ�߲�
	//�ָ�ϵͳ��MAC���״̬����
  //����postһ���߲�����
  //�պ������߲�ʵ��
     if(status==SUCCESS)
     {
         if(!IS_COORDINATOR)
             printf("\n---success disassociated from the network-----\n");
         else
             printf("\n---success disassociated the object RFD from the network------\n");	 	
     }		 
}

void mlmeBeaconNotifyIndication(MLME_BEACON_NOTIFY_INDICATION  beacon_info)
{
       bool Is_pending;
	uint8_t  i;
	Is_pending=FALSE;
	//printf("\nmac_current state is %d--\n",mac_current_state);
	if(mac_current_state==MAC_STATE_ACTIVE_PASSIVE_SCAN)
      	{
	   scan_result_info.scanType=ACTIVE_PASSIVE_SCAN;
	   scan_result_info.status=SUCCESS;
	   //scan_result_info.resultListSize++;
	   if(scan_result_info.resultListSize>MAC_MAX_PAN_DESCRIPTORS)
	   	{
	   	   head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, 0,WAIT_SCAN);
		   mlmeScanConfirm(scan_result_info);
		   mac_current_state=MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED;
		   PLME_SET_TRX_STATE_request(TRX_OFF);
		   phy_channel_state=INACTIVE_PERIOD;
               //printf("\n$$$$$$$$$$sccessed finish passive scan$$$$$$$$$$$\n");
		   
	   	}
	   else
	   	{
	   	   scan_result_info.pPANDescriptorList[scan_result_info.resultListSize]=beacon_info.panDescriptor;
		   scan_result_info.resultListSize++;  
	   	}
      	}
	/*else if()
	{
	     
	}*/
	else  
	{     
	       for(i=0;i<beacon_info.shortpendingadd_length;i++)
	       	{
	       	      if(beacon_info.shortaddrpending_list[i].Val==mac_pib_data.macShortAddress.Val)
				  	Is_pending=TRUE;
	       	}
		for(i=0;i<beacon_info.longpendingadd_length;i++)
			{
			      if(beacon_info.longaddrpending_list[i].longaddr==mac_pib_data.macExtAddress.longaddr)
				  	Is_pending=TRUE;
			}
	       /*if((Is_pending)&&(mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE))
		{
		     //�е�С���⣬���Э���߷�������֡��ô��?  
		     //head_of_timer_queue=timer3quere_cancel(head_of_timer_queue,0, WAIT_FOR_RESPONSE);
		}*/
		if(Is_pending)
			//����data request֡
			{
			      //printf("\nbe told there is association response frame in the coordinator\n");
			      data_request_frame_send();	   
			}
			//data_request_frame_send();
                /*#ifdef  I_AM_RFD 			                                                                                                                                                                                                                                                     
                else                                                                                                                                                                                                                                                                                         
                {                                                                                                                                                                                                                                                                                    
                		    //test  beacon  received                                                                                                                                                                                                                                                         
                		  //printf("\nsuccess received a beacon frame\n");                                                                                                                                                                                                                                   
                		  //printf("\n the logical channel is %d",beacon_info.panDescriptor.logicalChannel);                                                                                                                                                                                                 
                		  //printf("\n the coord panid is %08x",beacon_info.panDescriptor.coordinator.panID.Val);	                                                                                                                                                                                     
                	         //printf("\n the coord address  is %08x",beacon_info.panDescriptor.coordinator.shortAddr.Val);		                                                                                                                                                                             
                }                                                                                                                                                                                                                                                                                    
                #endif   */                                                                                                                                                                                                                                                                                                       
	}       
}               
//*************************//
void mlmeOrphanIndication(LONG_ADDR orphanAddress, bool securityUse, BYTE aclEntry)
{               
	//Э���߻��ͷ�ڵ����յ�һ���豸���͹�����orphan nitification֡ʱ��֪ͨ�߲��ԭ��
	//��ɸ߲����񣬸�������Ҫ�ǵ���orphan responseԭ�
	SHORT_ADDR shortaddress;
	//���Ҷ�ӦorphanAddress��shortaddress.
	//-------------------------------------------
	//-------------------�����--------------
	//--------------�����Ĵ���----------
	//shortaddress.Val=0xfc8b;
       //shortaddress.Val=RFD_SHORT_ADDRESS;
	printf("\nsuccess received a orphan notification frame\n");
	shortaddress=short_address_allocate_req(orphanAddress);
	mlmeOrphanResponse(orphanAddress, shortaddress, TRUE, FALSE);
}
//*************************//
void mlmeOrphanResponse(LONG_ADDR orphanAddress, SHORT_ADDR shortAddress, bool associatedMember, bool securityEnable)
{
	//Э���߻��ͷ�ڵ�߲����յ�orphan indication����Ϊ��MAC���һ����Ӧ
	//MAC����÷���������coordinator realignment֡
	NODE_INFO  dst;
	dst.addrMode=0x03;
	dst.longAddr.longaddr=orphanAddress.longaddr;
	//dst
       if((IS_FFD)&&(associatedMember)&&(mac_current_state==MAC_STATE_IDLE))
	   	coordinator_realignment_frame_send(1, dst,shortAddress);
	   	
	   	
}


void mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS *pSrcAddr, BYTE dstAddrMode, ADDRESS *pDstAddr, BYTE status)
{
	//֪ͨ�߲���ʾresponse֡�Ĵ������
	//�պ���
}


void mlmeRxEnableRequest(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration)
{
	//�պ���
}
void mlmeRxEnableConfirm(MAC_ENUM status)
{
	//�պ���
}

void  synrequest_task(void)
{
    mlmeSyncRequest(ppib.phyCurrentChanel,TRUE);
}

//*************************//
//�߲���ϵͳ��MAC��������״̬(�����Ѿ�����)�·���synͬ������
void mlmeSyncRequest(UINT8 logicalChannel, bool trackBeacon)
{

        mac_current_state=MAC_STATE_SYNING;
	 PLME_SET_TRX_STATE_request(RX_ON);	
	 PLME_SET_request(phyCurrentChanel_id, logicalChannel);
	  if(!IS_NETWORK_FORMED)
	  {
	     cc2420_setshortaddress((uint16_t)default_short_addr.Val);
	     cc2420_setpanid((uint16_t)mac_pib_data.macPANId.Val);
	  }	 
	 //printf("\n-----the channelID is %d\n",ppib.phyCurrentChanel);
	 phy_channel_state=WAITING_FOR_BEACON;
	 IS_MAC_SYNING=TRUE;
	 //MAC_SUPERFRAME_PERIOD=aBaseSuperframeDuration*(2<<mac_pib_data.macBeaconOrder);
	 //printf("\n&&&&&&&the macbeaconorder is %04x\n",mac_pib_data.macBeaconOrder);
	 MAC_SUPERFRAME_PERIOD=(uint32_t)(((uint32_t)aBaseSuperframeDuration)<<(mac_pib_data.macBeaconOrder));
	 //printf("\n&&&&&&&the MAC_SUPERFRAME PERIOD is %08lx\n",MAC_SUPERFRAME_PERIOD);
        MAC_CAP_PERIOD=(uint32_t)(((uint32_t)aBaseSuperframeDuration)<<(mac_pib_data.macSuperframeOrder));
	 //head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, 0, WAIT_FOR_BEACON, timinglength);
	 head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, SYN_WAITTING_FOR_BEACON, MAC_SUPERFRAME_PERIOD);
	 Is_received_beacon=FALSE;
	 //if()
	 
}
void mlmesyn_finished(void)
{
     uint32_t  deltatime;
     uint32_t truetime; 
     uint32_t  margintime;
     margintime=0;	 
     //if((mac_current_state==MAC_STATE_SYNING_END)||(mac_current_state==MAC_STATE_IDLE))
     {
          truetime=systime32();
          deltatime=(truetime>=last_beacon_rx_timestamp) ? truetime-last_beacon_rx_timestamp:(0x3fffffff-last_beacon_rx_timestamp+truetime);
          head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, CAP_OVER, (MAC_CAP_PERIOD-(deltatime>>1)-margintime));
          head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, RECEIVE_BEACON, (MAC_SUPERFRAME_PERIOD-(deltatime>>1)-margintime));
          //printf("\n ---------beacon rx time is %08lx\n",last_beacon_rx_timestamp);
	   //printf("\n----------true time is  %08lx\n",truetime);	  
          //printf("\n ---------cap   time is %08lx\n",MAC_CAP_PERIOD); 	
          //printf("\n ---------super time is %08lx\n",MAC_SUPERFRAME_PERIOD);
          //Leds_yellowToggle();
	   	  
          MAC_OBJECT_CAP_START_TICKS=last_beacon_rx_timestamp;
          MAC_OBJECT_CAP_END_TICKS=truetime+((MAC_CAP_PERIOD<<1)-deltatime-margintime);
          MAC_OBJECT_SUPERFRAME_TICKS=truetime+((MAC_SUPERFRAME_PERIOD<<1)-deltatime-margintime);
	   //mac_current_state=MAC_STATE_IDLE;
     }
}

void mlmeSyncLossIndication(MAC_ENUM lossReason)
{
	//����ԭ����MAC�㷢�͸�ԭ��֪ͨ�߲㡣
	//��ͨ��postһ���߲����񡣸߲�Զ�ʧͬ������Ҫ������������
	//�պ������߲�ʵ��
	NODE_INFO dst;
	//printf("\n--- a PAN ID conlict ----\n");
	if(IS_COORDINATOR)
	{
	//������Ҫ��������һ��mac���������
	coordinator_realignment_frame_send(0, dst, default_short_addr);
	mac_current_state=MAC_STATE_IDLE;
	}
	else
	{
	   if(lossReason==BEACON_LOSS)
	   	//����������
	   	mac_current_state=MAC_STATE_UNSTARTED;
	      //����
	      //mac_current_state=MAC_STATE_ENERGY_SCAN_FINISHED;
	}
}

//*************************//
void mcpsDataRequest(NODE_INFO dst, NODE_INFO src, UINT8 msduLength, BYTE *pMsdu, BYTE msduHandle, BYTE txOptions)
{
	//MAC�����յ�ԭ��󣬸���txOption�Ĳ���
	//��Ϊֱ��ͨ�ţ�����÷��������͸�����֡
	//��Ϊ���ͨ��,������ŵ�pending������,�����ء�
	FRAME_CONTROL  framectl_field;
	if((txOptions&0x04)==0)
		//ֱ��ͨ��
		{
		data_frame_send(pMsdu, msduLength,dst,src,msduHandle);
		mac_current_state=MAC_STATE_IDLE;
		}
	else if((dst.addrMode!=0x00)&&(dst.addrMode!=0x01))
	       //���ͨ��
		{
		   if((txOptions&0x01)==0)
		       framectl_field.v[0]=0x01;
		   else
		   	framectl_field.v[0]=0x21;
		   framectl_field.v[1]=0xcc&(dst.addrMode<<2)&(src.addrMode<<6);	
		   if(current_pending_num<=MAX_PENDING_LIST_SIZE)
	            {
	                add_pending_queue(msduHandle, framectl_field, dst,src, msduLength, pMsdu,0x00,FALSE,default_long_addr,default_short_addr);
                       //���뵽�ű�֡��pending����
                       /*if(dst.addrMode==0x02)
			     mac_tx_beacon.shortaddrpending_list[mac_tx_beacon.shortpendingadd_length++]=dst.shortAddr;
			  else 
			     mac_tx_beacon.longaddrpending_list[mac_tx_beacon.longpendingadd_length++]=dst.longAddr;
			     */
		     }
                 else
			   mcpsDataConfirm(TRANSACTION_OVERFLOW, msduHandle);
		   }
	else   mcpsDataConfirm(INVALID_PARAMETER, msduHandle);
}

void mcpsDataConfirm(MAC_ENUM status, BYTE msduHandle)
{
	//��ΪMAC���mcpsdatarequest��һ����Ӧ��֪ͨ�߲�
	//�պ������߲�ʵ��
	printf("\n----sensor data is sent successfull-----\n");
	mac_current_state=MAC_STATE_START_TO_SLEEP;
	
}
void mcpsDataIndication(MCPS_DATA_INDICATION mcps_data_indication_info)
{
	//MAC���յ�һ������֡��֪ͨ�߲㣬��postһ���߲�����
	//�պ������߲�ʵ��
	printf("\n-----success received the sensor data-------\n");
	if(IS_COORDINATOR)
	sensor_data_tx_to_pc(mcps_data_indication_info);
}

uint8_t  PD_DATA_indication(uint8_t psdulength,uint8_t * psdu,uint8_t ppdulinkquality,uint32_t  timestamp)
{
    head_of_rx_buffer=add_to_rx_buffer(psdulength, psdu, ppdulinkquality,timestamp);
	//printf("\nfinished add the beacon frame to the buffer\n");
   //printf("\npsdu->payload is %d\n",psdu[0]);	
    //printf("\nthe head->payload is %d",*(head_of_rx_buffer->payload));  	
    OS_post(mac_get_received_frame);
    //for(uint8_t i=0;i<psdulength;i++)
    //		printf("\npsdu-> [%d]--is--%02x\n",i,psdu[i]);
}

//���������Ǵ������ȡһ��֡�����н���
void  mac_get_received_frame(void)
{
      	  RX_BUFFER * current;
	  uint8_t      index;
	  uint8_t     i;
	  index=0;
	  current=head_of_rx_buffer;
	  //printf("\nthe head->payload is %d",*(head_of_rx_buffer->payload));       
         if(tail_of_rx_buffer==head_of_rx_buffer)
		 	tail_of_rx_buffer=head_of_rx_buffer->next;
  	  head_of_rx_buffer=head_of_rx_buffer->next;	 
         //��ʼ�������յ���֡��������д��mac_rx_current_frame��
         //Ȼ�����mac_frame_rcvd���д���
         mac_rx_current_frame.frameLength=(current->framelength)-2;
         mac_rx_current_frame.lqi=current->lqi;
         mac_rx_current_frame.timestamp=current->timestamp;
         //index++;
         //printf("\nthe processing received frame's current->timestamp is %08lx",current->timestamp);
         //for(index=0;index<mac_rx_current_frame.frameLength;index++)
         //    //printf("\nthe current->payload is %02x",*(current->payload+index));                                                                                                                                                                                                                                                                                                                                                          
	 //if(current->framelength==current->payload[0])                    
	  	//printf("\nreceived a mac bracon frame correctly");      
	  printf("\nthe received frame length  is %d\n",mac_rx_current_frame.frameLength);	
	  index=1;	
	  mac_rx_current_frame.frame_control_field.v[0]=current->payload[index++];
	  mac_rx_current_frame.frame_control_field.v[1]=current->payload[index++];
	  //printf("\nthe frame_ctl is v[0]=%02x\n",mac_rx_current_frame.frame_control_field.v[0]);
	  //printf("\nthe frame_ctl is v[1]=%02x\n",mac_rx_current_frame.frame_control_field.v[1]);
	  mac_rx_current_frame.macDSN=current->payload[index++];
         mac_rx_current_frame.logicalchannel_field=ppib.phyCurrentChanel;
	  switch(mac_rx_current_frame.frame_control_field.v[1]&0x0c)
	  	{
   	          case  MAC_DST_LONG_ADDR:
			mac_rx_current_frame.dst.addrMode=0x03;
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	mac_rx_current_frame.dst.panID.v[0]=current->payload[index++];
			mac_rx_current_frame.dst.panID.v[1]=current->payload[index++];
			//��˸��ֽڷų�Ŀ�ĵ�ַ		 	
		 	mac_rx_current_frame.dst.longAddr.v[0]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[1]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[2]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[3]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[4]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[5]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[6]=current->payload[index++];
			mac_rx_current_frame.dst.longAddr.v[7]=current->payload[index++];
		 	break;
	         case  MAC_DST_SHORT_ADDR:
			mac_rx_current_frame.dst.addrMode=0x02;
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	mac_rx_current_frame.dst.panID.v[0]=current->payload[index++];
			mac_rx_current_frame.dst.panID.v[1]=current->payload[index++];
			//�������ֽڷŶ�Ŀ�ĵ�ַ
		 	mac_rx_current_frame.dst.shortAddr.v[0]=current->payload[index++];
			mac_rx_current_frame.dst.shortAddr.v[1]=current->payload[index++];
		 	break;
	        case  MAC_DST_NO_ADDR:
                     mac_rx_current_frame.dst.addrMode=MAC_DST_NO_ADDR;		
 		 	break;
	        case  MAC_DST_ADDR_RESERVED:
		 	error("\nreserved dst address mode is used!\n");
		 	break;			
	  	}
	  switch(mac_rx_current_frame.frame_control_field.v[1]&0xc0)
	  	{
   	          case  MAC_SRC_LONG_ADDR:
			mac_rx_current_frame.src.addrMode=0x03;
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	mac_rx_current_frame.src.panID.v[0]=current->payload[index++];
			mac_rx_current_frame.src.panID.v[1]=current->payload[index++];
			//��˸��ֽڷų�Ŀ�ĵ�ַ		 	
		 	mac_rx_current_frame.src.longAddr.v[0]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[1]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[2]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[3]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[4]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[5]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[6]=current->payload[index++];
			mac_rx_current_frame.src.longAddr.v[7]=current->payload[index++];
		 	break;
	         case  MAC_SRC_SHORT_ADDR:
			mac_rx_current_frame.src.addrMode=0x02;
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	mac_rx_current_frame.src.panID.v[0]=current->payload[index++];
			mac_rx_current_frame.src.panID.v[1]=current->payload[index++];
			//�������ֽڷŶ�Ŀ�ĵ�ַ
		 	mac_rx_current_frame.src.shortAddr.v[0]=current->payload[index++];
			mac_rx_current_frame.src.shortAddr.v[1]=current->payload[index++];
		 	break;
	        case  MAC_SRC_NO_ADDR:
			mac_rx_current_frame.src.addrMode=0x00;
		 	break;
	        case  MAC_SRC_ADDR_RESERVED:
		 	error("\nreserved dst address mode is used!\n");
		 	break;			
	  	}	 
	  //MHR�Ѿ��������
	  mac_rx_current_frame.type=mac_rx_current_frame.frame_control_field.v[0]&0x07;
	  switch(mac_rx_current_frame.type)
	  	{
	  	   case MAC_FRAME_BEACON:
		   	  mac_rx_current_frame.superframe_field.SF.bytes.LSB=current->payload[index++];
			  mac_rx_current_frame.superframe_field.SF.bytes.MSB=current->payload[index++];
			  //GTS��Ϊ0x00
			  mac_rx_current_frame.gts_char_field.bytes=current->payload[index++];
			  //pending address field
			  mac_rx_current_frame.pendaddress_field.bytes=current->payload[index++];
			  mac_rx_current_frame.shortpendingadd_length=(mac_rx_current_frame.pendaddress_field.bytes&0x07);
			  mac_rx_current_frame.longpendingadd_length=(((uint8_t)((mac_rx_current_frame.pendaddress_field.bytes)&0x70))>>4);
			  //printf("\nreceived long  pending address bytes is%d\n",mac_rx_current_frame.longpendingadd_length);
			  for(i=0;i<mac_rx_current_frame.shortpendingadd_length;i++)
			  	{
		                    mac_rx_current_frame.shortaddrpending_list[i].v[0]=current->payload[index++];
				      mac_rx_current_frame.shortaddrpending_list[i].v[1]=current->payload[index++];
			  	}
			  for(i=0;i<mac_rx_current_frame.longpendingadd_length;i++)
			  	{
			  	     mac_rx_current_frame.longaddrpending_list[i].v[0]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[1]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[2]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[3]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[4]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[5]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[6]=current->payload[index++];
				     mac_rx_current_frame.longaddrpending_list[i].v[7]=current->payload[index++];		 
			  	}
			  mac_rx_current_frame.beacon_payload_length=current->payload[index++];
			  for(i=0;i<mac_rx_current_frame.beacon_payload_length;i++)
			   	     mac_rx_current_frame.beacon_payload[i]=current->payload[index++];		 
			  break;
		   case MAC_FRAME_DATA:
	                mac_rx_current_frame.nwk_data_length=current->payload[index++];
			  for(i=0;i<mac_rx_current_frame.nwk_data_length;i++)
			            mac_rx_current_frame.nwk_data_payload[i]=current->payload[index++];
		         break;
		   case MAC_FRAME_ACK:
		   	  //no payload
		   	  
		         break;
		   case MAC_FRAME_CMD:
		   	  mac_rx_current_frame.cmd=current->payload[index++];
			  switch(mac_rx_current_frame.cmd)
			  	{
			  	    case  MAC_CMD_ASSOCIATE_REQ:
					   //printf("\nindex is %d\n",index);	
					   mac_rx_current_frame.node_capinfo_field.bytes=current->payload[index++];
					   break;
				    case  MAC_CMD_ASSOCIATE_RPLY:
					   mac_rx_current_frame.shortaddralloc.v[0]=current->payload[index++];
					   mac_rx_current_frame.shortaddralloc.v[1]=current->payload[index++];
					   mac_rx_current_frame.association_status=current->payload[index++];
					   //printf("\n----the short alloc addr is %04x---\n",mac_rx_current_frame.shortaddralloc.Val);
					   //printf("\nthe associate status is %02x\n",mac_rx_current_frame.association_status);
					   break;
				    case  MAC_CMD_BEACON_REQ:
					   break;
				    case  MAC_CMD_COORD_REALIGNMENT:
					   mac_rx_current_frame.PAN_ID_field.v[0]=current->payload[index++];
					   mac_rx_current_frame.PAN_ID_field.v[1]=current->payload[index++];
					   mac_rx_current_frame.coordshortaddr_field.v[0]=current->payload[index++];
					   mac_rx_current_frame.coordshortaddr_field.v[1]=current->payload[index++];
					   mac_rx_current_frame.logicalchannel_field=current->payload[index++];
					   mac_rx_current_frame.shortaddrallocted.v[0]=current->payload[index++];
					   mac_rx_current_frame.shortaddrallocted.v[1]=current->payload[index++];
					   break;
				    case  MAC_CMD_DATA_REQ:
					   break;
				    case  MAC_CMD_DISASSOCIATE_NOTICE:
					   mac_rx_current_frame.disassociation_reason=current->payload[index++];
					   break;
				    case  MAC_CMD_GTS_REQ:
					   break;
				    case  MAC_CMD_ORPHAN_NOTICE:
					   break;	
				    case  MAC_CMD_PAN_ID_CONFLICT:
					   break;	
			  	}
		         break;	
	  	}
        if((index-1)!=mac_rx_current_frame.frameLength)
   	  {
   	    //printf("\nthe received frame length  is %d\n",mac_rx_current_frame.frameLength);
	    //printf("\nthe index is %d\n",index-1);	  
	      error("\n!!!received frame length error\n");	  
	      return;
         }
	  remove_from_rx_buffer(current);
	  //��ʼ����֡����
	  mac_frame_rcvd();
}

//MAC�����յ�һ��֡����δ����֡����
//����mac_rx_current_frame�еĽṹ��Ա���д���
void mac_frame_rcvd(void)
{
       //ack֡�ķ����Ѿ�����Ҫ����ɺ��ٵ��øú���
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	TX_BUFFER  * p_frame;
	uint8_t  i;
       switch(mac_rx_current_frame.type)
       	{
       	     case MAC_FRAME_ACK:
	                 //printf("\ncurrent ack frame dsn is %d\n",mac_rx_current_frame.macDSN);
	                 //head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, mac_rx_current_frame.macDSN,WAIT_FOR_ACK);
			   p_frame=get_frame_pointer(mac_rx_current_frame.macDSN);
			   if(p_frame==NULL) 
			   	break;
	                 switch(p_frame->cmd_type)
	                	{
	                	       case  MAC_CMD_ASSOCIATE_REQ:
	                               printf("\nsuccess receive a association req ack frame\n");
					   //printf("---receive ack time is  %08lx---",mac_rx_current_frame.timestamp); 				 
                                        if(mac_current_state==MAC_STATE_ASSOC_REQ_SENT)
                                        {
		                              mac_current_state=MAC_STATE_ASSOC_WAIT_FOR_RESPONSE;
		                              //head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, 0, WAIT_FOR_RESPONSE, aResponseWaitTime);
						  //printf("\ntimer3 is start\n");			  
                                        }				  
					     //�Ƿ���Ҫ��������Ƶ�ŵ�Ϊ����״̬??
					     break;
                                   case MAC_CMD_DATA_REQ:
	                                 printf("\nsuccess received the data request ack\n");
	                                 head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, 0, WAIT_FOR_DATA, aMaxFrameResponseTime);
				            phy_channel_state=PHY_IDLE;
					     mac_current_state=MAC_STATE_ASSOC_WAIT_FOR_RESPONSE;
					     PLME_SET_TRX_STATE_request(RX_ON);
	                                 break;
	      //-----//������������ֻ���ж�ϵͳMAC��״̬��Ϊ�����״̬���ɡ�     
	                           case  0x00://��������֡
	      //���Ϊdata frame��ack֡,���������������
	           //���ϲ㷢��mcps-data.comfirmԭ���ʾ�ɹ���������֡  
	                                 mcpsDataConfirm(SUCCESS, p_frame->FrameHandle);
	                                 break; 
	                           case  MAC_CMD_PAN_ID_CONFLICT:
	      //���ΪPANID confilict notification��ack֡�����������������
	           //ȷ��Э�����Ѿ��յ���֡����ȷ����.
	                                 //���¿���  
	                                 break;
	                           case  MAC_CMD_DISASSOCIATE_NOTICE:
	      //���Ϊdisassociation notification��ack֡�����������������
	           //���ϲ㷢��mlme-disassociation.confirmԭ��,����״̬�ɹ�
	                                 mlmeDisassociateConfirm(SUCCESS);
	                                 break;
	                           case MAC_CMD_ASSOCIATE_RPLY:
	      //���Ϊassociate response��ack֡�����������������
	           //���ϲ㷢��mlme-comm-statusԭ���ʾ��Ӧ֡���ͳɹ�
	                                 //mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS * pSrcAddr, BYTE dstAddrMode, ADDRESS * pDstAddr, BYTE status)
	                                 //
	                                 break;
	                           case MAC_CMD_COORD_REALIGNMENT:
	      //���Ϊcoordinator realignment��ack֡(ע��:������͸�֡ʱ���ǹ㲥���ſ���Ҫ��ack)�����������������ֱ�ӷ���
	           //NOTE:��֡�϶���Э�����յ�orphan����󷢳��ġ�(mlme-start.request������realignment֡�ǹ㲥֡)
	           //���ϲ㷢��mlme-comm-statusԭ���ʾ��֡���ͳɹ�(pg87)
	                                 //mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS * pSrcAddr, BYTE dstAddrMode, ADDRESS * pDstAddr, BYTE status)
	                                 break; 
	      //�������������ϣ��ͷŷ���֡ռ�õĴ洢�ռ�
	                	}
	                     remove_tx_queue(mac_rx_current_frame.macDSN);
	                     break;
                       case MAC_FRAME_BEACON:
	                      if(IS_FFD!=0x00)
				      mlmeSyncLossIndication(PAN_ID_CONFLICT);
			        else 
			        	{   
			                   last_beacon_rx_timestamp=mac_rx_current_frame.timestamp;
					     if(IS_MAC_SYNING)
					     	{
						 	//head_of_timer_queue=timer3quere_cancel(head_of_timer_queue,0, WAIT_FOR_BEACON);
						 	head_of_systimer_queue=systimer_stop_task(head_of_systimer_queue, SYN_WAITTING_FOR_BEACON);
                                                 //ͬ�����
                                                 mac_current_state=MAC_STATE_SYNING_END;
							IS_MAC_SYNING=FALSE;
							last_beacon_rx_timestamp=mac_rx_current_frame.timestamp;
							mlmesyn_finished();
							phy_channel_state=PHY_IDLE;
							PLME_SET_TRX_STATE_request(RX_ON);
							Is_received_beacon=TRUE;
							loss_sync_number=0;
							printf("\n------synchronizing success------ \n");
							
							//mlme
			        	       }
			        	     else if((mac_rx_current_frame.beacon_payload_length!=0)||(!mac_pib_data.macAutoRequest)||(mac_current_state==MAC_STATE_ACTIVE_PASSIVE_SCAN))
			        	     	{
			        	     	    mac_pib_data.macCoordShortAddress=mac_rx_current_frame.src.shortAddr;
			        	     	    beacon_info.bsn=mac_rx_current_frame.macDSN;
						    beacon_info.sduLength=mac_rx_current_frame.beacon_payload_length;
						    beacon_info.pendAddrSpec=mac_rx_current_frame.pendaddress_field.bytes;
						    beacon_info.shortpendingadd_length=mac_rx_current_frame.shortpendingadd_length;
						    beacon_info.longpendingadd_length=mac_rx_current_frame.longpendingadd_length;
						    for(i=0;i<beacon_info.shortpendingadd_length;i++)
								beacon_info.shortaddrpending_list[i]=mac_rx_current_frame.shortaddrpending_list[i];
						    for(i=0;i<beacon_info.longpendingadd_length;i++)
								beacon_info.longaddrpending_list[i]=mac_rx_current_frame.longaddrpending_list[i];
						    for(i=0;i<beacon_info.sduLength;i++)
								beacon_info.pSdu[i]=mac_rx_current_frame.beacon_payload[i];
						    beacon_info.panDescriptor.coordinator=mac_rx_current_frame.src;
						    beacon_info.panDescriptor.timeStamp=mac_rx_current_frame.timestamp;
						    beacon_info.panDescriptor.linkQuality=mac_rx_current_frame.lqi;
						    beacon_info.panDescriptor.superframeSpec=mac_rx_current_frame.superframe_field;
					           //printf("\n-----the superframe field is %04x\n",mac_rx_current_frame.superframe_field.SF.Val);
						    beacon_info.panDescriptor.logicalChannel=mac_rx_current_frame.logicalchannel_field;
						    //�����򱾰汾���ṩ֧�֡�	
						    //Leds_greenToggle();                                 						    
						    //printf("\nfinished function mac_frame_rcvd()\n");
						    //printf("\nbeacon frame received\n");
						    if((mac_current_state_saved==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE)||(mac_current_state_saved==MAC_STATE_ORPHAN_SCAN))
						       {mac_current_state=mac_current_state_saved;}
						    mlmeBeaconNotifyIndication(beacon_info); 
						   if(mac_current_state!=MAC_STATE_ACTIVE_PASSIVE_SCAN)	
						    	{
						            Is_received_beacon=TRUE;
						            mlmesyn_finished();
							     //printf("mac_current_state_saved is %d",mac_current_state_saved);		
							     //mac_current_state=mac_current_state_saved;
						            phy_channel_state=PHY_IDLE;	
						            PLME_SET_TRX_STATE_request(RX_ON);	 
						            //Is_received_beacon=TRUE;	 
							     //-------------------------------------		
							     //mac_current_state=MAC_STATE_SYNING_END;		
						            //printf("\n------synchronizing success------ \n");	 
						    	}
						     
			        	            }
			        	}
	                      break;
	                case   MAC_FRAME_DATA: 
	  //����յ���Ϊdata frame
	  	  //ȷ��ϵͳΪ��������״̬�����¸�֡����ͨ��ԭ��mcpsDataIndication��߲㱨�档
	  	  //���鿴����֡�е�pending�������������֡���������������data request����.
                                  //printf("\n---received the data frame1----\n");
	                           
				       if(mac_current_state==MAC_STATE_IDLE)
                             	{
                             	  //printf("\n---received the data frame2----\n");
                             	    mcps_data_indication_info.src=mac_rx_current_frame.src;
								  //printf("\nthe src mode is %02x\n",mcps_data_indication_info.src.addrMode);
					   					    //for(uint8_t i=0;i<8;i++)
							//printf("\nthe src addr[%d] is %x\n",i,mcps_data_indication_info.src.longAddr.v[i]);			  
					    mcps_data_indication_info.dst=mac_rx_current_frame.dst;
					    mcps_data_indication_info.mpduLinkQuality=mac_rx_current_frame.lqi;
					   mcps_data_indication_info.msduLength=mac_rx_current_frame.nwk_data_length;
					    for(i=0;i<mcps_data_indication_info.msduLength;i++)
							mcps_data_indication_info.pMsdu[i]=mac_rx_current_frame.nwk_data_payload[i];
                             	    mcpsDataIndication(mcps_data_indication_info);
									
                             	}
	                      break;
                       case   MAC_FRAME_CMD:
			       switch(mac_rx_current_frame.cmd)
			       	{
			       	        case  MAC_CMD_ASSOCIATE_REQ:
	  //����յ���Ϊassociation request֡(ֻ��Э���ߵĴ���Ųμӱ���)
	      //ȷ��ϵͳΪ��������״̬��(�����Ѿ�����)�����¸�֡��,ͨ��mlmeAssociateIndicationԭ��֪ͨ�߲�(�߲���ܷ���responseԭ��)
                                                 //printf("\n----------------sucess received a associate req frame----------mac_current_state is %d---\n",mac_current_state);
							if((mac_current_state==MAC_STATE_IDLE)&&(IS_FFD!=0x00))
	                                         	{
	                                         	    //printf("\n----------------sucess received a associate req frame---------------\n");
	                                              mlme_associate_indication_info.cap_info=mac_rx_current_frame.node_capinfo_field;
	                                              mlme_associate_indication_info.Deviceaddr=mac_rx_current_frame.src.longAddr;
	                                              mlmeAssociateIndication(mlme_associate_indication_info);
	                                         	}
                                                break;
						 case  MAC_CMD_ASSOCIATE_RPLY:
	   //����յ���Ϊassociation response֡
	      //ȷ��ϵͳMAC����״̬Ϊwaittingforresponse״̬(�Ѿ�����data request���յ�ack),ȡ����ʱ�����е�WAIT_FOR_DATA��ʱ����
	      //��ʼ������幤���������յ���Э���߶�Ӧ�Ĳ��������ö�Ӧ��PIB����
	      //����ɹ���������ı�ϵͳMAC״̬Ϊ��������״̬
	      //����mlme-associate.confirmԭ����ʾ״̬
	                                         //printf("\n-------success received a association reply frame------\n");
	                                         if((mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE)&&(!IS_COORDINATOR))
	                                         	{
	                                         	   printf("\n-------success received a association reply frame------\n");
	                                         	   head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, 0, WAIT_FOR_DATA);
							   mac_current_state=MAC_STATE_IDLE;
							   mlmeAssociateConfirm(mac_rx_current_frame.shortaddralloc, mac_rx_current_frame.association_status);
	                                         	}
	                                         break;
                                           case  MAC_CMD_DISASSOCIATE_NOTICE:
	   //����յ���Ϊdisassociation notification֡(Э���ߺ�enddevice���п����յ���֡):
	      //��ΪЭ�����յ���֡������Ҫͨ��mlmeDisassociateIndicationԭ�ﱨ����߲㣬��ɾ����Ӧ��enddevice��¼��
	      //��Ϊ�ն��豸�յ���֡����ͬ����Ҫͨ��mlmeDisassociateIndicationԭ�ﱨ����߲�,��ɾ����Ӧ��PAN��¼.ȡ����ʱ��waitfordata����
	      //�޸�ϵͳ��MAC���״̬����Ϊδ����״̬
	                                         if((IS_FFD!=0x00))
	                                         	{
	                                         	    
	                                         	    mlmeDisassociateIndication(mac_rx_current_frame.src.longAddr, mac_rx_current_frame.disassociation_reason, FALSE, 0x00);
	                                         	}
						      else
						      	{
						      	  //printf("\n--received a disassociation notice frame---\n");
	                                         	    //if(mac_current_state==MAC_STATE_IDLE)
	                                         	    {
	                                         	         head_of_timer_queue=timer3quere_cancel(head_of_timer_queue,0,WAIT_FOR_DATA);
								  mac_current_state=MAC_STATE_DISASSOCIATED;
	                                         	         mlmeDisassociateIndication(mac_rx_current_frame.src.longAddr, mac_rx_current_frame.disassociation_reason, FALSE, 0x00);
							         
	                                         	     };
						      	}
	                                         break;
                                           case MAC_CMD_DATA_REQ:
	   //����յ���Ϊdata request֡(ֻ��Э���ߵĴ���Ųμӱ���)
	      //����һ�����ݷ�������:����data request֡�е�Դ��ַ����pending������ѡ�����ݽ��з���,���û�����ݣ����͵�����payload��Ϊ�գ����Ҳ���Ҫack
	      //����ֱ�ӷ��ء�
	                                         if(IS_FFD!=0x00)
	                                         	{
	                                         	   //printf("\ndata request frame received\n");
	                                             pending_data_send(mac_rx_current_frame.src);
	                                         	}
	                                         break;
	                                    case MAC_CMD_PAN_ID_CONFLICT:
	  //����յ���ΪPANID confilict֡(ֻ��Э���ߵĴ���Ųμӱ���)
	      //Э���������������Ĺ���״̬(�����Ѿ�����)������Ҫͨ��mlmeSyncLossIndicationԭ��֪ͨ�߲�,����ʧͬ��ԭ��ΪPANIDconfilict	                                         if((mac_current_state==MAC_STATE_IDLE)&&(IS_FFD!=0x00))
	                                         	{
	                                         	    mlmeSyncLossIndication(PAN_ID_CONFLICT);
							    
							    //mac_current_state=MAC_STATE_ID_CONFLICT;
	                                         	}
	                                         break;
                                           case MAC_CMD_ORPHAN_NOTICE:
	  //����յ���ΪOrphan notification֡(ֻ��Э���ߵĴ���Ųμӱ���)
	      //Ѱ�Ҷ�Ӧ�ն��豸�Ĺ�����¼,����ҵ�������÷��ͽ��̣�����realignment֡����Ӧ�豸��
	      //���û���ҵ���Ӧ��¼����discard��֡
	                                         if((mac_current_state==MAC_STATE_IDLE)&&(IS_FFD!=0x00))
	                                         {
	                                         	    mlmeOrphanIndication(mac_rx_current_frame.src.longAddr, 0,0x00);
	                                         	    //coordinator_realignment_frame_send();��mlme-orphan.responseԭ�﷢��.
	                                         }
	                                         break;
                                           case MAC_CMD_BEACON_REQ:
	  //����յ���ΪBeacon request֡(ֻ��FFD�Ĵ���Ųμӱ���)
	      //��������ʵ�ֵ����ű�ʹ�����磬���Ժ��Ե���֡�������ڷ����ű�֡��OK��
	                                        break;
                                           case MAC_CMD_COORD_REALIGNMENT:
	  //����յ���ΪCoordinator realignment֡
	      //��ϵͳMAC���Ѿ�������������״̬(�����Ѿ�����)�������mlmeSyncLossIndication֪ͨ�߲㣬����loss reason��ΪRealignment�����޸�ϵͳ��MAC״̬������
	      //��ϵͳMAC��������ORPHAN SCAN״̬����ȡ����ʱ�����ж�Ӧ�ĵȴ����񣬲���¼��֡�еĲ�����Ȼ����scan confirmԭ����߲㣬�޸�ϵͳMAC��״̬������ 
	                                        if(!IS_COORDINATOR)
	                                        {
	                                            if(mac_current_state==MAC_STATE_ORPHAN_SCAN)
	                                            {

								printf("\n---success received the coordinator realignment frame----\n");					 
	                                                 
	                                                 //NOTE:��orphanscanʱ�����õ���wait_for_response.
								head_of_timer_queue=timer3quere_cancel(head_of_timer_queue, 0, WAIT_FOR_RESPONSE);
                                                        mac_pib_data.macCoordShortAddress=mac_rx_current_frame.coordshortaddr_field;
							       mac_pib_data.macPANId=mac_rx_current_frame.PAN_ID_field;
								//---------------------------------------------
								//------------�����-----------------------
								//���������߼��ŵ�--------------------
								//---------------------------------------------
								mac_pib_data.macShortAddress=mac_rx_current_frame.shortaddrallocted;
								mac_pib_data.macCoordExtendedAddress=mac_rx_current_frame.src.longAddr;
	                                         	       scan_result_info.status=SUCCESS;
	                                         	       scan_result_info.scanType=ORPHAN_SCAN;
								mac_current_state=MAC_STATE_IDLE;
	                                         	       mlmeScanConfirm(scan_result_info);
	                                         	       
	                                            }	
							  else
							  {
							       //
							     //printf("\n---success received a broadcast realignment frame----\n");
                                                        /*mac_pib_data.macCoordShortAddress=mac_rx_current_frame.coordshortaddr_field;
							       mac_pib_data.macPANId=mac_rx_current_frame.PAN_ID_field;
								mac_pib_data.macCoordExtendedAddress=mac_rx_current_frame.src.longAddr;
								mac_current_state=MAC_STATE_REALIGNMENT;
                                                        mlmeSyncLossIndication(REALIGNMENT);
                                                        */
							  }
	                                        }
                                               break;
				   }
	  //�������Σ�discard�յ���֡��
	                   break;
       	}
	   
}




//����Ϊ��֡�ķ��ͺ�����
//������ɹ��ܾ�����дmac_tx_current_frame�и�����
void data_request_frame_send(void)
{
       mac_tx_current_frame.cmd=MAC_CMD_DATA_REQ;
       mac_tx_current_frame.type=MAC_FRAME_CMD;
	mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
       //��ʱ���򻯴���
	mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
	if(mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE)
	{
	     //û�������Э���ߵ��κ���Ϣ
	     //printf("\nno addr mode\n");
	      mac_tx_current_frame.dst.addrMode=0x00;
	      //mac_tx_current_frame.dst.shortAddr=mac_pib_data.macCoordShortAddress;
	      //mac_tx_current_frame.dst.panID=mac_pib_data.macPANId;	 
	}
	else
	{
	    //printf("\nlong addr mode\n");
	      mac_tx_current_frame.dst.addrMode=0x02;
	      mac_tx_current_frame.dst.shortAddr=mac_pib_data.macCoordShortAddress;
	      mac_tx_current_frame.dst.panID=mac_pib_data.macPANId;
	}
	if((mac_pib_data.macShortAddress.Val==default_short_addr.Val)||(mac_pib_data.macShortAddress.Val==no_short_addr.Val))
	{
       	mac_tx_current_frame.src.addrMode=0x03;
		mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
		
	}
	else
	{
	       mac_tx_current_frame.src.addrMode=0x02;
		mac_tx_current_frame.src.shortAddr=mac_pib_data.macShortAddress;
	}
	mac_tx_current_frame.frame_control_field.v[0]=0x23;
	mac_tx_current_frame.frame_control_field.v[1]=(mac_tx_current_frame.dst.addrMode<<2)|(mac_tx_current_frame.src.addrMode<<6);
	mac_frame_send();	 
}

void pending_data_send(NODE_INFO dst)
{
      INDIRECT_TX_PENDING_QUEUE * p_temp;
      //NODE_INFO src;
      p_temp=get_pending_frame(dst);
      if(Is_data_pending(dst))
      	{
      	    while(p_temp!=NULL)
      	    	{
      	    	     if((p_temp->dst.panID.Val==dst.panID.Val)&&((p_temp->dst.longAddr.longaddr==dst.longAddr.longaddr)||(p_temp->dst.shortAddr.Val==dst.shortAddr.Val)))
			 break;		 	
      	    	}
	     if(((p_temp->framectl_field.v[0])&0x07)==MAC_FRAME_DATA)
	     	{
	     	     data_frame_send(p_temp->high_level_payload,p_temp->frameLength,p_temp->dst,p_temp->src,p_temp->FrameHandle);
	     	}
            else if(p_temp->associate_or_disassociate)
            	{
            	     //����associate��Ӧ֡ 
            	     associate_response_frame_send(mac_pib_data.macCoordShortAddress, p_temp->associate_status, p_temp->associate_device_addr, p_temp->shortaddralloc);
            	}
	      else
	      	{
	      	     //����disassociate notification֡
	      	     disassociate_notification_frame_send(p_temp->disassociation_reason, p_temp->end_device_addr);
	      	}
		remove_pending_queue(p_temp);
      	} 
	else 
         
	 return;
}

void  data_frame_send(BYTE * nwkpayload, BYTE nwkpayloadlength, NODE_INFO dst,NODE_INFO src,BYTE frameHandle)
{
      int i;
      mac_tx_current_frame.cmd=0x00;
      mac_tx_current_frame.type=MAC_FRAME_DATA;
      mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
      mac_tx_current_frame.dst=dst;
      mac_tx_current_frame.src=src;
      mac_tx_current_frame.nwk_data_length=nwkpayloadlength;
      mac_tx_current_frame.framehandle=frameHandle;
      for(i=0;i<nwkpayloadlength;i++)
          mac_tx_current_frame.nwk_data_payload[i]=*nwkpayload++;
      if((Is_data_pending(dst))&&(IS_COORDINATOR))
         mac_tx_current_frame.frame_control_field.v[0]=0x31;
      else
         mac_tx_current_frame.frame_control_field.v[0]=0x21;
      
      mac_tx_current_frame.frame_control_field.v[1]=(mac_tx_current_frame.dst.addrMode<<2)|(mac_tx_current_frame.src.addrMode<<6);
      	  
      mac_frame_send();
}

bool  Is_data_pending(NODE_INFO dst)
{
       INDIRECT_TX_PENDING_QUEUE * p_temp;
	p_temp=head_of_indirect_pending_buffer;
	while(p_temp!=NULL)
		{
		     if((p_temp->dst.longAddr.longaddr==dst.longAddr.longaddr)||(p_temp->dst.shortAddr.Val==dst.shortAddr.Val))
			 return TRUE;
		       p_temp=p_temp->next;
		}
	return FALSE;
}

void  associate_request_frame_send(SHORT_ADDR panid,NODE_INFO dst, Node_Capability node_capinfo)
{
      mac_tx_current_frame.cmd=MAC_CMD_ASSOCIATE_REQ;
      mac_tx_current_frame.type=MAC_FRAME_CMD;
      mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
      mac_tx_current_frame.src.addrMode=0x03;
      mac_tx_current_frame.dst=dst;
      mac_tx_current_frame.src.panID=default_short_addr;
      mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
      mac_tx_current_frame.node_capinfo_field=node_capinfo;
      mac_tx_current_frame.frame_control_field.v[0]=0x23;
      //printf("\nthe dst.addrmode is %02x",mac_tx_current_frame.dst.addrMode);	
      //printf("\nthe src.addrmode is %02x",mac_tx_current_frame.src.addrMode);	  
      mac_tx_current_frame.frame_control_field.v[1]=(((uint8_t)mac_tx_current_frame.dst.addrMode)<<2)|(((uint8_t)mac_tx_current_frame.src.addrMode)<<6);
      //printf("\nthe control field isMSB %02x",mac_tx_current_frame.frame_control_field.v[1]);	  
      mac_frame_send();	  
}



void disassociate_notification_frame_send(BYTE disassociation_reason,LONG_ADDR end_device_addr)
{    mac_tx_current_frame.cmd=MAC_CMD_DISASSOCIATE_NOTICE;
    mac_tx_current_frame.type=MAC_FRAME_CMD;  
    mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
    mac_tx_current_frame.src.addrMode=0x03;
   if(!IS_FFD)	
    mac_tx_current_frame.dst.addrMode=0x02;
   else
    mac_tx_current_frame.dst.addrMode=0x03;	
    mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
    mac_tx_current_frame.dst.panID=mac_pib_data.macPANId;	
    mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
    if(IS_FFD!=0x00)
       mac_tx_current_frame.dst.longAddr=end_device_addr;
    else
       mac_tx_current_frame.dst.shortAddr=mac_pib_data.macCoordShortAddress;
    mac_tx_current_frame.disassociation_reason=disassociation_reason;
    mac_tx_current_frame.frame_control_field.v[0]=0x23;
    if(!IS_FFD)	
    mac_tx_current_frame.frame_control_field.v[1]=0xc8;
    else
    mac_tx_current_frame.frame_control_field.v[1]=0xcc;		
    mac_frame_send();
} 

void panid_confilct_frame_send(void)
{
     mac_tx_current_frame.cmd=MAC_CMD_PAN_ID_CONFLICT;
     mac_tx_current_frame.type=MAC_FRAME_CMD;
     mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
     mac_tx_current_frame.src.addrMode=0x03;
     mac_tx_current_frame.dst.addrMode=0x02;
     mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
     mac_tx_current_frame.dst.panID=mac_pib_data.macPANId;
     mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
     //mac_tx_current_frame.dst.longAddr=mac_pib_data.macCoordExtendedAddress;
     mac_tx_current_frame.dst.shortAddr=mac_pib_data.macCoordShortAddress;
     mac_tx_current_frame.frame_control_field.v[0]=0x23;
     mac_tx_current_frame.frame_control_field.v[1]=0xc8;
     mac_frame_send();	 
}

void orphan_notification_frame_send(void)
{
      mac_tx_current_frame.cmd=MAC_CMD_ORPHAN_NOTICE;
      mac_tx_current_frame.type=MAC_FRAME_CMD;
      mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
      mac_tx_current_frame.src.addrMode=0x03;
      mac_tx_current_frame.dst.addrMode=0x02;
      mac_tx_current_frame.src.panID=default_short_addr;
      mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
      mac_tx_current_frame.dst.panID=default_short_addr;
      mac_tx_current_frame.dst.shortAddr=default_short_addr;
      mac_tx_current_frame.frame_control_field.v[0]=0x03;
      mac_tx_current_frame.frame_control_field.v[1]=0xc8;
      mac_frame_send();
      head_of_timer_queue=timer3queue_start(head_of_timer_queue,TIMER_ONE_SHOT, mac_tx_current_frame.macDSN, WAIT_FOR_RESPONSE, aResponseWaitTime);	  
}

void beacon_request_frame_send(void)
{
       
}


//#ifdef I_AM_COORDINATOR
void coordinator_realignment_frame_send(bool Isunicast,  NODE_INFO  dst, SHORT_ADDR allocated_addr)
{
       mac_tx_current_frame.cmd=MAC_CMD_COORD_REALIGNMENT;
       mac_tx_current_frame.type=MAC_FRAME_CMD;
	mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
	mac_tx_current_frame.src.addrMode=0x03;
	mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
	mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
	mac_tx_current_frame.dst.panID=default_short_addr;
       //mac_tx_current_frame.dst.addrMode=0x03;
	if(Isunicast)
		{  
		    //����
		    mac_tx_current_frame.dst.addrMode=0x03;
		    mac_tx_current_frame.dst.longAddr=dst.longAddr;
		    mac_tx_current_frame.shortaddrallocted=allocated_addr;
	           mac_tx_current_frame.frame_control_field.v[0]=0x23;
	           mac_tx_current_frame.frame_control_field.v[1]=0xcc;
		}
	else 
		{  
		    //�㲥
		    mac_tx_current_frame.dst.addrMode=0x02;
	           mac_tx_current_frame.dst.shortAddr=default_short_addr;
		    mac_tx_current_frame.shortaddrallocted=default_short_addr;
		    mac_tx_current_frame.frame_control_field.v[0]=0x03;
                  mac_tx_current_frame.frame_control_field.v[1]=0xc8;	
		}		
	mac_tx_current_frame.PAN_ID_field=mac_pib_data.macPANId;
	mac_tx_current_frame.coordshortaddr_field=mac_pib_data.macShortAddress;
	mac_tx_current_frame.logicalchannel_field=ppib.phyCurrentChanel;
	mac_frame_send();

}
void associate_response_frame_send(SHORT_ADDR coor_short_addr, BYTE associate_status,LONG_ADDR associate_device_addr,SHORT_ADDR shortaddralloc)
{
     mac_tx_current_frame.cmd=MAC_CMD_ASSOCIATE_RPLY;
     mac_tx_current_frame.type=MAC_FRAME_CMD;
     mac_tx_current_frame.macDSN=mac_pib_data.macDSN++;
     mac_tx_current_frame.src.addrMode=0x03;
     mac_tx_current_frame.dst.addrMode=0x03;
     mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
     mac_tx_current_frame.dst.panID=mac_pib_data.macPANId;
     mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
     mac_tx_current_frame.dst.longAddr=associate_device_addr;
     mac_tx_current_frame.shortaddralloc=shortaddralloc;
     mac_tx_current_frame.association_status=associate_status;
     mac_tx_current_frame.frame_control_field.v[0]=0x23;
     mac_tx_current_frame.frame_control_field.v[1]=0xcc;
     //printf("\nassociate response frame send\n");	 
     mac_frame_send();
}

//�����ű�֡��ʱ���������
void beacon_frame_tx_timing(void)
{
     uint32_t  deltatime;
     uint32_t truetime;
     truetime=systime32();
     deltatime=(truetime>=last_beacon_tx_timestamp) ? truetime-last_beacon_tx_timestamp:(0x3fffffff-last_beacon_tx_timestamp+truetime);
     //deltatime=deltatime>>1;
     head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, CAP_OVER, (MAC_CAP_PERIOD-(deltatime>>1)));
     head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, BEACON_SEND, (MAC_SUPERFRAME_PERIOD-(deltatime>>1)));
     //printf("\n ---------delta time is %08lx\n",deltatime);	 
     //printf("\n ---------cap   time is %08lx\n",MAC_CAP_PERIOD);
     //printf("\n ---------super time is %08lx\n",MAC_SUPERFRAME_PERIOD);	
     MAC_OBJECT_CAP_START_TICKS=last_beacon_tx_timestamp;
     MAC_OBJECT_CAP_END_TICKS=truetime+((MAC_CAP_PERIOD<<1)-deltatime);
     MAC_OBJECT_SUPERFRAME_TICKS=truetime+((MAC_SUPERFRAME_PERIOD<<1)-deltatime);    	 
}

//����֮ǰҪ��mac_tx_beacon���
void  beacon_frame_send(bool short_or_long)
{
      uint8_t  i;
      
        
      mac_tx_current_frame.type=MAC_FRAME_BEACON;
      mac_tx_current_frame.macBSN=mac_pib_data.macBSN++;
      mac_tx_current_frame.dst.addrMode=0x00;
      mac_tx_current_frame.src.panID=mac_pib_data.macPANId;
      if(short_or_long)	  
      {
          mac_tx_current_frame.src.addrMode=0x02;
	   mac_tx_current_frame.src.shortAddr=mac_pib_data.macShortAddress;
      } 
      else
      {
          mac_tx_current_frame.src.addrMode=0x03;
          mac_tx_current_frame.src.longAddr=mac_pib_data.macExtAddress;
      }
      mac_tx_current_frame.superframe_field=mac_tx_beacon.superframe_field;
      mac_tx_current_frame.pendaddress_field=mac_tx_beacon.pendaddress_field;
      mac_tx_current_frame.shortpendingadd_length=mac_tx_beacon.shortpendingadd_length;
      for(i=0;i<mac_tx_beacon.shortpendingadd_length;i++)
	  	mac_tx_current_frame.shortaddrpending_list[i]=mac_tx_beacon.shortaddrpending_list[i];
      mac_tx_current_frame.longpendingadd_length=mac_tx_beacon.longpendingadd_length;
      //printf("\nthe longpending addr length is %d\n",mac_tx_beacon.longpendingadd_length);	  
      for(i=0;i<mac_tx_beacon.longpendingadd_length;i++)
      	{
      	      // printf("\nthere is a long address\n");
	  	mac_tx_current_frame.longaddrpending_list[i]=mac_tx_beacon.longaddrpending_list[i];
      	}	
     mac_tx_current_frame.beacon_payload_length=mac_tx_beacon.beacon_payload_length;
     for(i=0;i<mac_tx_beacon.beacon_payload_length;i++)
	 	mac_tx_current_frame.beacon_payload[i]=mac_tx_beacon.beacon_payload[i];
     mac_tx_current_frame.frame_control_field.v[0]=0x00;
     mac_tx_current_frame.frame_control_field.v[1]=0xc0&(mac_tx_current_frame.src.addrMode<<6);
     //printf("\nframe_ctl field is %02x--\n",mac_tx_current_frame.frame_control_field.v[1]);	 

     //postһ��������pending�����е�Ԫ�س�ʱ����
     OS_post(pending_frame_handle);
	 
     mac_frame_send();        
}

void pending_frame_handle(void)
{
      INDIRECT_TX_PENDING_QUEUE *p_temp;
      p_temp=head_of_indirect_pending_buffer;
      while(p_temp!=NULL)
      	{
      	     //printf("\nindirect buffer is not NULL\n");
	  	p_temp->MacTranPersistCounter--;
		//p_temp=p_temp->next;
		if((p_temp->MacTranPersistCounter)==0)
		{
		     if((p_temp->framectl_field.v[0]&0x07)==0x01)
			 mcpsDataConfirm(TRANSACTION_EXPIRED, p_temp->FrameHandle);
		     //else   //����association response֡�ķ�������
		     //mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS * pSrcAddr, BYTE dstAddrMode, ADDRESS * pDstAddr, BYTE status)
		     remove_pending_queue(p_temp);
			 
		     	 
		}
		p_temp=p_temp->next;
      	}	
}
//#endif
//ʱ����е�������
void  timerqueue_adjust(void)
{
     current_timer_elem=head_of_timer_queue;
     current_timer_elem->next=NULL;
     head_of_timer_queue=head_of_timer_queue->next;
     //return  current_timer_elem;
}

void mac_timer_task(void)
{
	 //��Ҫ�������������͵Ķ�ʱ��ϵͳ�ĵ�ǰ״̬����һЩ��Ϊ����post��Ӧ������
	 //ά��ʱ�����
	TX_BUFFER * p_temp;
	//timerqueue_adjust();
	 
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	switch(current_timer_elem->timer_reason)
		{
		     case  WAIT_FOR_ACK:
			 	p_temp=get_frame_pointer(current_timer_elem->framedsn);
				if(p_temp==NULL)
					break;				    
	 //�����Ӧ�Ķ�ʱ����ԭ��Ϊack��ʱ������
	    //�����֡�е�FrameRetriesCounterΪ0������Ҫͨ����Ӧ��ԭ��(�磺confirm,mlme-comm-status)��߲㱨��NO_ack(����ͬmac_frame_rcvd)����������֡����.
	    //����������£�
	                     if(p_temp->FrameRetriesCounter!=0)
	                     {
	                     	    //�ط�;
	                     	     //printf("\n---Retransmit---\n");
	                     	       if(Parameter_list.top==((Parameter_list.tail+1)%MAX_TX_BUFFER_SIZE))
   	                                       error("\ntx buffer is full\n");
                                          else
                                            {
                                               Parameter_list.parameter_tx_task[Parameter_list.tail]=p_temp;
						     Parameter_list.tail=((Parameter_list.tail+1)%MAX_TX_BUFFER_SIZE);					   
                                               OS_post(transmit_with_csmaca); 
	                     	         } 
					      //ACK��ʱ�����������ڵײ㷢�����ʱ�����õ�.
						p_temp->FrameRetriesCounter--;
	                     }
				else  
				{
	                     switch(p_temp->cmd_type)
	                     	{
				          case MAC_CMD_DATA_REQ:
	        //��Ϊdata request֡�� *************Э����û�пɿ��Ա�֤(Ҳ��̫���ܷ�����������)******************
	              //����associate�е�datarequest:������mlme-association.confirm NO_data���ظ��߲�
	                                       //if(mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE) 
	              //����disassociation�е�data request:���Բ���mlme-disassociation.confirmԭ�״̬ΪNO ack
	              //����data��data request:���Բ���***********(û��������ԭ���ֱ�ӷ��أ�������)**************
	                                       break;
                                      case MAC_CMD_ASSOCIATE_REQ:
						     if(mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE) 
							 mlmeAssociateConfirm(default_short_addr, NO_ACK);
	        //��Ϊassociation request֡,����mlme-association.confirm��״̬ΪNO_ack
	                                        break;
	                               case 0x00:
						     mcpsDataConfirm(NO_ACK, p_temp->FrameHandle);
						     break;
	        //��Ϊdata֡,����mlme-data.confirm,״̬ΪNO_ack
	                               case MAC_CMD_DISASSOCIATE_NOTICE:
						     mlmeDisassociateConfirm(NO_ACK);
						     break;
	        //��Ϊdisassociation notification֡,��ͨ��mlme-disassociate.confirm��״̬ΪNO_ack
	                               case MAC_CMD_PAN_ID_CONFLICT:
						     break;
	        //��ΪPANID confilict notification֡,  *************Э����û�пɿ��Ա�֤****************
	                               case MAC_CMD_ASSOCIATE_RPLY:
						     break;
	        //��Ϊassociate response֡,����mlme-comm-statusԭ�״̬Ϊno_ack.
	                               case MAC_CMD_COORD_REALIGNMENT:
						     break;
	        //��Ϊcoordinator realignment֡,����mlme-comm-statusԭ�״̬Ϊno_ack.

	                     	}
				 remove_tx_queue(p_temp->FrameSeq);
				}
				 break;		 
	           case  WAIT_FOR_RESPONSE:
	 //�����Ӧ�Ķ�ʱ����ԭ��Ϊresponse��ʱ������
	                     if((mac_current_state==MAC_STATE_ASSOC_WAIT_FOR_RESPONSE)||(mac_current_state==MAC_STATE_INACTIVE))
						 	mlmeAssociateConfirm(default_short_addr, NO_DATA);
	    //����ǵȴ�association response֡������trackbeaconΪ�棬�����MLME-ASSOCIATE.confirmԭ��֪ͨ�߲㣬״̬��ΪNO_DATA	
	        //���trackbeaconΪ�٣���ֱ�ӵ��÷���������data request֡��
	    //���������orphanscan���ȴ�coordinator realignment����֡������Ϊ���ŵ���û��PAN,�ص���Ƶ���գ���������һ���ŵ���(����orphan_scan_done)������������ʱ����Ȼ���˳���
	                     else if(mac_current_state==MAC_STATE_ORPHAN_SCAN)
	                     	{
	                     	    //�л�����һ���ŵ��Ͻ���scan
	                     	    //-------------------------------------------------------
	                     	    //---------------�����-------------------------------
	                     	    //----------���������ŵ�Ϊ��һ���ŵ�-------
                                       //������е��ŵ����Ѿ�scan�������ϱ�������Ϣ.
					    //����orphan_notification֡
	                     	    orphan_notification_frame_send();
					    			
	                     	}
				break;
	    //Ŀǰ���뵽������???????????????
	           case WAIT_SCAN:
	                    if(mac_current_state==MAC_STATE_ACTIVE_PASSIVE_SCAN)
	                    	{
			               while((scan_channel&((uint32_t)1<<scan_channel_index))==0)
			               	{
					              scan_channel_index++;
						       if(scan_channel_index>=16)
						       	{
						       	   //mac_current_state=MAC_STATE_ACTIVE_PASSIVE_SCAN_FINISHED;
								   phy_channel_state=INACTIVE_PERIOD;
								   PLME_SET_TRX_STATE_request(TRX_OFF);
								   mlmeScanConfirm(scan_result_info);
							   	   return;
						       	}
			               	}
			               PLME_SET_request(phyCurrentChanel_id, scan_channel_index++);  			  
			               PLME_SET_TRX_STATE_request(RX_ON);			  
			               phy_channel_state=WAITING_FOR_BEACON;	
				        head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, 0, WAIT_SCAN, scan_time_period);
	                     	 printf("\nset the %d channel & waitting for beacon\n",scan_channel_index-1);   
	                    	}
	                    break;
	           case WAIT_FOR_DATA:
	 //�����Ӧ�Ķ�ʱ����ԭ��Ϊ����data request��ʱ������(��������data request���յ�ack��ȴ��Է�����)
	    //���������֡��data request,******************û�ж�Ӧ�Ĵ�ʩ����֤************************ֱ���˳�
	    //�����associate response֡��data request,�� ����mlme-associate-confirm,��״̬ΪNO-DATA
                           mlmeAssociateConfirm(default_short_addr, NO_DATA);
	                    break;
	    //�����disassociation notification��*********************û�ж�Ӧ�Ĵ�ʩ����֤*************ֱ���˳�
	           case WAIT_FOR_BEACON:
	 //�����Ӧ�Ķ�ʱ����ԭ��Ϊ�ȴ��ű�֡��ʱ������
	    //ȷ��ϵͳ��MAC����״̬Ϊͬ��ing״̬
	                   //NOTE:�������ֻ�ȴ�һ��.
	                   if(IS_MAC_SYNING)
				mlmeSyncLossIndication(BEACON_LOSS);
			     break;
	        //���Ϊ�٣������mlmeSyncLossIndication֪ͨ�߲㣬��ʧȥͬ��
	        //���Ϊ�棬���aMaxLostBeacons�ݼ�1,����Ѿ�����0�������mlmeSyncLossIndication֪ͨ�߲㣬��ʧȥͬ��
	        //���û�м����㣬��Ҫ����������ʱ��������
		}
       //ɾ����ʱ����
	free(current_timer_elem); 
 
}


uint16_t mac_timer3queue_getnextinterval(void)
{
     //���ض�ʱ����һ����ʱֵ
     //���û�У��򷵻���
     Timer_Queue_pointer  temp;
     temp=head_of_timer_queue->next;
     if(temp!=NULL) 
	 return (temp->ticks);
     else
	 return 0;
}

void initial_tx_queue(void)
{

}

TX_BUFFER * add_tx_queue(BYTE framelength, BYTE * payload, BYTE framehandle, bool is_need_ack, BYTE frameseq,BYTE cmd_type)
{
       TX_BUFFER  * current;
	current=(TX_BUFFER *)malloc(sizeof(TX_BUFFER));
	if(current==NULL)
	{
		error("\nmemory allocated failure\n");
		return NULL;
	}	
	current->payload=payload;
	current->FrameHandle=framehandle;
	current->FrameRetriesCounter=aMaxFrameRetries;
	current->frameLength=framelength;
	current->FrameSeq=frameseq;
	current->next=NULL;
	current->IsNeedAcked=is_need_ack;
	current->IsTxed=0;
	current->cmd_type=cmd_type;
       if(tail_of_tx_buffer==NULL)
        {
              //printf("\nadd to the head\n");
	   	head_of_tx_buffer=current;
		tail_of_tx_buffer=current;
        }
	 else
	 {
	 	tail_of_tx_buffer->next=current;
	       tail_of_tx_buffer=current;
	 }	   
	 return  current;
}
void remove_tx_queue(BYTE frameseq)
{
       TX_BUFFER  * p_temp1;
	TX_BUFFER  * p_temp2;
	p_temp1=head_of_tx_buffer;
	p_temp2=head_of_tx_buffer;
	while((p_temp2->FrameSeq!=frameseq)&&(p_temp2!=NULL))
	{
		      p_temp1=p_temp2;
		      p_temp2=p_temp2->next;
	}
	if(p_temp2==NULL)
		error("\n cannot find the corresponding frame in the tx buffer\n");
	else
	{
	       if(p_temp1==p_temp2)
	       	{
		   	  head_of_tx_buffer=p_temp1->next;
			  tail_of_tx_buffer=p_temp1->next;
	       	}
		else
		{
		       p_temp1->next=p_temp2->next;
		       p_temp2->next=NULL;
		}
		//printf("\nsuccess free the tx ram,the frameseq is %d\n",p_temp2->FrameSeq);
		free(p_temp2->payload);
		free(p_temp2);
		
	}
	 
}
TX_BUFFER * get_frame_pointer(BYTE frameseq)
{
       TX_BUFFER  * temp;
	   temp=head_of_tx_buffer;
	while(temp->FrameSeq!=frameseq)
		temp=temp->next;
	return temp;
}

RX_BUFFER * add_to_rx_buffer(BYTE framelength, BYTE *payload,BYTE lqi,uint32_t timestamp)
{
       RX_BUFFER  * temp;
	temp=(RX_BUFFER *)malloc(sizeof(RX_BUFFER));
	if(temp==NULL)
	{
		error("\nmemory allocated failure\n");
		return NULL;
	}
	temp->framelength=framelength;
	temp->payload=payload;
	temp->lqi=lqi;   
	temp->timestamp=timestamp;
	temp->next=NULL;
	if(tail_of_rx_buffer==NULL)
	{
	     head_of_rx_buffer=temp;
	     tail_of_rx_buffer=temp;
	}
	else
	{
	    tail_of_rx_buffer->next=temp;
	    tail_of_rx_buffer=temp;
	}
	return   temp;
}
void  remove_from_rx_buffer(RX_BUFFER  * framepointer)
{
       /*
        RX_BUFFER * p_temp1;
	RX_BUFFER * p_temp2;
	p_temp1=head_of_rx_buffer;
	p_temp2=tail_of_rx_buffer;
	while((p_temp2!=framepointer)&&(p_temp2!=NULL))
	{
		    p_temp1=p_temp2;
		    p_temp2=p_temp2->next;
	}
	if(p_temp2==NULL)
	{
	        error("\nthe frame is not in the rx buffer\n");
		 return;
	}
	else
	{
	       if(p_temp1==p_temp2)
		   	head_of_rx_buffer=p_temp2->next;
		else 
		{
		       p_temp1->next=p_temp2->next;
		       p_temp2->next=NULL;
		}
	}
	*/
	free(framepointer->payload);
       free(framepointer);   
       //printf("\n***************succeed free a physical packet*********************\n");  
}

//������������ݵ������ṩ�Ĳ�����Ϣ����ɺϷ���MAC֡���з��͡�
//�����ű�֡��ack֡��������øú�����
//
 //����֡���͵Ȳ�����Ϣ��֯�ɺϷ���MAC֡
   //��������ŵ��Ƿ�Ϸ�
   //�����Ҫ�����������͵ģ������õ��ú���direct_transmit����ʽ(ʵʱ�Ը�)
   //�����Ҫ����csma_ca�㷨���͵ģ�������postһ��������transmit_with_csmaca(ʵʱ�Ե�)
void mac_frame_send(void)
{
   TX_BUFFER  * temp;
   //MAC�������򳤶�֮�ͣ���������FCS��
   BYTE framelength;
   BYTE * payload;
   BYTE framehandle;
   bool is_need_ack;
   BYTE frameseq;
   uint8_t  index;
   BYTE  cmd_type;
   uint8_t  i;
   index=0;
   uint32_t  return_send_timestamp;
   //framelength=mac_tx_current_frame.frameLength;
   payload=(BYTE *)malloc(aMaxMACFrameSize*sizeof(BYTE));
   payload[index++]=mac_tx_current_frame.frame_control_field.v[0];
   payload[index++]=mac_tx_current_frame.frame_control_field.v[1];
   //printf("\nframe_ctl field is %02x--\n",payload[1]);
   payload[index++]=mac_tx_current_frame.macDSN;
   frameseq=mac_tx_current_frame.macDSN;
   is_need_ack=mac_tx_current_frame.frame_control_field.bits.AckReq;
   cmd_type=0x00;//��������֡
   framehandle=mac_rx_current_frame.framehandle;//������֡���õ���
   //printf("the transmitted beacon frame length is %d", index);
   
   switch(mac_tx_current_frame.frame_control_field.v[1]&0x0c)
   	{
   	     case  MAC_DST_LONG_ADDR:
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	payload[index++]=mac_tx_current_frame.dst.panID.v[0];
			payload[index++]=mac_tx_current_frame.dst.panID.v[1];
			//��˸��ֽڷų�Ŀ�ĵ�ַ		 	
		 	payload[index++]=mac_tx_current_frame.dst.longAddr.v[0];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[1];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[2];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[3];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[4];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[5];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[6];
			payload[index++]=mac_tx_current_frame.dst.longAddr.v[7];
		 	break;
	     case  MAC_DST_SHORT_ADDR:
		 	//ǰ�����ֽڷ�Ŀ��PANID
		 	payload[index++]=mac_tx_current_frame.dst.panID.v[0];
			payload[index++]=mac_tx_current_frame.dst.panID.v[1];
			//�������ֽڷŶ�Ŀ�ĵ�ַ
		 	payload[index++]=mac_tx_current_frame.dst.shortAddr.v[0];
			payload[index++]=mac_tx_current_frame.dst.shortAddr.v[1];
		 	break;
	     case  MAC_DST_NO_ADDR:
		 	break;
	     case  MAC_DST_ADDR_RESERVED:
		 	error("\nreserved dst address mode is used!\n");
		 	break;
   	}
   switch(mac_tx_current_frame.frame_control_field.v[1]&0xc0)
   	{
   	     case  MAC_SRC_LONG_ADDR:
		 	//ǰ�����ֽڷ�ԴPANID
		 	payload[index++]=mac_tx_current_frame.src.panID.v[0];
			payload[index++]=mac_tx_current_frame.src.panID.v[1];
			//��˸��ֽڷų�Դ��ַ		 	
		 	payload[index++]=mac_tx_current_frame.src.longAddr.v[0];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[1];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[2];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[3];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[4];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[5];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[6];
			payload[index++]=mac_tx_current_frame.src.longAddr.v[7];
		 	break;
	     case  MAC_SRC_SHORT_ADDR:
		 	//ǰ�����ֽڷ�ԴPANID
		 	payload[index++]=mac_tx_current_frame.src.panID.v[0];
			payload[index++]=mac_tx_current_frame.src.panID.v[1];
			//�������ֽڷŶ�Դ��ַ
		 	payload[index++]=mac_tx_current_frame.src.shortAddr.v[0];
			payload[index++]=mac_tx_current_frame.src.shortAddr.v[1];
		 	break;
	     case  MAC_SRC_NO_ADDR:
		 	break;
	     case  MAC_SRC_ADDR_RESERVED:
		 	error("\nreserved src address mode is used!\n");
		 	break;   	     
   	}
       
   switch(mac_tx_current_frame.type)
   	{
   	     case  MAC_FRAME_BEACON:
		 	//��֡�ṹ
		 	//printf("the transmitted beacon frame length is %d", index);
		 	payload[index++]=mac_tx_current_frame.superframe_field.SF.bytes.LSB;
			payload[index++]=mac_tx_current_frame.superframe_field.SF.bytes.MSB;
                     //GTS��Ϣ���汾û��ʵ��gts�����Э��
			payload[index++]=0x00;
			//pending��Ϣ
			payload[index++]=mac_tx_current_frame.pendaddress_field.bytes;
			for(i=0;i<mac_tx_current_frame.shortpendingadd_length;i++)
			{
			     payload[index++]=mac_tx_current_frame.shortaddrpending_list[i].v[0];
			     payload[index++]=mac_tx_current_frame.shortaddrpending_list[i].v[1];
			}
			for(i=0;i<mac_tx_current_frame.longpendingadd_length;i++)
			{
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[0];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[1];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[2];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[3];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[4];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[5];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[6];
			     payload[index++]=mac_tx_current_frame.longaddrpending_list[i].v[7];
			}
			//�ű�֡����
			payload[index++]=mac_tx_current_frame.beacon_payload_length;
			for(i=0;i<mac_tx_current_frame.beacon_payload_length;i++)
				payload[index++]=mac_tx_current_frame.beacon_payload[i];
			//printf("\nthe transmitted beacon frame length is %d", index);
			//for(uint8_t i=0;i<index;i++)
			//	printf("\nthe beacon frame payload[%d] is %02x\n",i,payload[i]);
			last_beacon_tx_timestamp=direct_transmit(index, payload);
			//printf("\nthe transmitted beacon frame time is %08lx", last_beacon_tx_timestamp);
			//Leds_yellowToggle();
			phy_channel_state=PHY_IDLE;
			mac_current_state=MAC_STATE_IDLE;
			PLME_SET_TRX_STATE_request(RX_ON);
			OS_post(beacon_frame_tx_timing);
			//for(uint8_t i=0;i<index;i++)
			//	printf("\nthe beacon frame payload[%d] is %02x\n",i,payload[i]);
			return;
		 	break;
	     case  MAC_FRAME_DATA:
		 	//������·������
		 	payload[index++]=mac_tx_current_frame.nwk_data_length;
		 	for( i=0;i<mac_tx_current_frame.nwk_data_length;i++)
				payload[index++]=mac_tx_current_frame.nwk_data_payload[i];
			break;
	     case  MAC_FRAME_ACK:
		 	//�Ѿ��ɵײ㷢��,���ﲻ������֡
		 	break;
	     case  MAC_FRAME_CMD:
		 	{
				payload[index++]=mac_tx_current_frame.cmd;
				cmd_type=mac_tx_current_frame.cmd;
				switch(mac_tx_current_frame.cmd)
					{
					    case MAC_CMD_ASSOCIATE_REQ:
							payload[index++]=mac_tx_current_frame.node_capinfo_field.bytes;
							break;
					    case MAC_CMD_ASSOCIATE_RPLY:
							payload[index++]=mac_tx_current_frame.shortaddralloc.v[0];
							payload[index++]=mac_tx_current_frame.shortaddralloc.v[1];
							payload[index++]=mac_tx_current_frame.association_status;
							//printf("---****index is %d***---\n",index);
							//return;
							break;
				           case MAC_CMD_DISASSOCIATE_NOTICE:
						   	payload[index++]=mac_tx_current_frame.disassociation_reason;
						   	break;
					    case MAC_CMD_DATA_REQ:
							break;
					    case MAC_CMD_PAN_ID_CONFLICT:
							//printf("\n---the panID conflict frame length is %d---\n",index);
							break;
					    case MAC_CMD_ORPHAN_NOTICE:
							break;
					    case MAC_CMD_BEACON_REQ:
							break;
					    case MAC_CMD_COORD_REALIGNMENT:
							payload[index++]=mac_tx_current_frame.PAN_ID_field.v[0];
							payload[index++]=mac_tx_current_frame.PAN_ID_field.v[1];
							payload[index++]=mac_tx_current_frame.coordshortaddr_field.v[0];
							payload[index++]=mac_tx_current_frame.coordshortaddr_field.v[1];
							payload[index++]=mac_tx_current_frame.logicalchannel_field;
							payload[index++]=mac_tx_current_frame.shortaddrallocted.v[0];
							payload[index++]=mac_tx_current_frame.shortaddrallocted.v[1];
							//printf("--the realignment frame length is %d--",index);
							
							break;
					    case MAC_CMD_GTS_REQ:
							//���汾û��ʵ��GTS���Э��
							break;				
					}
	     	       }
		 	break;
   	}
   //if(index!=framelength)
   //	error("\nthe frame length's computing error\n");
   framelength=index;
   //printf("the transmitted  frame length is %d", framelength);
   temp=add_tx_queue(framelength,payload,framehandle,is_need_ack,frameseq,cmd_type);
   head_of_tx_buffer=temp;
   //�м��һ��Ԫ��
   if(Parameter_list.top==((Parameter_list.tail+1)%MAX_TX_BUFFER_SIZE))
   	error("\ntx buffer is full\n");
   else
   {
      Parameter_list.parameter_tx_task[Parameter_list.tail]=temp;
      Parameter_list.tail=((Parameter_list.tail+1)%MAX_TX_BUFFER_SIZE);
      //printf("the transmitted  frame length is %d", framelength);	  
      OS_post(transmit_with_csmaca); 
   }
}


uint32_t direct_transmit(uint8_t psdulength,uint8_t * psdu)
{
	//ֱ�ӷ���֡��������csma_ca
  //���������PD_data_request����
  uint32_t  send_timestamp;
  send_timestamp=PD_DATA_request(psdulength,psdu);
  free(psdu);
  //printf("\ntransmit beacon finished");
  return send_timestamp;
}


void initial_pending_queue(void)
{
    
}

//���һ�����ݰ���pending������
void add_pending_queue(BYTE framehandle, FRAME_CONTROL framectl, NODE_INFO dst, NODE_INFO src, BYTE framelength, BYTE *payload,BYTE associate_status_or_disassociate_reason,bool associate_or_disassociate,LONG_ADDR addr,SHORT_ADDR shortaddralloc)
{
      INDIRECT_TX_PENDING_QUEUE * temp;
      //bool   is_added_before;	  
      temp=(INDIRECT_TX_PENDING_QUEUE *)malloc(sizeof(INDIRECT_TX_PENDING_QUEUE));
      temp->framectl_field=framectl;
      //���ڷ�����֡�����Σ���ֱ����framehandleΪ0
      temp->FrameHandle=framehandle;
      temp->frameLength=framelength;
      temp->high_level_payload=payload;
      temp->MacTranPersistCounter=mac_pib_data.macTransactionPersistenceTime;
      temp->associate_or_disassociate=associate_or_disassociate;
      if((temp->framectl_field.v[0]&0x07)!=0x01)
      	{
      	     if(associate_or_disassociate)
      	     	{
      	     	  
	         temp->associate_status=associate_status_or_disassociate_reason;
	         temp->associate_device_addr=addr;
	         temp->shortaddralloc=shortaddralloc;
      	     	}	 
	      else
	      	{
		  temp->disassociation_reason=associate_status_or_disassociate_reason;
		  temp->end_device_addr=addr;
	      	}  
      	}
      temp->dst=dst;
      temp->src=src;
      temp->IsValid=1;
      temp->next=NULL;
	if(tail_of_indirect_pending_buffer==NULL)
	{
	     head_of_indirect_pending_buffer=temp;
	     tail_of_indirect_pending_buffer=temp;
	}
	else
	{
	    tail_of_indirect_pending_buffer->next=temp;
	    tail_of_indirect_pending_buffer=temp;
	}
	if(dst.addrMode==0x02)
		{
	            /*for(uint8_t i=0;i<mac_tx_beacon.shortpendingadd_length;i++)	
	            	{
			  if(dst.shortAddr.Val==mac_tx_beacon.shortaddrpending_list[i].Val)
			  	
	            	}*/ 	
	    mac_tx_beacon.shortaddrpending_list[mac_tx_beacon.shortpendingadd_length++]=dst.shortAddr;
           mac_tx_beacon.pendaddress_field.bytes=(mac_tx_beacon.pendaddress_field.bytes)+1;	
		}
       else 
       	{
	    mac_tx_beacon.longaddrpending_list[mac_tx_beacon.longpendingadd_length++]=dst.longAddr;
	    mac_tx_beacon.pendaddress_field.bytes=(mac_tx_beacon.pendaddress_field.bytes)+16;	
	    //printf("\nadd the long addr to the pending list\n");	
       	}
	//printf("\nthe longpending addr length is %d\n",mac_tx_beacon.longpendingadd_length);
	current_pending_num++;
}

//��pending������ɾ��һ��Ԫ��
void remove_pending_queue(INDIRECT_TX_PENDING_QUEUE  * current)
{
       uint8_t  index;
       INDIRECT_TX_PENDING_QUEUE * p_temp1;
	INDIRECT_TX_PENDING_QUEUE * p_temp2;
	p_temp1=head_of_indirect_pending_buffer;
	p_temp2=head_of_indirect_pending_buffer;
	while((p_temp2!=current)&&(p_temp2!=NULL))
	{
		    p_temp1=p_temp2;
		    p_temp2=p_temp2->next;
	}
	if(p_temp2==NULL)
	{
	        error("\nthe frame is not in the pending queue\n");
		 return;
	}
	else
	{
	       if(head_of_indirect_pending_buffer==tail_of_indirect_pending_buffer)
	       {
		   	head_of_indirect_pending_buffer=NULL;
			tail_of_indirect_pending_buffer=NULL;
	       }	
		else if(p_temp2==head_of_indirect_pending_buffer)
			{
			     head_of_indirect_pending_buffer=p_temp2->next;
			     p_temp2->next==NULL;
			}
		else
		{
		       p_temp1->next=p_temp2->next;
			if(p_temp2->next!=NULL)   
			tail_of_indirect_pending_buffer=p_temp2->next;   
			else
			tail_of_indirect_pending_buffer=p_temp1;
		       p_temp2->next=NULL;
			
		}
	}
	//ɾ���ű�֡�еĶ�Ӧ��ַ�б�
	if(current->dst.addrMode==0x02)
	{    
	     index=0;
	     while((current->dst.shortAddr.Val!=mac_tx_beacon.shortaddrpending_list[index].Val)&&(index<=mac_tx_beacon.shortpendingadd_length))
	     	     index++;
	     if(current->dst.shortAddr.Val==mac_tx_beacon.shortaddrpending_list[index].Val)	 
	     	{
	     	     //��ʼɾ���ű�֡�е�pending��ַ
	     	     
	     	     for(;index<=mac_tx_beacon.shortpendingadd_length-1;index++)
			    mac_tx_beacon.shortaddrpending_list[index]=mac_tx_beacon.shortaddrpending_list[index+1];
		     mac_tx_beacon.shortpendingadd_length--;
		     mac_tx_beacon.pendaddress_field.bytes=mac_tx_beacon.pendaddress_field.bytes-1;
	     	}
	     else
	       error("\nIt is not in the pending queue\n");	 	
	}
	else if(current->dst.addrMode==0x03)
	{
	     index=0;
	     while((current->dst.longAddr.longaddr!=mac_tx_beacon.longaddrpending_list[index].longaddr)&&(index<=mac_tx_beacon.longpendingadd_length))
	             index++;
	     if(current->dst.longAddr.longaddr==mac_tx_beacon.longaddrpending_list[index].longaddr)
	     	{
	     	       for(;index<=mac_tx_beacon.longpendingadd_length-1;index++)
			     mac_tx_beacon.longaddrpending_list[index]=mac_tx_beacon.longaddrpending_list[index+1];
			mac_tx_beacon.longpendingadd_length--;
			mac_tx_beacon.pendaddress_field.bytes=mac_tx_beacon.pendaddress_field.bytes-16;
	     	}
	}
	
       free(current);      
	current_pending_num--;
}
//����Ŀ�Ľڵ��ַ��Ϣ��ȡpending�е�ָ��
INDIRECT_TX_PENDING_QUEUE * get_pending_frame(NODE_INFO dst)
{
     INDIRECT_TX_PENDING_QUEUE  *p_temp;
     p_temp=head_of_indirect_pending_buffer; 
     while((p_temp!=NULL)&&!((p_temp->dst.panID.Val==dst.panID.Val)&&((p_temp->dst.shortAddr.Val==dst.shortAddr.Val)||(p_temp->dst.longAddr.longaddr==dst.longAddr.longaddr))))
     	 p_temp=p_temp->next;     if(p_temp==NULL)
	 	error("\ncannot find a pending frame in the queue\n");
     return p_temp;
}



uint8_t   csma_state;
#define  csma_state_initial   0
#define   csma_state_delaying   1


TX_BUFFER   * temp_csma_ca_frame;
uint8_t BE;
uint8_t NB;
uint8_t CW;   


uint8_t  random_generate(void)
{
     return 0;
}
/*
uint8_t  wait_for_backoff_boundary(void)
{


       uint32_t  delta; 
	   
    	do{
	    MAC_CURRENT_TICKS=systime32();
	    delta=(MAC_CURRENT_TICKS>MAC_OBJECT_CAP_START_TICKS)?(MAC_CURRENT_TICKS-MAC_OBJECT_CAP_START_TICKS):(MAC_CURRENT_TICKS+0x40000000-MAC_OBJECT_CAP_START_TICKS);
	    if(phy_channel_state!=DOINGCSMA_CA)
                 return 0;
	}while(!(delta%(2*aUnitBackoffPeriod)));
	return 1;	
	
}
*/
uint8_t  do_backoff(void)
{
      uint8_t  random_backoff;
      uint32_t   timing_symbol_value;	  
      random_backoff=random_generate();
      if(random_backoff==0)
            OS_post(CSMA_CA_DOING);
      else
      	{
            random_backoff=(random_backoff<<BE)-random_backoff;	  
            timing_symbol_value=random_backoff*aUnitBackoffPeriod; 	
            head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, CSMA_TIMING, timing_symbol_value);	  
      	}		
}


uint8_t  csma_initial(void)
{

       uint32_t  wait_for_boundary_time;
	uint32_t  delta_time;   
	uint8_t    random_backoff;   
	uint32_t   timing_symbol_value;
       
	
	//�����ĳ�ʼ��
       NB=0;
	CW=1;
	if(mac_pib_data.macBattLifeExt==FALSE)
		BE=mac_pib_data.macMinBE;
	else
		BE=(mac_pib_data.macMinBE>2)? 2:mac_pib_data.macMinBE;
       phy_channel_state=DOINGCSMA_CA;
	//���㶨ʱ���Ķ�ʱֵ,��������ʱ
	MAC_CURRENT_TICKS=systime32();

	//��λ��backoff�ı߽�
       delta_time=(MAC_CURRENT_TICKS>MAC_OBJECT_CAP_START_TICKS)?(MAC_CURRENT_TICKS-MAC_OBJECT_CAP_START_TICKS):(MAC_CURRENT_TICKS+0x40000000-MAC_OBJECT_CAP_START_TICKS);

	//ת����ʵ�ʵķ�����Ŀ  
	delta_time=delta_time>>1;
	wait_for_boundary_time=aUnitBackoffPeriod-(delta_time%aUnitBackoffPeriod);
	random_backoff=random_generate();
	random_backoff=(random_backoff<<BE)-random_backoff;
	timing_symbol_value=wait_for_boundary_time+random_backoff*aUnitBackoffPeriod;
	head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, CSMA_TIMING, timing_symbol_value);
       
	//printf("\n----csma timing  start--time is %08lx---\n",systime32());
}
//bool  a;
void CSMA_CA_DOING(void)
{
        uint8_t  return_status;
	 uint8_t  cca_result;
	 uint32_t random_backoff;
	 uint32_t  csma_ca_send_timestamp;
	 uint32_t   now_time_ticks;
	 uint16_t   timinglength;  
        switch(csma_state)
        	{
		    case  csma_state_initial:
				return_status=csma_initial();
				//printf("\ncsma is init\n");
				csma_state=csma_state_delaying;
				break;
		    case  csma_state_delaying:
				cca_result=PLME_CCA_request();
				Leds_redToggle();
				if((phy_channel_state!=DOINGCSMA_CA))
					{
					   //free(temp_csma_ca_frame);
					   failure_to_tx_with_csma();
					   return;
					}  
				if((cca_result==IDLE)&&(CW!=0))
					{
					    csma_state=csma_state_delaying;
					    head_of_systimer_queue=systimer_start_task(head_of_systimer_queue, CSMA_TIMING, aUnitBackoffPeriod);
					    CW--;	
					}
				else if((cca_result==IDLE)&&(CW==0))
					{
					    //�ɹ�����
					    //printf("\nthe frame length is %d\n",temp_csma_ca_frame->frameLength);
					    csma_ca_send_timestamp=PD_DATA_request(temp_csma_ca_frame->frameLength, temp_csma_ca_frame->payload);
					    phy_channel_state=PHY_IDLE;
					    Leds_redToggle();
				           if(temp_csma_ca_frame->IsNeedAcked)	
				           	{
				           	      phy_channel_state=WAITING_FOR_ACK;
						      PLME_SET_TRX_STATE_request(RX_ON);
						      //wait_for_ack_timing 	  
						      //printf("\nthe frame is need ack\n");
						      now_time_ticks=systime32();
						      timinglength=(uint16_t)((now_time_ticks>=csma_ca_send_timestamp)?(now_time_ticks-csma_ca_send_timestamp):(now_time_ticks+65536-csma_ca_send_timestamp));
							  
						      head_of_timer_queue=timer3queue_start(head_of_timer_queue, TIMER_ONE_SHOT, temp_csma_ca_frame->FrameSeq, WAIT_FOR_ACK, ((mac_pib_data.macAckWaitDuration)-timinglength));
                                                //printf("\n---send at  %08lx---send time is %d---\n",csma_ca_send_timestamp,timinglength);
						      //printf("\ntimer header ->framedsn is %d---\n",head_of_timer_queue->framedsn);
						      //printf("\nthe csma tx frame's dsn is %d\n",temp_csma_ca_frame->FrameSeq);
						     //printf("\nthis frame need acked, the phychannel is %d\n",phy_channel_state);
				           	}
						   //printf("\nthe csma transimitted frame is payload length is: %d",temp_csma_ca_frame->frameLength);   
                                       //for(uint8_t i=0;i<temp_csma_ca_frame->frameLength;i++)
                                       //	{
					    //	 //printf("\nthe csma transimitted frame is payload[%d]: %02x",i,temp_csma_ca_frame->payload[i]);
                                       //	}			   
					    //remove_tx_queue(temp_csma_ca_frame->FrameSeq);
						//if()	
						 //printf("\ncsma transmitted success\n");	
					}
				else
					{
					    CW=1;
					    NB++;
					    BE++;	
					    BE=(BE>aMaxBE)? aMaxBE:BE;
					    if(NB>(mac_pib_data.macMaxCsmaBackoffs))
					    	{
					    	      //����ʧ��,��߲�֪ͨ
					    	      failure_to_tx_with_csma();
					    	}
					    else
					    	{
					    	      csma_state=csma_state_delaying;
					    	      do_backoff();
					    	}
					}
				break;
		    default :
				break;
        	}
}

void  failure_to_tx_with_csma(void)
{

      //����֡������߲�֪ͨ
      //------------------------------
      //------------------------------
      //-----�������------------
      remove_tx_queue(temp_csma_ca_frame->FrameSeq);
    //printf("\n---csma failure----\n");	  
      //free(temp_csma_ca_frame);
}

/*
//slotted CSMA-CA algorithm
void CSMA_CA_DOING(void)
{
       TX_BUFFER * temp;

	uint8_t BE;
       uint8_t NB;
	uint8_t CW;   

	uint8_t    random_backoff;   
	uint8_t  status; 
	uint32_t  object_backoff;
	uint8_t    cca_result;
	uint32_t  csma_ca_send_timestamp;
	//uint32_t   start_backoff_time;
       //���ȴӷ��Ͷ�����ȡ�ô����͵�����
       temp=Parameter_list.parameter_tx_task[Parameter_list.top];
	Parameter_list.top=(Parameter_list.top+1)%MAX_TX_BUFFER_SIZE;
	if(temp==NULL)
	{
	     error("\nerror:there is no frame in the tx buffer\n");
	     return;
	}
	
	//�����ĳ�ʼ��
       NB=0;
	CW=2;
	if(mac_pib_data.macBattLifeExt==FALSE)
		BE=mac_pib_data.macMinBE;
	else
		BE=(mac_pib_data.macMinBE>2)? 2:mac_pib_data.macMinBE;
       phy_channel_state=DOINGCSMA_CA;
	//mac_current_state=MAC_STATE
	//��λ��backoff�ı߽�
      	status=wait_for_backoff_boundary();
       if(status==0)
	   	return;
                
	//��ǰ�Ѿ���unit boundary
       
       do{

	    random_backoff=random_generate();
	    random_backoff=(random_backoff<<BE)-random_backoff;
	    MAC_CURRENT_TICKS=systime32();
	    object_backoff=(uint32_t)(random_backoff*aUnitBackoffPeriod*2)+MAC_CURRENT_TICKS;
	    if(object_backoff>=0x40000000)
			object_backoff=object_backoff-0x40000000;
	    //������			
	    while(MAC_CURRENT_TICKS!=object_backoff)
			{
			     MAC_CURRENT_TICKS=systime32();
			     if((phy_channel_state!=DOINGCSMA_CA))
                              return;	
			     //�п����ж�ʹ��MAC_CURRENT_TICKS��object_backoff����Ƚ���ȵ�ʱ��.	 
			     //��Ҫ��Ӵ���
			}
	    //perform  CCA
	    do{
	         status=wait_for_backoff_boundary();
	         if(status==0)
			 	return;
	         cca_result=PLME_CCA_request();
		  if(cca_result!=IDLE)
		  	break;
	    }while((CW--));
	    //cama�ɹ�,����	
	    if(cca_result==IDLE)
	    	{
		   csma_ca_send_timestamp=PD_DATA_request(temp->frameLength, temp->payload);
		   
		   return;
	    	}  
	    else
	    	{
	    	   CW=2;
		   NB=NB+1;	   
	    	}
		
      	}
	while(NB<(mac_pib_data.macMaxCsmaBackoffs));
  
}
*/
void transmit_with_csmaca(void)
{
       csma_state=csma_state_initial;

	temp_csma_ca_frame=Parameter_list.parameter_tx_task[Parameter_list.top];
	Parameter_list.top=(Parameter_list.top+1)%MAX_TX_BUFFER_SIZE;
	if(temp_csma_ca_frame==NULL)
	{
	     error("\nerror:there is no frame in the tx buffer\n");
	     return;
	}   
	if(phy_channel_state==PHY_IDLE)
		{
		   PLME_SET_TRX_STATE_request(RX_ON);
		   //printf("\n^^^^^^^^^^\n"); 
                 CSMA_CA_DOING();
                 //printf("\n^^^^^^^^^^\n");
		}
	else
		failure_to_tx_with_csma();
}
