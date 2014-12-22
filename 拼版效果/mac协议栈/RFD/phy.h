#ifndef PHY_H
#define PHY_H


#include "generic.h"
#include "CC2420Const.h"
#include "cc2420.h"
#include "mac.h"

#include "systime.h"

//-----------------------global state variable--------------------------------------------


uint8_t rxbuf_flag;

//-----------------------PHY_constant--------------------------------------------------
//
#define aTurnaroundTime 12
#define aUnitBackoffPeriod     5
#define aMaxPHYPacketSize 127

//-----------------------PHY_PIB------------------------------------------------------
typedef struct _PHY_PIB {
  uint8_t phyCurrentChanel;
  //uint32_t phyChannelsSupported;
  uint8_t phyTransmitPower;
  uint8_t phyCCAMode;
} phy_pib;

enum{
	phyCurrentChanel_id=0,
	//phyChannelsSupported,_id,
	phyTransmitPower_id,
	phyCCAMode_id
	};

phy_pib  ppib;
//-----------------------some confirm  type----------------------------------------------------
typedef struct _ED_STATE{
  uint8_t state;
  int8_t energylevel;
}ED_STATE;

typedef struct  _GET_STATE{
 uint8_t state;
 uint8_t pibattribute;
 uint8_t pibattributevalue;
} GET_STATE;

typedef struct  _SET_STATE{
 uint8_t state;
 uint8_t pibattribute;
 } SET_STATE;

//----------------------PHY_enumeration_description--------------------------------------
enum phy_description {
	BUSY=0,
	BUSY_RX,
	BUSY_TX,
	FORCE_TRX_OFF,
	IDLE,
       PHY_INVALID_PARAMETER,
	RX_ON,
	PHY_SUCCESS,
	TRX_OFF,
	TX_ON,
	PHY_UNSUPPORTED_ATTRIBUTE
};


//----------------------function defined by uplayer-----------------------------------------
//extern uint8_t  PD_data_confirm(uint8_t state);
extern uint8_t  PD_DATA_indication(uint8_t psdulength,uint8_t * psdu,uint8_t ppdulinkquality,uint32_t  timestamp);
//extern uint8_t  PLME_CCA_confirm(uint8_t state);
//extern uint8_t  PLME_ED_confirm(uint8_t state,uint8_t energylevel);
//extern uint8_t  PLME_GET_confirm(uint8_t state,uint8_t pibattribute,uint8_t pibattributevalue);
//extern uint8_t  PLME_SET_confirm(uint8_t state,uint8_t pibattribute);
//extern uint8_t  PLME_SET_TRX_STATE_confirm(uint8_t state);
extern  uint32_t MAC_OBJECT_CAP_START_TICKS;
extern  uint32_t   MAC_OBJECT_CAP_END_TICKS;
extern  Timer_Queue_pointer head_of_timer_queue;
//---------------------function supported to  uplayer-----------------------------------------

void  phy_init(void);//
uint32_t PD_DATA_request(uint8_t psdulength,uint8_t * psdu);
uint8_t PLME_CCA_request(void);//
ED_STATE PLME_ED_request(void);//
GET_STATE PLME_GET_request(uint8_t pibattribute);//
SET_STATE PLME_SET_request(uint8_t pibattribute,uint8_t pibattributevalue);//
uint8_t PLME_SET_TRX_STATE_request(uint8_t state);//
//--------------------function pre-process receive packet-------------------------------------
void phy_handlepacket(void);
void  ack_send_handle_no_pending(void);
void ack_send_handle_pending(void);
#endif 

