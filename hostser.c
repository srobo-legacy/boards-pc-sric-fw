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
#include "hostser.h"
#include "usart1.h"
#include "crc16.h"
#include <io.h>

extern uint8_t hostser_txbuf[HOSTSER_BUF_SIZE];

uint8_t hostser_rxbuf[HOSTSER_BUF_SIZE];
/* Where the next byte needs to go */
static uint8_t rxbuf_pos = 0;

/* Returns the next byte to be transmitted */
static bool hostser_tx_cb( uint8_t *b );

/* Data is handed to this from USART1 */
static void hostser_rx_cb( uint8_t b );

void hostser_init( void )
{
	usart1_tx_next_byte = hostser_tx_cb;
	usart1_rx = hostser_rx_cb;
}

static bool hostser_tx_cb( uint8_t *b )
{
	return false;
}

static void hostser_rx_cb( uint8_t b )
{
	static bool escape_next = false;
	uint8_t len;
	uint16_t crc, recv_crc;

	if( b == 0x7E ) {
		escape_next = false;
		rxbuf_pos = 0;
	} else if( b == 0x7D ) {
		escape_next = true;
		return;
	} else if( escape_next ) {
		escape_next = false;
		b ^= 0x20;
	}
			
	/* End of buffer :/ */
	if( rxbuf_pos == HOSTSER_BUF_SIZE )
		return;

	hostser_rxbuf[rxbuf_pos] = b;
	rxbuf_pos += 1;

	if( hostser_rxbuf[0] != 0x7e
	    /* Make sure we've reached the minimum frame size */
	    || rxbuf_pos < (SRIC_LEN + 2) )
		return;

	len = hostser_rxbuf[SRIC_LEN];
	if( len != rxbuf_pos - (SRIC_LEN + 3) )
		return;

	/* Everything gets hashed */
	crc = crc16( hostser_rxbuf, rxbuf_pos - 2 );

	recv_crc = hostser_rxbuf[ rxbuf_pos-2 ];
	recv_crc |= hostser_rxbuf[ rxbuf_pos-1 ] << 8;

	if( crc == recv_crc ) {
		/* We have a valid frame :-O */
		nop();
	} else
		/* CRC's invalid -- throw it away */
		rxbuf_pos = 0;
}
