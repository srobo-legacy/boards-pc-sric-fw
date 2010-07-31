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

void hostser_tx( void );

/* Returns true when the tx is busy */
bool hostser_tx_busy( void );

bool hostser_rx_avail( void );

/* Blocks until  */
void hostser_rx( void );

#endif	/* __HOSTSER_H */
