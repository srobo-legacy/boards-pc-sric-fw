#ifndef __CONFIG_H
#define __CONFIG_H

/* Whether we're the bus director
   If 1, this causes the device to take address 1, generate and manage
   the token. */
#define SRIC_DIRECTOR 1

/* Dummy scheduler config to appease the build
 (the scheduler isn't yet used by this project) */
#define SCHED_QUEUE_SIZE 1
#define SCHED_DIV 0

#endif	/* __CONFIG_H */
