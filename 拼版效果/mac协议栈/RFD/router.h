#ifndef _ROUTER_H_
#define _ROUTER_H_




#include "mac.h"
#include "generic.h"


typedef struct _ASSOCIATE_TABLE_ELEM
{
     LONG_ADDR  ieee_addr;
     SHORT_ADDR  allocated_addr;
     bool                is_valid;
} ASSOCIATE_TABLE_ELEM;

#define  MAX_ASSOCIATE_TABLE_NUM      10


void  associate_tab_init(void);



void  router_init(void);

uint8_t  Is_associated(LONG_ADDR long_address);

SHORT_ADDR short_address_allocate_req(LONG_ADDR  ieee_addr);

bool   disassociate_req(LONG_ADDR ieee_addr);

SHORT_ADDR  get_a_short_address(void);

void sensor_data_tx_to_pc(MCPS_DATA_INDICATION);

#endif