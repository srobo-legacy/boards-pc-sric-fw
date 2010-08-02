#ifndef __SRIC_H
#define __SRIC_H
#include <stdbool.h>
#include <stdint.h>

#define MAX_PAYLOAD 64

#define SRIC_TXBUF_SIZE (6 + MAX_PAYLOAD)
#define SRIC_RXBUF_SIZE SRIC_TXBUF_SIZE
extern uint8_t sric_txbuf[];
extern uint8_t sric_rxbuf[];

/* Offsets of fields in the tx buffer */
enum {
	SRIC_DEST = 1,
	SRIC_SRC = 2,
	SRIC_LEN = 3,
	SRIC_DATA = 4
	/* CRC is last two bytes */
};

/* The number of bytes in a SRIC header */
#define SRIC_HEADER_SIZE 4

/* The number of bytes in the header and footer of a SRIC frame */
#define SRIC_OVERHEAD (SRIC_HEADER_SIZE + 2)

/* Our SRIC address */
#define SRIC_ADDRESS 1

#define sric_addr_set_ack(x) (x | 0x80)
#define sric_addr_is_ack(x) ( x & 0x80 )
#define sric_frame_is_ack(buf) ( sric_addr_is_ack(buf[SRIC_DEST]) )

/* Initialise the internal goo */
void sric_init( void );

/* Transmit byte generator */
bool sric_tx_cb( uint8_t *b );
/* Callback for each byte received */
void sric_rx_cb( uint8_t b );

/* Begin transmission of the frame in the tx buffer */
void sric_tx( void );

/* Transmit a response frame */
void sric_tx_resp( void );

/* Indicate that the received frame has been processed */
void sric_rx_done( void );

#endif	/* __SRIC_H */
