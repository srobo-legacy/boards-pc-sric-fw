#ifndef __SRIC_H
#define __SRIC_H
#include <stdbool.h>
#include <stdint.h>

#define MAX_PAYLOAD 64

#define SRIC_TXBUF_SIZE (6 + MAX_PAYLOAD)
extern uint8_t sric_txbuf[];

/* Offsets of fields in the tx buffer */
enum {
	SRIC_DEST = 1,
	SRIC_SRC = 2,
	SRIC_LEN = 3,
	SRIC_DATA = 4
	/* CRC is last two bytes */
};

/* Initialise the internal goo */
void sric_init( void );

/* Begin transmission of the frame in the tx buffer */
void sric_tx( void );

/* Returns true when it is safe to fiddle with the tx buffer */
bool sric_tx_isbusy( void );

/* Returns true when  */
bool sric_resp_avail( void );

/* Transmit the frame in the tx buffer,
   and return when the response has been received. */
void sric_txrx( void );

/* Returns true when a frame has been received */
bool sric_rx_avail( void );

/* Transmit a response frame */
void sric_tx_resp( void );

#endif	/* __SRIC_H */