/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire UART routines for polled serial IO
 *
 * Notes:
 */

#ifndef __UART_H__
#define __UART_H__

#include "derivative.h"

/********************************************************************/

void uart0_init (UART0_MemMapPtr uartch, int sysclk, int baud);
char uart0_getchar (UART0_MemMapPtr channel);
void uart0_putchar (UART0_MemMapPtr channel, char ch);
int uart0_getchar_present (UART0_MemMapPtr channel);

/********************************************************************/

#endif /* __UART_H__ */
