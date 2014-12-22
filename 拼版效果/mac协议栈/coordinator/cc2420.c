#include "cc2420.h"

//------------------------------HPL-------------------------------------------------------------------
inline  uint8_t cc2420_pin_init(void)
{

  cc2420_bramspi = TRUE;
  OS_MAKE_MISO_INPUT();
  OS_MAKE_MOSI_OUTPUT();
  OS_MAKE_SPI_SCK_OUTPUT();
  OS_MAKE_CC_RSTN_OUTPUT();
  OS_MAKE_CC_VREN_OUTPUT();
  OS_MAKE_CC_CS_OUTPUT();
  OS_MAKE_CC_FIFOP1_INPUT();
  OS_MAKE_CC_CCA_INPUT();
  OS_MAKE_CC_SFD_INPUT();
  OS_MAKE_CC_FIFO_INPUT();
  
    return SUCCESS;
}

inline static  uint8_t SPI_init(void)
{
   { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_MAKE_SPI_SCK_OUTPUT();
      OS_MAKE_MISO_INPUT();
      OS_MAKE_MOSI_OUTPUT();
      SPSR |= 1 << 0;//SPI STATE 
      SPCR |= 1 << 4;
      SPCR &= ~(1 << 3);
      SPCR &= ~(1 << 2);
      SPCR &= ~(1 << 1);
      SPCR &= ~(1 << 0);
      SPCR |= 1 << 6;
    }
    os_atomic_end(_nesc_atomic); }
	return SUCCESS;
}

inline  uint8_t cc2420_reginit(void)
{
  
  // Set default parameters
  cc2420_currentparameters[CP_MAIN] = 0xf800;
  cc2420_currentparameters[CP_MDMCTRL0] = ((0 << cc2420_MDMCTRL0_PANCRD) |(0 << cc2420_MDMCTRL0_ADRDECODE) | //is not coordinator disable auto adr_decode
       (2 << cc2420_MDMCTRL0_CCAHIST) | (3 << cc2420_MDMCTRL0_CCAMODE)  | //cca mode is 3
       (1 << cc2420_MDMCTRL0_AUTOCRC) | (2 << cc2420_MDMCTRL0_PREAMBL)); //auto crc is set 

  cc2420_currentparameters[CP_MDMCTRL1] = 20 << cc2420_MDMCTRL1_CORRTHRESH;//default
  cc2420_currentparameters[CP_RSSI] =     0xE080;//default 
  cc2420_currentparameters[CP_SYNCWORD] = 0xA70F;
  cc2420_currentparameters[CP_TXCTRL] = ((1 << cc2420_TXCTRL_BUFCUR) | //default is 2-1.16ma,now is 980ua
       (1 << cc2420_TXCTRL_TURNARND) | (3 << cc2420_TXCTRL_PACUR) | //default ,turnaround time is 12 symbols
       (1 << cc2420_TXCTRL_PADIFF) | (0x1f << cc2420_TXCTRL_PAPWR));//default
 cc2420_currentparameters[CP_RXCTRL0] = ((1 << cc2420_RXCTRL0_BUFCUR) | //default is 980ua
       (2 << cc2420_RXCTRL0_MLNAG) | (3 << cc2420_RXCTRL0_LOLNAG) | 
       (2 << cc2420_RXCTRL0_HICUR) | (1 << cc2420_RXCTRL0_MCUR) | 
       (1 << cc2420_RXCTRL0_LOCUR));//default
 cc2420_currentparameters[CP_RXCTRL1]  = ((1 << cc2420_RXCTRL1_LOLOGAIN)   | (1 << cc2420_RXCTRL1_HIHGM    ) |//default
	                                   (1 << cc2420_RXCTRL1_LNACAP)     | (1 << cc2420_RXCTRL1_RMIXT    ) |
									   (1 << cc2420_RXCTRL1_RMIXV)      | (2 << cc2420_RXCTRL1_RMIXCUR  ) );//default
							    
 cc2420_currentparameters[CP_FSCTRL]   = ((1 << cc2420_FSCTRL_LOCK)        | ((357+5*(cc2420_DEF_CHANNEL-11)) << cc2420_FSCTRL_FREQ));//2405mhz
 cc2420_currentparameters[CP_SECCTRL0] = ((0 << cc2420_SECCTRL0_PROTECT) |(1 << cc2420_SECCTRL0_CBCHEAD)  |
	                                   (1 << cc2420_SECCTRL0_SAKEYSEL)  | (1 << cc2420_SECCTRL0_TXKEYSEL) |
									   (1 << cc2420_SECCTRL0_SECM ) );//disable the security
 cc2420_currentparameters[CP_SECCTRL1] = 0;
 cc2420_currentparameters[CP_BATTMON]  = 0;//battery monitor is disable 
    //cc2420_currentparameters[CP_IOCFG0]   = (((TOSH_DATA_LENGTH + 2) << cc2420_IOCFG0_FIFOTHR) | (1 <<cc2420_IOCFG0_FIFOPPOL)) ;
	//set fifop threshold to greater than size of  msg, fifop goes active at end of msg
 cc2420_currentparameters[CP_IOCFG0]   = (((127) << cc2420_IOCFG0_FIFOTHR) | (1 <<cc2420_IOCFG0_FIFOPPOL)) ;//polarity is inverted as compareed to the specification
 cc2420_currentparameters[CP_IOCFG1]   =  0;
  return SUCCESS;

}

