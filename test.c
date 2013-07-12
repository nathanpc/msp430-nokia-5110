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

#define ADC BIT3

void adc_setup();
unsigned int value = 0;

void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Disable WDT.
	BCSCTL1 = CALBC1_1MHZ;     // 1MHz clock.
	DCOCTL = CALDCO_1MHZ;
	BCSCTL2 &= ~(DIVS_3);      // SMCLK = DCO = 1MHz.
    
    //adc_setup();
	lcd_setup();

    //__delay_cycles(500000);
	delay_ms(1);
	lcd_init();
    //clearLCD();
	//__delay_cycles(1000000);
	//lcd_putc('P');
	//lcd_command(0, 0b11111111);
	lcd_clear();
	lcd_command(0, 0b10101010);

	while (TRUE) {
/*
		if (value < 85) {
			lcd_command(0, 0b11100111);
		} else {
			delay_ms(3000);
		}
		value++;*/

    	/*__delay_cycles(1000000);                   // Wait for ADC Ref to settle 
    	clearLCD();
    	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    	//__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled    
    	value = ADC10MEM;
    	
    	char vstr[40];
    	sprintf(vstr, "%d", value);
    	writeStringToLCD(vstr);*/
	}
}

void adc_setup() {
	// Enable the pin to be used as a ADC.
	P1DIR &= ~ADC;
	P1SEL |= ADC;

	// Configure ADC  Channel
	ADC10CTL1 = INCH_3 + ADC10DIV_3;  // Channel 3 (P1.3), ADC10CLK / 4.
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference.
	ADC10AE0 |= ADC;  // Enable the ADC.
}

// ADC10 interrupt service routine
/*#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}
*/
