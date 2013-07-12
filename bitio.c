/**
 *	bitio.c
 *	Bit I/O
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdint.h>
#include "boolean.h"

/**
 * Get a single bit from a byte.
 * 
 * @param b A byte.
 * @param pos The bit position to be extracted.
 * @return A bit.
 */
unsigned int get_byte(char b, unsigned int pos) {
	return (b & (1 << pos));
}

/**
 * Puts the desired bit into a pin. It's used to get the bits in a char
 * to send to the LCD.
 * 
 * @param c The character.
 * @param pos Bit position.
 * @param pin The pin to be set.
 */
void bit_to_pin(char c, unsigned int pos, volatile unsigned char *port, unsigned int pin) {
	if (get_byte(c, pos)) {
		*port |= pin;
	} else {
		*port &= ~pin;
	}
}