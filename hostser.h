#ifndef __HOSTSER_H
#define __HOSTSER_H
/* Routines for managing interaction with the host device 
   (e.g. BeagleBoard or PC) over serial link. */
#include <stdbool.h>
#include <stdint.h>
#include "sric.h"

#define HOSTSER_BUF_SIZE SRIC_TXBUF_SIZE
/* Transmit buffer
   All bytes except the first are escaped as they leave. */
extern uint8_t hostser_txbuf[];
/* Number of bytes in the transmit buffer */
extern uint8_t hostser_txlen;

void hostser_init( void );

/* Request that the given frame is transmitted
   Sorts out CRC.
   Must be called when the interface is idle. */
void hostser_tx( void );

/* Returns true when the tx is busy */
bool hostser_tx_busy( void );

/* Returns true if there's a received frame */
bool hostser_rx_avail( void );

/* Blocks until a frame is received
   hostser_rx_done must be called as soon as the received data is no longer
   required. */
void hostser_rx( void );

/* Indicate that the received frame has been processed */
void hostser_rx_done( void );

#endif	/* __HOSTSER_H */
