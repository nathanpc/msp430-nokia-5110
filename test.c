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

	// A simple string print.
	lcd_print("Hello, world!");

	// Another example string.
	lcd_set_pos(0, 1);  // Going to the second row.
	lcd_print("Nathan Campos");

	// A simple bar.
	lcd_set_pos(0, 1);
	for (unsigned int i = 0; i < 84; i++) {
		lcd_command(0, 0b11111111);
		delay_ms(10);
	}

	// Clear just that row and write some text.
	lcd_clear_row(1);
	lcd_putc('@');  // If all you want is a single character.
	lcd_print("nathanpc");

	// And more text!
	lcd_set_pos(0, 3);
	lcd_print("http://about.me/nathanpc");

	while (TRUE) {
	}
}
