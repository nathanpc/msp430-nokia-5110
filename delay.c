/**
 *	delay.h
 *	The usual delay stuff for the MSP430G2553.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include "delay.h"

/**
 * Delay by some milliseconds.
 * 
 * @param ms number of milliseconds to delay.
 */
void delay_ms(unsigned int ms) {
	while (ms--) {
		__delay_cycles(1000);
	}
}

/**
 * Delay by some microseconds.
 * 
 * @param ms number of microseconds to delay.
 */
void delay_us(unsigned int us) {
	while (us--) {
		__delay_cycles(1);
	}
}
