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
#include <io.h>
#include <signal.h>
#include "leds.h"
#include "fields.h"
#include "usart1.h"

/* Kick the XT2 crystal until it starts oscillating */
void xt2_boot( void )
{
	/* Use red LED to diagnose oscillator fault */
	led_red(1);

	do {
		uint8_t i;

		/* Switch the XT2 osc on */
		BCSCTL1 &= ~XT2OFF;
		field_set( BCSCTL2, SELM_XT2CLK, SELM_3 );

		/* Clear flag to allow oscillator test */
		IFG1 &= ~OFIFG;

		/* Wait at least 50µs (currently on ~800kHz DCO) */
		for( i=0; i<40; i++ )
			nop();
	} while( IFG1 & OFIFG );

	led_red(0);
}

void init( void )
{
	/* Tristate all pins */
	P1DIR = P2DIR = P3DIR = P4DIR = P5DIR = P6DIR = 0;

	leds_init();

	/* Source MCLK from XT2 */
	xt2_boot();
	/* Source SMCLK from XT2 */
	BCSCTL2 |= SELS;

	usart1_init();
	eint();
}

int main( void )
{
	/* Disable the watchdog timer */
	WDTCTL = WDTHOLD | WDTPW;
	init();

	while(1)
	{
		nop();
	}
}
