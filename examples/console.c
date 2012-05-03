/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
	Simple console input/output, over serial port #0

	Partially copied from Jim Lynch's tutorial
*/

#include "../lpc214x.h"
#include "console.h"

#define USE_UART1

#define PINSEL0		*(volatile unsigned int *)0xE002C000

#define U0THR		*(volatile unsigned int *)0xE000C000
#define U0RBR		*(volatile unsigned int *)0xE000C000
#define U0DLL		*(volatile unsigned int *)0xE000C000
#define U0DLM		*(volatile unsigned int *)0xE000C004
#define U0FCR		*(volatile unsigned int *)0xE000C008
#define U0LCR		*(volatile unsigned int *)0xE000C00C
#define U0LSR		*(volatile unsigned int *)0xE000C014

/* Initialize Serial Interface       */
void ConsoleInit(int iDivider)  
{          
#ifndef USE_UART1
	PINSEL0 = (PINSEL0 & ~0x0000000F) | 0x00000005;	/* Enable RxD0 and TxD0              */
	U0LCR = 0x83;                          			/* 8 bits, no Parity, 1 Stop bit     */
	U0DLL = iDivider & 0xFF;						/* set divider / baud rate */
	U0DLM = iDivider >> 8;
	U0LCR = 0x03;                          			/* DLAB = 0                          */
	
	// enable FIFO
	U0FCR = 1;
#else
	// Enable RXD1 and TXD1 by masking out current P08 and P09 MUX and or'ing
	// in RXD1 and TXD1
	PCB_PINSEL0 = ((PCB_PINSEL0 & ~(PCB_PINSEL0_P08_MASK | PCB_PINSEL0_P09_MASK)) | (PCB_PINSEL0_P08_TXD1 | PCB_PINSEL0_P09_RXD1));
	// Enable programming of the divisor by setting DLAB bit
	UART1_LCR |= UART_LCR_DLAB;
	// Program the divisor
	UART1_DLL = iDivider & 0xFF;
	UART1_DLM = iDivider >> 8;

	// Turn on FIFOs and clear buffers
	UART1_FCR = UART_FCR_EN | UART_FCR_CLR;
	
	// Set transmission format to 8N1		
	UART1_LCR = UART_LCR_NOPAR | UART_LCR_1STOP | UART_LCR_8BITS;
#endif // USE_UART1
}

/* Write character to Serial Port    */
int putchar(int ch)  
{
#ifndef USE_UART1	
	if (ch == '\n') {
		while (!(U0LSR & 0x20));
		U0THR = '\r';
	}
	while (!(U0LSR & 0x20));
	U0THR = ch;
#else
	if (ch == '\n') {
		while (!(UART1_LSR & 0x20));
		UART1_THR = '\r';
	}
	while (!(UART1_LSR & 0x20));
	UART1_THR = ch;
#endif // USE_UART1
	
	return ch;
}


int getchar (void)  {                    /* Read character from Serial Port   */

#ifndef USE_UART1
  while (!(U0LSR & 0x01));
  return (U0RBR);
#else
	while (!(UART1_LSR & 0x01));
	return (UART1_RBR);
#endif // USE_UART1
}


int puts(const char *s)
{
	while (*s) {
		putchar(*s++);
	}
	putchar('\n');
	return 1;
}


