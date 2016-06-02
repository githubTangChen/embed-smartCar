/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:       TERMINAL_PORT defined in <board>.h
 */

#include "common.h"
#include "uart.h"

/********************************************************************/

uint_8 *fp;
char in_char (void)
{
  return uart_getchar ();
}
/********************************************************************/
void out_char (char ch)
{
  uart_putchar(ch);
}

/********************************************************************/
/********************************************************************/