//---------------------------------------------------------设置cc2420寄存器的值
bool cc2420_setreg(void)

{
  uint16_t data;
  
  cc2420_writereg(cc2420_MAIN, cc2420_currentparameters[CP_MAIN]);
  cc2420_writereg(cc2420_MDMCTRL0, cc2420_currentparameters[CP_MDMCTRL0]);
  data = cc2420_readreg(cc2420_MDMCTRL0);
  if (data != cc2420_currentparameters[CP_MDMCTRL0]) {
  //printf("reginit is unsuccess\n");
    return FALSE;
    }
  cc2420_writereg(cc2420_MDMCTRL1, cc2420_currentparameters[CP_MDMCTRL1]);
  cc2420_writereg(cc2420_RSSI, cc2420_currentparameters[CP_RSSI]);
  cc2420_writereg(cc2420_SYNCWORD, cc2420_currentparameters[CP_SYNCWORD]);
  cc2420_writereg(cc2420_TXCTRL, cc2420_currentparameters[CP_TXCTRL]);
  cc2420_writereg(cc2420_RXCTRL0, cc2420_currentparameters[CP_RXCTRL0]);
  cc2420_writereg(cc2420_RXCTRL1, cc2420_currentparameters[CP_RXCTRL1]);
  cc2420_writereg(cc2420_FSCTRL, cc2420_currentparameters[CP_FSCTRL]);

  cc2420_writereg(cc2420_SECCTRL0, cc2420_currentparameters[CP_SECCTRL0]);
  cc2420_writereg(cc2420_SECCTRL1, cc2420_currentparameters[CP_SECCTRL1]);
  cc2420_writereg(cc2420_IOCFG0, cc2420_currentparameters[CP_IOCFG0]);
  cc2420_writereg(cc2420_IOCFG1, cc2420_currentparameters[CP_IOCFG1]);

  cc2420_sendcmd(cc2420_SFLUSHTX);
  cc2420_sendcmd(cc2420_SFLUSHRX);
 //printf("reginit is success\n");
  return TRUE;
}

//----------------------------------------------------------------FIFOP中断处理中转
inline uint8_t cc2420_fifopintr(void){
    
 
   if (!OS_READ_CC_FIFO_PIN()) {
      //printf("收到一个包,但芯片缓存 溢出\n");
      cc2420_readreg(0x3F);
      rfpackernum=0;
      cc2420_sendcmd(0x08);
      cc2420_sendcmd(0x08);
      return SUCCESS;
    }
    rfpackernum++;
    receive_timestamp=get_timestamp32();
    //printf("===succeed received a physical packet====\n");
    if (cc2420_rxbufuse>1) 
		{
		    //printf("\nno available ram buffer\n");
		      return SUCCESS;
		
    	       }
    else 
		{
              cc2420_rxbufuse++;
		cc2420_readrxfifo();
    	      }
    return SUCCESS;
 }
//---------------------------------------------------------打开对fifop的中断支持,下降延触发
inline  uint8_t cc2420_enablefifop(void)
{
  EICRB &= ~(1 << 4);
  EICRB |= 1 << 5;
  EIMSK |= 1 << 6;
  return SUCCESS;
}

