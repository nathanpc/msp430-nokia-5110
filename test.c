/**
 *  test.c
 *  Test drive.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdio.h>
#include <msp430g2553.h>
#include "PCD8544.h"

#include "boolean.h"
#include "delay.h"

void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Disable WDT.
	BCSCTL1 = CALBC1_1MHZ;     // 1MHz clock.
	DCOCTL = CALDCO_1MHZ;
	BCSCTL2 &= ~(DIVS_3);      // SMCLK = DCO = 1MHz.

	// Setup the LCD stuff.
	lcd_setup();
	delay_ms(1);  // Just to make sure the LCD is ready
	lcd_init();
	lcd_clear();

	lcd_print("Hello, world!");

	while (TRUE) {
	}
}
