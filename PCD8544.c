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

/*
void writeStringToLCD(const char *string) {
    while(*string) {
        writeCharToLCD(*string++);
    }
}

void writeCharToLCD(char c) {
    unsigned char i;
    for(i = 0; i < 5; i++) {
        writeToLCD(LCD5110_DATA, font[c - 0x20][i]);
    }
    writeToLCD(LCD5110_DATA, 0);
}

void writeBlockToLCD(char *byte, unsigned char length) {
    unsigned char c = 0;
    while(c < length) {
        writeToLCD(LCD5110_DATA, *byte++);
        c++;
    }
}

void writeGraphicToLCD(char *byte, unsigned char transform) {
    int c = 0;
    char block[8];

    if(transform & FLIP_V) {
        SPI_LSB_FIRST;
    }
    if(transform & ROTATE) {
        c = 1;
        while(c != 0) {
            (*byte & 0x01) ? (block[7] |= c) : (block[7] &= ~c);
            (*byte & 0x02) ? (block[6] |= c) : (block[6] &= ~c);
            (*byte & 0x04) ? (block[5] |= c) : (block[5] &= ~c);
            (*byte & 0x08) ? (block[4] |= c) : (block[4] &= ~c);
            (*byte & 0x10) ? (block[3] |= c) : (block[3] &= ~c);
            (*byte & 0x20) ? (block[2] |= c) : (block[2] &= ~c);
            (*byte & 0x40) ? (block[1] |= c) : (block[1] &= ~c);
            (*byte & 0x80) ? (block[0] |= c) : (block[0] &= ~c);
            *byte++;
            c <<= 1;
        }
    } else {
        while(c < 8) {
            block[c++] = *byte++;
        }
    }

    if(transform & FLIP_H) {
        c = 7;
        while(c > -1) {
            writeToLCD(LCD5110_DATA, block[c--]);
        }
    } else {
        c = 0;
        while(c < 8) {
            writeToLCD(LCD5110_DATA, block[c++]);
        }
    }
    SPI_MSB_FIRST;
}

void writeToLCD(unsigned char dataCommand, unsigned char data) {
    LCD5110_SELECT;
    if(dataCommand) {
        LCD5110_SET_DATA;
    } else {
        LCD5110_SET_COMMAND;
    }
    UCB0TXBUF = data;
    while(!(IFG2 & UCB0TXIFG))
        ;
    LCD5110_DESELECT;
}

void clearLCD() {
    setAddr(0, 0);
    int c = 0;
    while(c < PCD8544_MAXBYTES) {
        writeToLCD(LCD5110_DATA, 0);
        c++;
    }
    setAddr(0, 0);
}

void clearBank(unsigned char bank) {
    setAddr(0, bank);
    int c = 0;
    while(c < PCD8544_HPIXELS) {
        writeToLCD(LCD5110_DATA, 0);
        c++;
    }
    setAddr(0, bank);
}

void setAddr(unsigned char xAddr, unsigned char yAddr) {
    writeToLCD(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void initLCD() {
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLCD(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	}*/
