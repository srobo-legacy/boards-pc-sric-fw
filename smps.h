#ifndef __SMPS_H
#define __SMPS_H
/* Functions for controlling the 12V boost converter */

void smps_init( void );

void smps_enable( void );
void smps_disable( void );

#endif	/* __SMPS_H */
