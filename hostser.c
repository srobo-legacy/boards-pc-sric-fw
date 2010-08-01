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
#include "crc16.h"
#include <io.h>

/* Linked in elsewhere */
extern const hostser_conf_t hostser_conf;

/*** Transmit buffer ***/
uint8_t hostser_txbuf[HOSTSER_BUF_SIZE];
uint8_t hostser_txlen = 0;

/* Offset of next byte to be transmitted from the tx buffer */
static uint8_t txbuf_pos = 0;

/**** Receive buffer ****/
uint8_t hostser_rxbuf[HOSTSER_BUF_SIZE];
/* Where the next byte needs to go */
static uint8_t rxbuf_pos = 0;

/* Events that the state machine responds to */
typedef enum {
	/* A frame has been received */
	EV_RX_FRAME_RECEIVED,
	/* The 'user' has finished with the received frame */
	EV_RX_DONE,
	/* A lock has been requested on the transmit buffer */
	EV_TX_LOCKREQ,
	/* A frame has been queued for transmission */
	EV_TX_QUEUED,
} hs_event_t;

/* State machine that manages the host interface */
static void fsm( hs_event_t flag );

/* Set crc in transmit buffer */
static void tx_set_crc( void );

static volatile enum {
	/* Nothing's happening */
	HS_IDLE,
	/* A frame has been received and is being processed by the user */
	HS_FRAME_RECEIVED,
	/* Waiting for an ACK back from the host */
	HS_TX_WAIT_ACK,
	/* The output buffer has been locked by the 'user' */
	HS_TX_LOCKED,
} hostser_state = HS_IDLE;

void hostser_init( void )
{
}

bool hostser_tx_cb( uint8_t *b )
{
	static bool escape_next = false;

	if( txbuf_pos == hostser_txlen )
		/* Transmission complete */
		return false;

	*b = hostser_txbuf[txbuf_pos];

	if( escape_next )
		*b ^= 0x20;
	/* Don't escape byte 0 (0x7E) */
	else if( txbuf_pos != 0 && (*b == 0x7E || *b == 0x7D ) ) {
		*b = 0x7D;
		escape_next = true;
		return true;
	}

	txbuf_pos++;
	return true;
}

void hostser_rx_cb( uint8_t b )
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
		fsm( EV_RX_FRAME_RECEIVED );
	} else
		/* CRC's invalid -- throw it away */
		rxbuf_pos = 0;
}

static void fsm( hs_event_t flag )
{
	switch( hostser_state )	{
	case HS_IDLE:
		/* Nothing's currently happening */

		if( flag == EV_RX_FRAME_RECEIVED ) {
			/* Transmit ACK to host */
			hostser_txbuf[0] = 0x7E;
			hostser_txbuf[SRIC_DEST] = sric_addr_set_ack(1);
			hostser_txbuf[SRIC_SRC] = SRIC_ADDRESS;
			hostser_txbuf[SRIC_LEN] = 0;
			tx_set_crc();

			hostser_txlen = SRIC_OVERHEAD;
			txbuf_pos = 0;
			hostser_conf.usart_tx_start( hostser_conf.usart_tx_start_n );
			hostser_state = HS_FRAME_RECEIVED;

		} else if ( flag == EV_TX_LOCKREQ )
			hostser_state = HS_TX_LOCKED;

		break;

	case HS_FRAME_RECEIVED:
		/* A frame has been received */
		/* Stay in this state until we can throw it away.  */

		if( flag == EV_RX_DONE )
			hostser_state = HS_IDLE;

		break;

	case HS_TX_LOCKED:
		/* The transmit buffer is being filled up by something else */
		/* Stay in this state until we can use the transmit buffer again */

		if( flag == EV_TX_QUEUED ) {
			txbuf_pos = 0;
			hostser_conf.usart_tx_start( hostser_conf.usart_tx_start_n );

			hostser_state = HS_TX_WAIT_ACK;
		}
		break;

	case HS_TX_WAIT_ACK:
		/* Waiting for an ACK back about the frame that we just transmitted */

		if( flag == EV_RX_FRAME_RECEIVED ) {
			/* If it's not an ACK, then we can only assume
			   that the host received our frame, and the ACK failed to 
			   reach us -- process the frame from the IDLE state */
			if( !sric_frame_is_ack(hostser_rxbuf) ) {
				hostser_state = HS_IDLE;
				fsm( EV_RX_FRAME_RECEIVED );
			} else
				hostser_state = HS_IDLE;
		}
		/* TODO: Timeout and retransmit */
		break;

	default:
		break;
	}
}

static void tx_set_crc( void )
{
	uint8_t len = hostser_txbuf[SRIC_LEN];
	uint16_t c = crc16( hostser_txbuf, SRIC_HEADER_SIZE + len );

	hostser_txbuf[ SRIC_DATA + len ] = c & 0xff;
	hostser_txbuf[ SRIC_DATA + len + 1 ] = (c >> 8) & 0xff;
}

bool hostser_rx_avail( void )
{
	return hostser_state == HS_FRAME_RECEIVED;
}

void hostser_rx( void )
{
	while( !hostser_rx_avail() );
	return;
}

void hostser_rx_done( void )
{
	fsm( EV_RX_DONE );
}

bool hostser_tx_busy( void )
{
	return hostser_state == HS_IDLE;
}

void hostser_tx( void )
{
	if( hostser_state != HS_TX_LOCKED
	    || hostser_tx_busy() ) {
		/* Panic :-O These things must never happen*/
		while(1);
	}

	tx_set_crc();
	hostser_txlen = SRIC_OVERHEAD + hostser_txbuf[ SRIC_LEN ];

	fsm( EV_TX_QUEUED );
}

void hostser_tx_lock( void )
{
	/* Repeatedly try to get the lock */
	while( hostser_state != HS_TX_LOCKED )
		fsm( EV_TX_LOCKREQ );
}
