#ifndef _AVRHARDWARE_H
#define _AVRHARDWARE_H

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
//#include "cc2420Const.h"

// check for version 3.3 of GNU gcc or later
#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3))
#define __outw(val, port) outw(port, val);
#endif

#ifndef __inw
#ifndef __SFR_OFFSET
#define __SFR_OFFSET 0
#endif /* !__SFR_OFFSET */
#define __inw(_port) inw(_port)

#define __inw_atomic(__sfrport) ({				\
	uint16_t __t;					\
	bool bStatus;					\
	bStatus = bit_is_set(SREG,7);			\
	cli();						\
	__t = inw(__sfrport);				\
	if (bStatus) sei();				\
	__t;						\
 })

#endif /* __inw */
// cc2420 Interrupt definition
#define cc2420_FIFOP_INT_ENABLE()  sbi(EIMSK , INT6)
#define cc2420_FIFOP_INT_DISABLE() cbi(EIMSK , INT6)
//---------------------
#define OS_ASSIGN_PIN_list(name, port, bit) \
    void OS_SET_##name##_PIN();  \
    void OS_CLR_##name##_PIN();  \
    int    OS_READ_##name##_PIN();\
    void OS_MAKE_##name##_OUTPUT();  \
    void OS_MAKE_##name##_INPUT() ;
//--------------------------------------------------
#define OS_ASSIGN_PIN(name, port, bit) \
    void OS_SET_##name##_PIN() {PORT##port |= (1<<(bit));} \
    void OS_CLR_##name##_PIN() {PORT##port &= (~(1<<(bit)));} \
    int OS_READ_##name##_PIN()							\
  {return (PIN##port & (1 << bit)) != 0;}								\
    void OS_MAKE_##name##_OUTPUT() {DDR##port |= (1<<(bit));} \
    void OS_MAKE_##name##_INPUT() {DDR##port &= (~(1<<(bit)));} 


/* Watchdog Prescaler
 */
enum {
  OS_period16 = 0x00, // 47ms
  OS_period32 = 0x01, // 94ms
  OS_period64 = 0x02, // 0.19s
  OS_period128 = 0x03, // 0.38s
  OS_period256 = 0x04, // 0.75s
  OS_period512 = 0x05, // 1.5s
  OS_period1024 = 0x06, // 3.0s
  OS_period2048 = 0x07 // 6.0s
};

//--------------------------------------------------------------------------
// LED assignments
OS_ASSIGN_PIN_list(RED_LED, A, 2);
OS_ASSIGN_PIN_list(GREEN_LED, A, 1);
OS_ASSIGN_PIN_list(YELLOW_LED, A, 0);

OS_ASSIGN_PIN_list(SERIAL_ID, A, 4);
OS_ASSIGN_PIN_list(BAT_MON, A, 5);
OS_ASSIGN_PIN_list(THERM_PWR, A, 7);

// ChipCon control assignments

#define OS_CC_FIFOP_INT SIG_INTERRUPT6
OS_ASSIGN_PIN_list(CC_RSTN, A, 6); // chipcon reset
OS_ASSIGN_PIN_list(CC_VREN, A, 5); // chipcon power enable
//OS_ASSIGN_PIN_list(CC_FIFOP1, D, 7);  // fifo interrupt
OS_ASSIGN_PIN_list(CC_FIFOP1, E, 6);  // fifo interrupt

OS_ASSIGN_PIN_list(CC_CCA, D, 6);	  // 
OS_ASSIGN_PIN_list(CC_SFD, D, 4);	  // chipcon packet arrival
OS_ASSIGN_PIN_list(CC_CS, B, 0);	  // chipcon enable
OS_ASSIGN_PIN_list(CC_FIFO, B, 7);	  // chipcon fifo

OS_ASSIGN_PIN_list(RADIO_CCA, D, 6);	  // 

// Flash assignments
OS_ASSIGN_PIN_list(FLASH_SELECT, A, 3);
OS_ASSIGN_PIN_list(FLASH_CLK,  D, 5);
OS_ASSIGN_PIN_list(FLASH_OUT,  D, 3);
OS_ASSIGN_PIN_list(FLASH_IN,  D, 2);

// interrupt assignments
OS_ASSIGN_PIN_list(INT0, E, 4);
OS_ASSIGN_PIN_list(INT1, E, 5);
OS_ASSIGN_PIN_list(INT2, E, 6);
OS_ASSIGN_PIN_list(INT3, E, 7);

// spibus assignments 
OS_ASSIGN_PIN_list(MOSI,  B, 2);
OS_ASSIGN_PIN_list(MISO,  B, 3);
//OS_ASSIGN_PIN_list(SPI_OC1C, B, 7);
OS_ASSIGN_PIN_list(SPI_SCK,  B, 1);

// power control assignments
OS_ASSIGN_PIN_list(PW0, C, 0);
OS_ASSIGN_PIN_list(PW1, C, 1);
OS_ASSIGN_PIN_list(PW2, C, 2);
OS_ASSIGN_PIN_list(PW3, C, 3);
OS_ASSIGN_PIN_list(PW4, C, 4);
OS_ASSIGN_PIN_list(PW5, C, 5);
OS_ASSIGN_PIN_list(PW6, C, 6);
OS_ASSIGN_PIN_list(PW7, C, 7);

// i2c bus assignments
OS_ASSIGN_PIN_list(I2C_BUS1_SCL, D, 0);
OS_ASSIGN_PIN_list(I2C_BUS1_SDA, D, 1);

// uart assignments
OS_ASSIGN_PIN_list(UART_RXD0, E, 0);
OS_ASSIGN_PIN_list(UART_TXD0, E, 1);
OS_ASSIGN_PIN_list(UART_XCK0, E, 2);
	
OS_ASSIGN_PIN_list(UART_RXD1, D, 2);
OS_ASSIGN_PIN_list(UART_TXD1, D, 3);
OS_ASSIGN_PIN_list(UART_XCK1, D, 5);

inline void  OS_wait_250ns();
inline void  OS_uwait(int u_sec);
inline  int is_host_lsb(void);
inline uint16_t toLSB16(uint16_t a);
inline uint16_t fromLSB16(uint16_t a);
inline uint8_t  rcombine(uint8_t  r1, uint8_t  r2);
inline  uint8_t rcombine4(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4);

void OS_SET_PIN_DIRECTIONS(void);
//---------------------------------------------------------------------------


#define OS_CYCLE_TIME_NS 136

// each nop is 1 clock cycle
// 1 clock cycle on mica2 == 136ns



enum {
  OS_ADC_PORTMAPSIZE = 12
};

enum 
{
//  OS_ACTUAL_CC_RSSI_PORT = 0,
//  OS_ACTUAL_VOLTAGE_PORT = 7,
  OS_ACTUAL_BANDGAP_PORT = 30,  // 1.23 Fixed bandgap reference
  OS_ACTUAL_GND_PORT     = 31   // GND 
};

enum 
{
 // TOS_ADC_CC_RSSI_PORT = 0,
 // TOS_ADC_VOLTAGE_PORT = 7,
  TOS_ADC_BANDGAP_PORT = 10,
  TOS_ADC_GND_PORT     = 11
};

#endif 