//---------------------------------------------------------向cc2420发送一个命令
uint8_t cc2420_sendcmd(uint8_t addr)
{
   uint8_t status;

  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
      OS_CLR_CC_CS_PIN();
      SPDR = addr;//这是单片机内部spi数据寄存器的地址
      while (!(SPSR & 0x80)) {//这是单片机内部spi状态寄存器的地址
        }
      ;
      status = SPDR;
    }
    os_atomic_end(_nesc_atomic); }
    OS_SET_CC_CS_PIN();
  return status;
}
//---------------------------------------------------------向cc2420指定的地址写数据
uint8_t cc2420_writereg(uint8_t addr, uint16_t data)
{
  uint8_t status;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      cc2420_bramspi = FALSE;
      OS_CLR_CC_CS_PIN();
      SPDR = addr;
      while (!(SPSR & 0x80)) {
        }
      ;
      status = SPDR;
      if (addr > 0x0E) {
          SPDR = data >> 8;
          while (!(SPSR & 0x80)) {
            }
          ;
          SPDR = data & 0xff;
          while (!(SPSR & 0x80)) {
            }
          ;
        }
      cc2420_bramspi = TRUE;
    }
    os_atomic_end(_nesc_atomic); }
  OS_SET_CC_CS_PIN();
  return status;
}

//---------------------------------------------------------------从cc2420指定的地址读数据
uint16_t cc2420_readreg(uint8_t addr)
{
  uint16_t data = 0;
  uint8_t status;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      cc2420_bramspi = FALSE;
      OS_CLR_CC_CS_PIN();
      SPDR = addr | 0x40;
      while (!(SPSR & 0x80)) {
        }
      ;
      status = SPDR;
      SPDR = 0;
      while (!(SPSR & 0x80)) {
        }
      ;
      data = SPDR;
      SPDR = 0;
      while (!(SPSR & 0x80)) {
        }
      ;
      data = (data << 8) | SPDR;
      OS_SET_CC_CS_PIN();
      cc2420_bramspi = TRUE;
    }
    os_atomic_end(_nesc_atomic); }
  return data;
}

uint8_t cc2420_readlargeram(uint16_t addr, uint8_t length, uint8_t *buffer)
{
   uint8_t i = 0;
   uint8_t status;
  if (!cc2420_bramspi) {
    return FALSE;
    }

  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
      cc2420_bramspi = FALSE;
      //cc2420_ramaddr = addr;
     // cc2420_ramlen = length;
      
      OS_CLR_CC_CS_PIN();
		  
      SPDR = (addr & 0x7F) | 0x80;
      while (!(SPSR & 0x80)) {
        }
      ;
      status = SPDR;
      //printf("status1 is %x\n",status);
       
       SPDR = (addr >> 1) & 0xe0;
       while (!(SPSR & 0x80)) ;
	status=SPDR;
      	SPDR=0;
	for (i = 0; i < length; i++) {
		 	 
                      while (!(SPSR & 0x80)) {
					  	
                       }
		        buffer[i]=SPDR;
			 SPDR=0;
		 	//printf("%x\n",buffer[i]);
        }
    }
  OS_SET_CC_CS_PIN();
  os_atomic_end(_nesc_atomic); }
  cc2420_bramspi = TRUE;
  return SUCCESS;
}

//---------------------------------------------------------连续写cc2420ram
uint8_t cc2420_writelargeram(uint16_t addr, uint8_t length, uint8_t *buffer)
{
  uint8_t i = 0;
  uint8_t status;

  if (!cc2420_bramspi) {
    return FALSE;
    }
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
      cc2420_bramspi = FALSE;
      //cc2420_ramaddr = addr;
      //cc2420_ramlen = length;
      //cc2420_rambuf = buffer;
      OS_CLR_CC_CS_PIN();
      SPDR = (addr & 0x7F) | 0x80;
      while (!(SPSR & 0x80)) {
        }
      ;
      status = SPDR;
      
      SPDR = (addr >> 1) & 0xC0;
      while (!(SPSR & 0x80)) {
        }
      ;
      status = SPDR;
      //for (i = 0; i < length; i++) 
      	//printf("\nwrite is %02x\n", buffer[i]);
      for (i = 0; i < length; i++) {
          SPDR = buffer[i];
          while (!(SPSR & 0x80)) {
            }
          ;
        }
    }
    OS_SET_CC_CS_PIN();
    os_atomic_end(_nesc_atomic); }
  cc2420_bramspi = TRUE;
  return SUCCESS;
}

