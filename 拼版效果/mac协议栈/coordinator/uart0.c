#include "uart0.h"

int usart_putchar(char c)
{
       
	if(c=='\n')
		usart_putchar('\r');
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0=c;
	//OS_uwait(16);*/
	return 0;
}
//-----------------------------------------------------------
int usart_putbyte(uint8_t  c)
{
	
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0=c;
	return 0;
}

//-----------------------------------------------------------
int usart_getchar(void)
{
	loop_until_bit_is_set(UCSR0A,RXC0);
	return UDR0;
}

void IoInit(void)
{
	//UART初始化
	UCSR0B=_BV(RXEN0)|_BV(TXEN0);/*(1<<RXCIE)|(1<<TXCIE)|*/
	UBRR0L=8;	//57600 baud 
	
	//i/o流UART连接
	fdevopen(usart_putchar,usart_getchar,0);	
}