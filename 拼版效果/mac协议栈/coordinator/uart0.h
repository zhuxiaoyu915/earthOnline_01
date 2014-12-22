#ifndef _uart0_h
#define _uart0_h
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
int usart_putchar(char c);
int usart_getchar(void);
//---------------------
int usart_putbyte(uint8_t  c);
//---------------------
void IoInit(void);
#endif