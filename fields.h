#ifndef __FIELDS_H
#define __FIELDS_H
/* Macro for dealing with multibit fields */

/* Set the field in x to val.
   The field is defined by mask */
#define field_set( x, val, mask ) do { x = (x & ~mask) | val; } while(0)

#endif	/* __FIELDS_H */
