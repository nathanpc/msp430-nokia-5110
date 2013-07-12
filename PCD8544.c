/**
 *  PCD8544.c
 *  PCD8544 (bit bang) driver for the MSP430G2553 based on RobG's code.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include "PCD8544.h"
#include <msp430g2553.h>

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "bitio.h"

// Pins.
#define SCLK BIT0
#define MOSI BIT1
#define D_C  BIT2
#define EN   BIT3

// Properties.
#define LCDWIDTH  84
#define LCDHEIGHT 48
#define LCDAREA   LCDWIDTH * LCDHEIGHT

/**
 *  Setup the pins for communication with the LCD driver.
 */
void lcd_setup() {
	P2DIR |= (SCLK + MOSI + D_C + EN);

	P2OUT &= ~(SCLK + MOSI + D_C);
	P2OUT |= EN;
}

/**
 *  Initializes the LCD with some defaults.
 */
void lcd_init() {
	lcd_command(PCD8544_FUNCTIONSET | PCD8544_EXTINSTRUCTIONS, 0);
	lcd_command(PCD8544_SETVOP | 0x3F, 0);
	lcd_command(PCD8544_SETTEMP | 0x02, 0);
	lcd_command(PCD8544_SETBIAS | 0x03, 0);
	lcd_command(PCD8544_FUNCTIONSET, 0);
	lcd_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, 0);
}

/**
 *  Send a command to the LCD controller. (Simulating SPI)
 *
 *  @param command A command to send.
 *  @param data Some data to be sent.
 */
void lcd_command(const char command, const char data) {
	// Combine both into one byte to be sent.
	char dbyte = (command | data);

	P2OUT &= ~SCLK;  // Put the clock line LOW to start.
	P2OUT &= ~EN;    // Pull the EN pin LOW to start sending a packet.

	// The packet must be most significant bit first, so we need to send the
	// data from bits 7 to 0.
	for (int i = 7; i >= 0; i--) {
		// Check if it's time to set the Data pin and if it'll be needed.
		if ((i == 0) & (command == 0)) {
			P2OUT |= D_C;
		}

		// Set MOSI according to bit.
		bit_to_pin(dbyte, i, &P2OUT, MOSI);

		// Send a clock pulse.
		P2OUT |= SCLK;   // Set clock HIGH to send the bit.
		P2OUT &= ~SCLK;  // Set the clock back to LOW and prepare for the next bit.
	}

	// Finish the packet and clean the mess.
	P2OUT |= EN;
	P2OUT &= ~(SCLK + MOSI + D_C);
}

/**
 *  Prints a character on the screen.
 *
 *  @param c A character.
 */
void lcd_putc(const char c) {
	// Print each of the 5 collumns of pixels in the font.
	for (unsigned int i = 0; i < 5; i++) {
		lcd_command(0, font[c - 0x20][i]);
	}

	// Send a blank collumn to separate from the next character.
	lcd_command(0, 0);
}

/**
 *  Prints a string on the screen.
 *
 *  @param string A string of characters.
 */
void lcd_print(const char *string) {
	while (*string) {
		lcd_putc(*string++);
	}
}

/**
 *  Clears the screen
 */
void lcd_clear() {
	// Start from (0,0).
	lcd_set_pos(0, 0);

	// Fill the whole screen with blank pixels.
	for (unsigned int i = 0; i < (LCDWIDTH * (LCDHEIGHT / 8)); i++) {
		lcd_command(0, 0);
	}

	// Go back to (0,0).
	lcd_set_pos(0, 0);
}

/**
 *  Clears a row of the display.
 *
 *  @param row A display row (Y address).
 */
void lcd_clear_row(unsigned int row) {
	// Start from the beginning of the row.
	lcd_set_pos(0, row);

	// Fill the row with blank pixels.
	for (unsigned int i = 0; i < LCDWIDTH; i++) {
		lcd_command(0, 0);
	}

	// Go back to where everything started.
	lcd_set_pos(0, row);
}

/**
 *  Sets the position of the memory cursor in the LCD controller.
 *
 *  @param x The X position (0 >= X <= 83).
 *  @param y The Y position (0 >= Y <= 5).
 */
void lcd_set_pos(unsigned int x, unsigned int y) {
	// Pretty straight forward huh?
	lcd_command(PCD8544_SETXADDR, x);
	lcd_command(PCD8544_SETYADDR, y);
}
