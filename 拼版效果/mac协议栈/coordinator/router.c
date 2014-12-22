#include "project.h"
#include "router.h"

extern  SHORT_ADDR  default_short_addr;
extern  MAC_PIB  mac_pib_data;

ASSOCIATE_TABLE_ELEM   associate_tab[MAX_ASSOCIATE_TABLE_NUM];

uint16_t   short_address_initial;
uint16_t   address_allocated_num;


void  associate_tab_init(void)
{
    for(uint8_t index=0;index<MAX_ASSOCIATE_TABLE_NUM;index++)
    	{
		associate_tab[index].is_valid=FALSE;
		associate_tab[index].ieee_addr.longaddr=0x0000000000000000;
    	}	
}
void  router_init(void)
{
     associate_tab_init();
     short_address_initial=0x1201;	 
     address_allocated_num=0;	 
}
SHORT_ADDR short_address_allocate_req(LONG_ADDR  ieee_addr)
{
     
     
      //地址分配，记录关联信息
      SHORT_ADDR  allocated_addr;
	//allocated_addr=get_a_short_address();
      
      

	
	//return allocated_addr;
     
      uint8_t index;

      //allocated_addr.Val=0x0044;
      //	  return allocated_addr;
      index=Is_associated(ieee_addr);	  	  
      if(index!=MAX_ASSOCIATE_TABLE_NUM)
	  	return associate_tab[index].allocated_addr;

      index=0;	  
      while((associate_tab[index].is_valid)&&(index<MAX_ASSOCIATE_TABLE_NUM))
	  	index++;
	  
      if(index==MAX_ASSOCIATE_TABLE_NUM)
	  	return default_short_addr;
      associate_tab[index].ieee_addr=ieee_addr;
      associate_tab[index].is_valid=TRUE;
      associate_tab[index].allocated_addr=get_a_short_address();
      return 	associate_tab[index].allocated_addr;  
    
}


uint8_t Is_associated(LONG_ADDR long_address)
{
      uint8_t index;
	  index=0;
      while((!associate_tab[index].is_valid)||(associate_tab[index].ieee_addr.longaddr!=long_address.longaddr))
      {
          index++;
	   if(index==MAX_ASSOCIATE_TABLE_NUM)
	   	return MAX_ASSOCIATE_TABLE_NUM;
      	}
      return index;
}

SHORT_ADDR  get_a_short_address(void)
{
       //从地址库中得到一个短地址
       SHORT_ADDR  allocated_addr;
       allocated_addr.Val=(short_address_initial+address_allocated_num);
	address_allocated_num++;
	return allocated_addr;
}


bool   disassociate_req(LONG_ADDR ieee_addr)
{
      //地址删除，删除关联信息
      uint8_t index;
      index=0;
      while((associate_tab[index].ieee_addr.longaddr!=ieee_addr.longaddr)||(!associate_tab[index].is_valid))
      	{
      	   index++;
	   if(index==MAX_ASSOCIATE_TABLE_NUM)
	   	return  FALSE;
      	}
      if((associate_tab[index].ieee_addr.longaddr==ieee_addr.longaddr)&&(associate_tab[index].is_valid))
      	     associate_tab[index].is_valid=FALSE;
	  	return TRUE;
      		
}

void sensor_data_tx_to_pc(MCPS_DATA_INDICATION mcps_data_indication_info)
{
    SHORT_ADDR  src_short_addr;  
    SHORT_ADDR  dst_short_addr;
    if(mcps_data_indication_info.src.addrMode==0x03)	
    	{
        src_short_addr=short_address_allocate_req(mcps_data_indication_info.src.longAddr);
   //printf("\n--src short addr is %02x--\n",src_short_addr.v[0]);
   //printf("\n--src short addr is %02x--\n",src_short_addr.v[1]);
    	}
    else if(mcps_data_indication_info.src.addrMode==0x02)
	 src_short_addr=mcps_data_indication_info.src.shortAddr;
    dst_short_addr=mac_pib_data.macCoordShortAddress;

    //传向后台
    usart_putbyte(0x7e);
    usart_putbyte(0x42);

    usart_putbyte(0x1f);
    usart_putbyte(0x02);
    usart_putbyte(dst_short_addr.v[1]);
    usart_putbyte(src_short_addr.v[0]);

    usart_putbyte(0x01);

    usart_putbyte(0x00);

    for(uint8_t index=0;index<10;index++)
    	{
    	    usart_putbyte(mcps_data_indication_info.pMsdu[2*index]);
	    usart_putbyte(mcps_data_indication_info.pMsdu[2*index+1]);
    	}

    usart_putbyte(0x01);//crc1
    usart_putbyte(0x01);//crc0
	
    usart_putbyte(0x7e);
  //printf("\n---finished tx to snamp---\n");	
    return;
   
}