//-------------------------------------------------------------------------
uint8_t cc2420_writetxfifo(uint8_t len, uint8_t *msg)//modified by zhouqiang in 2005.11.30
{
  uint8_t i = 0;
  uint8_t status;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      cc2420fifo_bspi = FALSE;
      cc2420fifo_txlength = len;
      cc2420fifo_txbuf = msg;
      OS_CLR_CC_CS_PIN();
      SPDR = 0x3E;
      while (!(SPSR & 0x80)) {
        }
      ;
       status = SPDR;
	//added by zhouqiang in 2005.11.30//write length field to the rxfifo
       SPDR=cc2420fifo_txlength+2;
	while (!(SPSR & 0x80)) { };
       status = SPDR;
	//----------------------------//
      for (i = 0; i < cc2420fifo_txlength; i++) {
          SPDR = *cc2420fifo_txbuf;
          cc2420fifo_txbuf++;
          while (!(SPSR & 0x80)) {
            }
          ;
        }
      cc2420fifo_bspi = TRUE;
    }
    os_atomic_end(_nesc_atomic); }
  OS_SET_CC_CS_PIN();
  
  //cc2420_sendcmd(cc2420_STXON); //do it in other process_2005.11.30
 
  return status;
}
//-----------------------------------------------------------------------
void  cc2420_readrxfifo()
{
  uint8_t status;
  uint8_t i;
 //printf("开始一个读fifo任务\n");
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      cc2420fifo_bspi = FALSE;
      OS_CLR_CC_CS_PIN();
	  
      SPDR = 0x3F | 0x40;//read rxfifo
      while (!(SPSR & 0x80))     ;
      status = SPDR;
      SPDR = 0;
      while (!(SPSR & 0x80))      ;
	  
      cc2420_rxlength = SPDR;
	  
      //printf("开始分配空间给rxbuf,rxflag is %x\n",cc2420_rxflag);
	  
    
      cc2420_rxbuf[cc2420_rxflag] =malloc(cc2420_rxlength +1);
     // printf("point is %04x\n",cc2420_rxbuf[cc2420_rxflag] );
   
	  
      if (cc2420_rxlength > 0) {
          cc2420_rxbuf[cc2420_rxflag] [0] = cc2420_rxlength ;

          cc2420_rxlength ++;

          
          for (i = 1; i < cc2420_rxlength ; i++) {
              SPDR = 0;
              while (!(SPSR & 0x80)) {
                }
              ;
              cc2420_rxbuf[cc2420_rxflag] [i] = SPDR;
            }
        }
     
      cc2420fifo_bspi = TRUE;
     
      rfpackernum--;
      cc2420_rxflag++;  
      cc2420_rxflag&=0x01;
      
    }
    os_atomic_end(_nesc_atomic); }
  OS_SET_CC_CS_PIN();
  phy_handlepacket();//modied by zhouqiang in 2005.11.30 
}

//-------------------------------------------------------------------------
inline  uint8_t cc2420_oscon(void)

{
  uint8_t i;
  uint8_t status;
  bool bXoscOn = FALSE;
  i = 0;
  cc2420_sendcmd(cc2420_SXOSCON);
  while (i < 200 && bXoscOn == FALSE) {
      OS_uwait(100);
      status = cc2420_sendcmd(cc2420_SNOP);
      status = status & (1 << 6);
      if (status) {

        bXoscOn = TRUE;
        }

      i++;
    }
  if (!bXoscOn) {
    //printf("osc is unsuccess now\n");
    return FAIL;
    }
//printf("osc is success now\n");
  return SUCCESS;
}

inline uint8_t cc2420_oscoff(void) {
    cc2420_sendcmd(cc2420_SXOSCOFF);   //turn-off crystal
    return SUCCESS;
  }

//--------------------------------------------------------
inline  uint8_t cc2420_VREFOn(void){
    OS_SET_CC_VREN_PIN();                    //turn-on  
    OS_uwait(1800);    
  //printf("vref is on now\n");
	return SUCCESS;
  }
inline  uint8_t cc2420_VREFOff(void){
    OS_CLR_CC_VREN_PIN();                    //turn-off  
    OS_uwait(1800);  
	return SUCCESS;
  }
//------------------------------------------------------------------
inline  uint8_t cc2420_enableAutoAck(void) {
    cc2420_currentparameters[CP_MDMCTRL0] |= (1 << cc2420_MDMCTRL0_AUTOACK);
    return cc2420_writereg(cc2420_MDMCTRL0,cc2420_currentparameters[CP_MDMCTRL0]);
  }

inline  uint8_t cc2420_disableAutoAck(void) {
    cc2420_currentparameters[CP_MDMCTRL0] &= ~(1 << cc2420_MDMCTRL0_AUTOACK);
    return cc2420_writereg(cc2420_MDMCTRL0,cc2420_currentparameters[CP_MDMCTRL0]);
  }

inline  uint8_t cc2420_enableAddrDecode(void) {
    cc2420_currentparameters[CP_MDMCTRL0] |= (1 << cc2420_MDMCTRL0_ADRDECODE);
    return cc2420_writereg(cc2420_MDMCTRL0,cc2420_currentparameters[CP_MDMCTRL0]);
  }

