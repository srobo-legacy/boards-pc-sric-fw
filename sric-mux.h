#ifndef __SRIC_MUX_H
#define __SRIC_MUX_H

/* Set the analogue switches so that we're mastering the bus */
void sric_mux_master(void);

/* Set the analogue switches so that we're passing the bus through */
void sric_mux_pass(void);

#endif	/* __SRIC_MUX_H */
