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
#include "libsric/sric-gw.h"
#include "libsric/sric.h"
#include "libsric/sric-client.h"
#include "smps.h"
#include "sric-mux.h"
#include "config.h"
#include "drivers/sched.h"
#include "drivers/pinint.h"

#if SRIC_DIRECTOR
#include "libsric/token-dir.h"
#else
#include "libsric/token-msp.h"
#endif

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

#if SRIC_DIRECTOR
	/* Send received frames to the gateway */
	.rx_cb = sric_gw_hostser_rx,
	/* Notify the gateway when transmission is complete */
	.tx_done_cb = sric_gw_hostser_tx_done,
#else
	.rx_cb = NULL,
	.tx_done_cb = NULL,
#endif
};

const sric_conf_t sric_conf = {
	.usart_tx_start = usart_tx_start,
	.usart_rx_gate = usart_rx_gate,
	.usart_n = 0,

	.txen_dir = &P3DIR,
	.txen_port = &P3OUT,
	.txen_mask = (1<<0),

#if SRIC_DIRECTOR
	/* Send received frames to the gateway */
	.rx_cmd = sric_gw_sric_rxcmd,
	.rx_resp = sric_gw_sric_rxresp,
	.error = sric_gw_sric_err,
	.token_drv = &token_dir_drv,
#else
	/* We're a simple client */
	.rx_cmd = sric_client_rx,
	.rx_resp = NULL,
	.error = NULL,
	.token_drv = &token_msp_drv,
#endif
};

#if SRIC_DIRECTOR
const token_dir_conf_t token_dir_conf = {
	.haz_token = sric_haz_token,

	.to_port = &P3OUT,
	.to_dir = &P3DIR,
	.to_mask = (1<<1),

	.ti_port = &P1IN,
	.ti_dir = &P1DIR,
	.ti_mask = (1<<0),
};
#else
const token_msp_conf_t token_msp_conf = {
	.haz_token = sric_haz_token,

	.to_port = &P3OUT,
	.to_dir = &P3DIR,
	.to_mask = (1<<1),

	.ti_port = &P1IN,
	.ti_dir = &P1DIR,
	.ti_mask = (1<<0),
};
#endif

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

	pinint_init();
	smps_init();
	sched_init();
	usart_init();

#if SRIC_DIRECTOR
		sric_mux_master();
		token_dir_init();
		hostser_init();
#else
		token_msp_init();
		sric_mux_pass();
#endif

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
