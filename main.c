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
#include <sys/cdefs.h>
#include "leds.h"
#include "fields.h"
#include "drivers/usart.h"
#include "drivers/xt2.h"
#include "libsric/hostser.h"
#include "libsric/gw.h"
#include "libsric/sric.h"
#include "smps.h"
#include "sric-mux.h"
#include "config.h"

const usart_t usart_config[2] = {
	{
		.tx_gen_byte = sric_tx_cb,
		.rx_byte = sric_rx_cb,

		/* 115200 baud -- values from
		   http://mspgcc.sourceforge.net/baudrate.html */
		.br0 = 0x45,
		.br1 = 0x00,
		.mctl = 0xAA,

		.sel_rx = &P3SEL,
		.sel_tx = &P3SEL,
		.sel_rx_num = 5,
		.sel_tx_num = 4,
	},

	{
		.tx_gen_byte = hostser_tx_cb,
		.rx_byte = hostser_rx_cb,

		/* 115200 baud -- values from
		   http://mspgcc.sourceforge.net/baudrate.html */
		.br0 = 0x45,
		.br1 = 0x00,
		.mctl = 0xAA,

		.sel_rx = &P3SEL,
		.sel_tx = &P3SEL,
		.sel_rx_num = 7,
		.sel_tx_num = 6,
	}
};

const hostser_conf_t hostser_conf = {
	.usart_tx_start = usart_tx_start,
	.usart_tx_start_n = 1,
	.rx_cb = gw_hostser_rx,
	.tx_done_cb = gw_hostser_tx_done,
};

const sric_conf_t sric_conf = {
	.usart_tx_start = usart_tx_start,
	.usart_rx_gate = usart_rx_gate,
	.usart_n = 0,
};

void init( void )
{
	/* Tristate all pins */
	P1DIR = P2DIR = P3DIR = P4DIR = P5DIR = P6DIR = 0;

	leds_init();

	/* Source MCLK from XT2 */
	/* Use the red LED to indicate oscillator fault */
	led_red(1);
	xt2_start();
	led_red(0);

	/* Source SMCLK from XT2 */
	BCSCTL2 |= SELS;

	if( SRIC_DIRECTOR )
		sric_mux_master();
	else
		sric_mux_pass();

	smps_init();
	usart_init();
	hostser_init();
	sric_init();
	eint();
}

int main( void )
{
	/* Disable the watchdog timer */
	WDTCTL = WDTHOLD | WDTPW;
	init();

	smps_enable();

	while(1)
	{
		nop();
	}
}
