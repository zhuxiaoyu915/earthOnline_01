
#include "avrhardware.h"
#include "project.h"
inline void  OS_wait_250ns() {
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
}

inline void  OS_uwait(int u_sec) {
    while (u_sec > 0) {
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      u_sec--;
    }
}

// LED assignments
OS_ASSIGN_PIN(RED_LED, A, 2);
OS_ASSIGN_PIN(GREEN_LED, A, 1);
OS_ASSIGN_PIN(YELLOW_LED, A, 0);

OS_ASSIGN_PIN(SERIAL_ID, A, 4);
OS_ASSIGN_PIN(BAT_MON, A, 5);
OS_ASSIGN_PIN(THERM_PWR, A, 7);

// ChipCon control assignments

#define OS_CC_FIFOP_INT SIG_INTERRUPT6
OS_ASSIGN_PIN(CC_RSTN, A, 6); // chipcon reset
OS_ASSIGN_PIN(CC_VREN, A, 5); // chipcon power enable
//OS_ASSIGN_PIN(CC_FIFOP1, D, 7);  // fifo interrupt
OS_ASSIGN_PIN(CC_FIFOP1, E, 6);  // fifo interrupt

OS_ASSIGN_PIN(CC_CCA, D, 6);	  // 
OS_ASSIGN_PIN(CC_SFD, D, 4);	  // chipcon packet arrival
OS_ASSIGN_PIN(CC_CS, B, 0);	  // chipcon enable
OS_ASSIGN_PIN(CC_FIFO, B, 7);	  // chipcon fifo

OS_ASSIGN_PIN(RADIO_CCA, D, 6);	  // 

// Flash assignments
OS_ASSIGN_PIN(FLASH_SELECT, A, 3);
OS_ASSIGN_PIN(FLASH_CLK,  D, 5);
OS_ASSIGN_PIN(FLASH_OUT,  D, 3);
OS_ASSIGN_PIN(FLASH_IN,  D, 2);

// interrupt assignments
OS_ASSIGN_PIN(INT0, E, 4);
OS_ASSIGN_PIN(INT1, E, 5);
OS_ASSIGN_PIN(INT2, E, 6);
OS_ASSIGN_PIN(INT3, E, 7);

// spibus assignments 
OS_ASSIGN_PIN(MOSI,  B, 2);
OS_ASSIGN_PIN(MISO,  B, 3);
//OS_ASSIGN_PIN(SPI_OC1C, B, 7);
OS_ASSIGN_PIN(SPI_SCK,  B, 1);

// power control assignments
OS_ASSIGN_PIN(PW0, C, 0);
OS_ASSIGN_PIN(PW1, C, 1);
OS_ASSIGN_PIN(PW2, C, 2);
OS_ASSIGN_PIN(PW3, C, 3);
OS_ASSIGN_PIN(PW4, C, 4);
OS_ASSIGN_PIN(PW5, C, 5);
OS_ASSIGN_PIN(PW6, C, 6);
OS_ASSIGN_PIN(PW7, C, 7);

// i2c bus assignments
OS_ASSIGN_PIN(I2C_BUS1_SCL, D, 0);
OS_ASSIGN_PIN(I2C_BUS1_SDA, D, 1);

// uart assignments
OS_ASSIGN_PIN(UART_RXD0, E, 0);
OS_ASSIGN_PIN(UART_TXD0, E, 1);
OS_ASSIGN_PIN(UART_XCK0, E, 2);
	
OS_ASSIGN_PIN(UART_RXD1, D, 2);
OS_ASSIGN_PIN(UART_TXD1, D, 3);
OS_ASSIGN_PIN(UART_XCK1, D, 5);

void OS_SET_PIN_DIRECTIONS(void)
{
  /*  outp(0x00, DDRA);
  outp(0x00, DDRB);
  outp(0x00, DDRD);
  outp(0x02, DDRE);
  outp(0x02, PORTE);
  */

  OS_MAKE_RED_LED_OUTPUT();
  OS_MAKE_YELLOW_LED_OUTPUT();
  OS_MAKE_GREEN_LED_OUTPUT();

      
  OS_MAKE_PW7_OUTPUT();
  OS_MAKE_PW6_OUTPUT();
  OS_MAKE_PW5_OUTPUT();
  OS_MAKE_PW4_OUTPUT();
  OS_MAKE_PW3_OUTPUT(); 
  OS_MAKE_PW2_OUTPUT();
  OS_MAKE_PW1_OUTPUT();
  OS_MAKE_PW0_OUTPUT();

//cc2420 pins  
  OS_MAKE_MISO_INPUT();
  OS_MAKE_MOSI_OUTPUT();
  OS_MAKE_SPI_SCK_OUTPUT();
  OS_MAKE_CC_RSTN_OUTPUT();    
  OS_MAKE_CC_VREN_OUTPUT();
  OS_MAKE_CC_CS_INPUT(); 
  OS_MAKE_CC_FIFOP1_INPUT();    
  OS_MAKE_CC_CCA_INPUT();
  OS_MAKE_CC_SFD_INPUT();
  OS_MAKE_CC_FIFO_INPUT(); 

  OS_MAKE_RADIO_CCA_INPUT();



  OS_MAKE_SERIAL_ID_INPUT();
  OS_CLR_SERIAL_ID_PIN();  // Prevent sourcing current

  OS_MAKE_FLASH_SELECT_OUTPUT();
  OS_MAKE_FLASH_OUT_OUTPUT();
  OS_MAKE_FLASH_CLK_OUTPUT();
  OS_SET_FLASH_SELECT_PIN();
    
  OS_SET_RED_LED_PIN();
  OS_SET_YELLOW_LED_PIN();
  OS_SET_GREEN_LED_PIN();
}

//--------------------------------------------------------------

inline  int is_host_lsb(void)
{
  const uint8_t n[2] = { 1, 0 };

  return * (uint16_t *)n == 1;
}

 inline uint16_t toLSB16(uint16_t a)
{
  return is_host_lsb() ? a : (a << 8) | (a >> 8);
}


 inline uint16_t fromLSB16(uint16_t a)
{
  return is_host_lsb() ? a : (a << 8) | (a >> 8);
}

//---------------------------------------------------------------------------
 inline uint8_t rcombine(uint8_t  r1, uint8_t  r2)
{
  return r1 == FAIL ? FAIL : r2;
}

 inline  uint8_t rcombine4(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4)
{
  return rcombine(r1, rcombine(r2, rcombine(r3, r4)));
}
 