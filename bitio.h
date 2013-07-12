/**
 *	bitio.h
 *	Bit I/O
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef _BITIO_H_
#define _BITIO_H_

#include <stdint.h>
#include "boolean.h"

unsigned int get_byte(char b, unsigned int pos);
void bit_to_pin(char c, unsigned int pos, volatile unsigned char *port, unsigned int pin);

#endif  // _BITIO_H_
