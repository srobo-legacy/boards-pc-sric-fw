#ifndef __GW_H
#define __GW_H
/* Host <-> SRIC 'gateway' */
#include "sric.h"
#include "hostser.h"

void gw_init( void );

/* Call when frame available on the host side */
void gw_hostser_rx( void );

/* Call when frame available on the SRIC side */
void gw_sric_rx( void );

#endif	/* __GW_H */
