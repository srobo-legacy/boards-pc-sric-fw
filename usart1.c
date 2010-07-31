/*   Copyright (C) 2010 Robert Spanton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */
#include "usart1.h"
#include <io.h>
#include <signal.h>
#include <sys/cdefs.h>
#include "leds.h"

/* Next byte callback */
bool (*usart1_tx_next_byte) ( uint8_t *b ) = NULL;

void usart1_init( void )
{
	/* Hold peripheral in reset during configuration  */
	U1CTL = SWRST;

	/* 8-bit data */
	U1CTL |= CHAR;
	/* U1CTL.PENA = 0 : Parity disabled */
	/* U1CTL.SPB = 0 : One stop bit */
	/* U1CTL.LISTEN = 0 : Loopback disabled */
	/* U1CTL.SYNC = 0: UART mode */
	/* U1CTL.MM = 0 : Idle-line multiprocessor mode */

	/* Use SMCLK */
	U1TCTL = SSEL_SMCLK;

	/* URXWIE = 0 : All received bytes trigger interrupt */
	U1RCTL = 0;

	/* Set baud to 115200 -- values from 
	   http://mspgcc.sourceforge.net/baudrate.html */
	U1BR0=0x45;
	U1BR1=0x00;
	U1MCTL=0xAA;

	/* Enable transmit and receive */
	ME2 |= UTXE1 | URXE1;

	/* These pins please */
	P3SEL |= (1<<6) | (1<<7);

	/* Take out of reset */
	U1CTL &= ~SWRST;

	/* Enable interrupts */
	IE2 |= UTXIE1 | URXIE1;
}

interrupt (USART1TX_VECTOR) usart1_isr_tx( void )
{
	uint8_t b;

	/* Ignore if the transmitter isn't enabled */
	if( !(ME2 & UTXE1) )
		return;

	if( usart1_tx_next_byte == NULL
	    || !usart1_tx_next_byte(&b) )
		/* Nothing to transmit -- disable the transmitter */
		return;

	U1TXBUF = b;
}

interrupt (USART1RX_VECTOR) usart1_isr_rx( void )
{
	nop();
}

void usart1_tx_start( void )
{
	if( ME2 & UTXE1 )
		/* Ignore if already enabled */
		return;

	/* Enable the transmitter */
	ME2 |= UTXE1;
	/* Trigger an interrupt :-P */
	IFG2 |= UTXIFG1;
}
