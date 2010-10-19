#ifndef __CONFIG_H
#define __CONFIG_H

/* Whether we're the bus director
   If 1, this causes the device to take address 1, generate and manage
   the token. */
#define SRIC_DIRECTOR 1

/* SRIC_PROMISC enables/disables promiscuous mode, in which the
   interface calls a function for every frame that's received */
#define SRIC_PROMISC 1

#define SCHED_TIMER_B 1
#define SCHED_QUEUE_SIZE 4
/* Divide SMCLK to give 1kHz timer */
#define SCHED_DIV 8000

/* Number of pin interrupt entries */
#define PININT_NCONF 4

#endif	/* __CONFIG_H */
