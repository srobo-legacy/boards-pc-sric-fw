#ifndef __CMDS_H
#define __CMDS_H
#include <stdint.h>
#include "libsric/sric-client.h"

/* The command table */
extern const sric_cmd_t sric_commands[];

/* Number of commands */
extern const uint8_t sric_cmd_num;

#endif	/* __CMDS_H */
