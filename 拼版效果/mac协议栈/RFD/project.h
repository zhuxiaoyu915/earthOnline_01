

#ifndef rfj_H
#define rfj_H

#include "generic.h"


enum  {
  FALSE = 0, 
  TRUE = 1
};

enum  {
  FAIL = 0, 
  SUCCESS = 1
};

enum  {
  NULL = 0x0
};




enum  {
  LedsC_RED_BIT = 1, 
  LedsC_GREEN_BIT = 2, 
  LedsC_YELLOW_BIT = 4
};

enum  {
  Timer0_maxTimerInterval = 230
};

enum cc2420RadioM_nesc_unnamed4265 {
  cc2420RadioM_DISABLED_STATE = 0, 
  cc2420RadioM_IDLE_STATE, 
  cc2420RadioM_TX_STATE, 
  cc2420RadioM_PRE_TX_STATE, 
  cc2420RadioM_POST_TX_STATE, 
  cc2420RadioM_POST_TX_ACK_STATE, 
  cc2420RadioM_RX_STATE, 
  cc2420RadioM_POWER_DOWN_STATE, 

  cc2420RadioM_TIMER_INITIAL = 0, 
  cc2420RadioM_TIMER_BACKOFF, 
  cc2420RadioM_TIMER_ACK
};

//----------------------------------------------------
typedef struct _demo_addressfield
{
  uint16_t destination_pan;
  uint16_t destination_address;
  uint16_t source_pan;
  uint16_t source_address;
} demo_addressfield;

typedef struct _demo_datapacket
{
  uint8_t length;
  uint16_t fcf;
  uint8_t dsn;
  demo_addressfield address;
  uint8_t  payload;
 } demo_datapacket;

typedef struct _demo_beacon_addressfield
{
  uint16_t source_pan;
  uint16_t source_address;
} demo_beacon_addressfield;


typedef struct _demo_beaconpacket
{

  uint8_t length;
  uint16_t fcf;
  uint8_t bsn;
  demo_beacon_addressfield address;
  uint16_t Superframe_specification;
  uint8_t   GTS_field;
  uint8_t   Pending_addressfields;
  uint8_t   payload;
  
 } demo_beaconpacket;


//---------------------------------------------------
#endif