inline  uint8_t cc2420_disableAddrDecode(void) {
    cc2420_currentparameters[CP_MDMCTRL0] &= ~(1 << cc2420_MDMCTRL0_ADRDECODE);
    return cc2420_writereg(cc2420_MDMCTRL0,cc2420_currentparameters[CP_MDMCTRL0]);
  }

//---------------------------------------------------------命令cc2420进入rx模式

inline   uint8_t cc2420_setrxmode(void)

{
  cc2420_sendcmd(cc2420_SRXON);
  return SUCCESS;
}

inline   uint8_t cc2420_settxmode(void)

{
  cc2420_sendcmd(cc2420_STXON);
  return SUCCESS;
}

inline   uint8_t cc2420_settxccamode(void)

{
  cc2420_sendcmd(cc2420_STXONCCA);
  return SUCCESS;
}

inline   uint8_t cc2420_setrfoff(void)

{
  cc2420_sendcmd(cc2420_SRFOFF);
  return SUCCESS;
}


//---------------------------------------------------------设置cc2420 的频道
 inline uint8_t cc2420_setchnl(int8_t chnl)

{
  int fsctrl;

  fsctrl = 357 + 5 * (chnl - 11);
  cc2420_currentparameters[CP_FSCTRL] = (cc2420_currentparameters[CP_FSCTRL] & 0xfc00) | (fsctrl << 0);
  //printf("\n$$$$$channel state is %04x\n",cc2420_currentparameters[CP_FSCTRL] );
  cc2420_writereg(cc2420_FSCTRL, cc2420_currentparameters[CP_FSCTRL]);
  return SUCCESS;
}

//---------------------------------------------------------设置cc2420短地址，同时写入cc2420
inline uint8_t cc2420_setshortaddress(uint16_t addr)

{
   addr = toLSB16(addr);
   cc2420_writelargeram(cc2420_RAM_SHORTADR, 2, (uint8_t *)&addr);
 //printf("shortaddress is set\n");
   return SUCCESS;
}

inline uint8_t cc2420_getshortaddress(uint16_t * addr)

{
     cc2420_readlargeram(cc2420_RAM_SHORTADR, 2, (uint8_t *)addr);
   //printf("shortaddress is get\n");
	 return SUCCESS;
}

 inline uint8_t cc2420_setpanid(uint16_t id)
 {
 	id = toLSB16(id);
       cc2420_writelargeram(cc2420_RAM_PANID, 2, (uint8_t *)&id);
     //printf("PANID is set\n");
       return SUCCESS;
 }

inline uint8_t cc2420_getpanid(uint16_t * id)

{
     cc2420_readlargeram(cc2420_RAM_PANID, 2, (uint8_t *)&id);
   //printf("PANID is get\n");
     return SUCCESS;
}

inline void cc2420_bcn_accept(void)
{
	
  cc2420_currentparameters[CP_IOCFG0] = cc2420_currentparameters[CP_IOCFG0]  | (1 << 11);
  cc2420_writereg(cc2420_IOCFG0, cc2420_currentparameters[CP_IOCFG0]);
  
}
//------------------------------------------------------------------------------
inline void  cc2420_get_ieee(uint8_t * buffer)
{
   
  cc2420_readlargeram(cc2420_RAM_IEEEADR,8,buffer);
   
}

 
inline void  cc2420_set_ieee(uint8_t * buffer)
{
   
  cc2420_writelargeram(cc2420_RAM_IEEEADR,8,buffer);
   
}

inline void cc2420_set_cordinator(void)
{
	cc2420_currentparameters[CP_MDMCTRL0] = cc2420_currentparameters[CP_MDMCTRL0]  | (1 << 12);
       cc2420_writereg(cc2420_MDMCTRL0, cc2420_currentparameters[CP_MDMCTRL0]);
}

inline void cc2420_init(void)
{
	cc2420_rxbuf_now=0;//modified by zhouqiang in 2005.12.1  晚
       cc2420_rxlength_now=0;
	cc2420_rxflag=0;
	cc2420_rxbufuse=0;
	rfpackernum=0;
	receive_timestamp=0;
	send_timestamp=0;
	cc2420_pin_init();
	SPI_init();
	cc2420_VREFOn();
	OS_CLR_CC_RSTN_PIN();
	OS_uwait(20);  
	OS_uwait(20);  
	OS_SET_CC_RSTN_PIN();
	cc2420_oscon();
	cc2420_reginit();
	cc2420_setreg();
	cc2420_sendcmd(cc2420_SFLUSHRX);
	cc2420_sendcmd(cc2420_SFLUSHRX);
	//cc2420_sendcmd(cc2420_SFLUSHTX);
       if(IS_COORDINATOR)
	   	cc2420_set_cordinator();
     //printf("cc2420 init is success\n");
}





