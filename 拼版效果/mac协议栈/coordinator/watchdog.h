#include  <avr/wdt.h>

//use the avr_libs
//such as below codes



/*
#define wdt_reset() __asm__ __volatile__ ("wdr")

#define _wdt_write(value)				\
	__asm__ __volatile__ (				\
		"in __tmp_reg__,__SREG__" "\n\t"	\
		"cli" "\n\t"				\
		"wdr" "\n\t"				\
		"out %0,%1" "\n\t"			\
		"out __SREG__,__tmp_reg__" "\n\t"	\
		"out %0,%2"				\
		: 		\
		: "I" (_SFR_IO_ADDR(WDTCR)),		\
		  "r" (0x18),	\
		  "r" ((unsigned char)(value))		\
		: "r0"					\
	)


#define wdt_enable(timeout) _wdt_write((timeout) | _BV(WDE))

#define wdt_disable() _wdt_write(0)


#define WDTO_15MS	0


#define WDTO_30MS	1

#define WDTO_60MS	2


#define WDTO_120MS	3

#define WDTO_250MS	4


#define WDTO_500MS	5


#define WDTO_1S		6


#define WDTO_2S		7

*/

