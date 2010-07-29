/*   Copyright (C) 2010 Robert Spanton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */
#ifndef __LEDS_H
#define __LEDS_H

#define LED0 (1<<3)
#define LED1 (1<<4)
#define LEDS_BOTH (LED0 | LED1)

#define leds_init() do { P4OUT &= ~LEDS_BOTH;	\
		P4DIR |= LEDS_BOTH; } while(0)

/* "Private" Macro -- use led0_set or led1_set instead */
#define _led_set(x,b) do { if(x) { P4OUT |= b; }	\
		else { P4OUT &= ~b; } } while(0)

#define led0_set(x) _led_set(x, LED0)
#define led1_set(x) _led_set(x, LED1)

#define led_green(x) led0_set(x)
#define led_red(x) led1_set(x)

#endif	/* __LEDS_H */
