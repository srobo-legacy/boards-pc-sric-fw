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

uint8_t sric_txbuf[SRIC_TXBUF_SIZE];

struct {
	/* Next byte to be transmitted */
	uint8_t out_pos;
	
} tx;

void sric_init( void )
{

}

void sric_tx( void )
{

}

bool sric_tx_isbusy( void )
{
	return false;
}

bool sric_resp_avail( void )
{
	return false;
}

void sric_txrx( void )
{

}

bool sric_rx_avail( void )
{
	return false;
}

void sric_tx_resp( void )
{

}
