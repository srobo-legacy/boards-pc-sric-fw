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
#include "sric.h"
#include <io.h>

uint8_t sric_txbuf[SRIC_TXBUF_SIZE];
uint8_t sric_rxbuf[SRIC_RXBUF_SIZE];

struct {
	/* Next byte to be transmitted */
	uint8_t out_pos;
	
} tx;

#define SRIC_TXEN (1<<0)
#define lvds_tx_en() do { P3OUT |= SRIC_TXEN; } while(0)
#define lvds_tx_dis() do { P3OUT &= ~SRIC_TXEN; } while(0)

void sric_init( void )
{
	lvds_tx_dis();
	P3DIR |= SRIC_TXEN;
}

bool sric_tx_cb( uint8_t *b )
{
	return false;
}

void sric_rx_cb( uint8_t b )
{

}

void sric_tx( void )
{

}

void sric_tx_resp( void )
{

}

void sric_rx_done( void )
{

}
