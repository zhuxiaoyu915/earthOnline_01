#ifndef rfdev_H
#define rfdev_H

#include "top.h"

bool cc2420_bramspi;
uint8_t *cc2420_rambuf;
uint8_t cc2420_ramlen;
uint16_t cc2420_ramaddr;

bool cc2420fifo_bspi;

uint8_t *cc2420fifo_txbuf;
uint8_t   cc2420fifo_txlength;

uint8_t *cc2420_rxbuf[2];
uint8_t   cc2420_rxlength;
uint8_t  *cc2420_rxbuf_now;
uint8_t    cc2420_rxlength_now;
uint8_t    cc2420_rxbufuse;
uint8_t   rfpackernum;
uint8_t    cc2420_rxflag;

uint16_t  cc2420_currentparameters[14];
uint32_t receive_timestamp;
uint32_t send_timestamp;

//--------------------------------------------------------------------------------------
uint8_t cc2420_stateRadio;
volatile uint16_t cc2420_LocalAddr;
uint8_t ieee_address[8];
//----------------------------------------------------------------------------------------

inline  uint8_t cc2420_pin_init(void);
inline  uint8_t cc2420_reginit(void);
bool cc2420_setreg(void);
inline uint8_t cc2420_fifopintr(void);
inline  uint8_t cc2420_enablefifop(void);
uint8_t cc2420_sendcmd(uint8_t addr);
uint8_t cc2420_writereg(uint8_t addr, uint16_t data);
uint16_t cc2420_readreg(uint8_t addr);
uint8_t cc2420_readlargeram(uint16_t addr, uint8_t length, uint8_t *buffer);
uint8_t cc2420_writelargeram(uint16_t addr, uint8_t length, uint8_t *buffer);
uint8_t cc2420_writetxfifo(uint8_t len, uint8_t *msg);
void cc2420_readrxfifo(void);
inline  uint8_t cc2420_oscon(void);
inline uint8_t  cc2420_oscoff(void);
inline  uint8_t cc2420_VREFOn(void);
inline  uint8_t cc2420_VREFOff(void);
inline  uint8_t cc2420_enableAutoAck(void) ;
inline  uint8_t cc2420_disableAutoAck(void);
inline  uint8_t cc2420_enableAddrDecode(void);
inline  uint8_t cc2420_disableAddrDecode(void) ;
inline  uint8_t cc2420_setrxmode(void);
inline  uint8_t cc2420_settxmode(void);
inline  uint8_t cc2420_settxccamode(void);
inline  uint8_t cc2420_setrfoff(void);
inline  uint8_t cc2420_setchnl(int8_t chnl);
inline  uint8_t cc2420_setshortaddress(uint16_t addr);
inline uint8_t cc2420_setpanid(uint16_t id);
inline uint8_t cc2420_getpanid(uint16_t * id);
inline void cc2420_bcn_accept(void);
inline  uint8_t cc2420_getshortaddress(uint16_t * addr);
inline void      cc2420_get_ieee(uint8_t * buffer);
inline void      cc2420_set_ieee(uint8_t * buffer);
inline void      cc2420_set_cordinator(void);
inline void      cc2420_init(void);


#endif
