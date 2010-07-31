#ifndef __USART1_H
#define __USART1_H
#include <stdint.h>
#include <stdbool.h>

void usart1_init( void );

/* Callback to be called to retrieve the next byte for transmission.
   Must return false if there is no next byte (in which case transmission
   should be resumed using usart1_tx_start). */
extern bool (*usart1_tx_next_byte) ( uint8_t *b );

/* Trigger transmission
   Bytes to be transmitted will be picked up through the next byte callback */
void usart1_tx_start( void );

/* Callback to be presented with bytes as they're received */
extern void (*usart1_rx) ( uint8_t b );

#endif	/* __USART1_H */
