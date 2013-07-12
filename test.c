#include <stdio.h>

#include <msp430g2553.h>
#include "PCD8544.h"

#include "boolean.h"
#include "delay.h"

/*#define LCD5110_SCLK_PIN BIT5
#define LCD5110_DN_PIN BIT7
#define LCD5110_SCE_PIN BIT0
#define LCD5110_DC_PIN BIT1
#define LCD5110_SELECT P1OUT &= ~LCD5110_SCE_PIN
#define LCD5110_DESELECT P1OUT |= LCD5110_SCE_PIN
#define LCD5110_SET_COMMAND P1OUT &= ~LCD5110_DC_PIN
#define LCD5110_SET_DATA P1OUT |= LCD5110_DC_PIN
#define LCD5110_COMMAND 0
#define LCD5110_DATA 1

#define SPI_MSB_FIRST UCB0CTL0 |= UCMSB // or UCA0CTL0 |= UCMSB (USCIA) or USICTL0 &= ~USILSB (USI)
#define SPI_LSB_FIRST UCB0CTL0 &= ~UCMSB // or UCA0CTL0 &= ~UCMSB or USICTL0 |= USILSB (USI)

void writeStringToLCD(const char *string);
void writeCharToLCD(char c);
void writeBlockToLCD(char *byte, unsigned char length);
void writeGraphicToLCD(char *byte, unsigned char transform);
void writeToLCD(unsigned char dataCommand, unsigned char data);
void clearLCD();
void clearBank(unsigned char bank);
void setAddr(unsigned char xAddr, unsigned char yAddr);
void initLCD();

unsigned char currXAddr = 0; //TODO this will be used for tracking current addr
unsigned char currYAddr = 0; //not implemented

//char testBlock[8] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
char testBlock[8] = {0x00, 0x7F, 0x7F, 0x33, 0x33, 0x03, 0x03, 0x03};
char testBlock2[8] = {0x00, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00};
*/

unsigned int value=0;
void ConfigureAdc(void)
{
	P1DIR &= ~BIT3;
	P1SEL |= BIT3;
  /* Configure ADC  Channel */
  ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/4
  ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  //Vcc & Vss as reference
  ADC10AE0 |= BIT3;                         //P1.3 ADC option    
}

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;         // disable WDT
    BCSCTL1 = CALBC1_1MHZ;            // 1MHz clock
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);                  // SMCLK = DCO = 1MHz  

    /*P1OUT |= LCD5110_SCE_PIN + LCD5110_DC_PIN;
    P1DIR |= LCD5110_SCE_PIN + LCD5110_DC_PIN;
    

    // setup USIB
    P1SEL |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;
    P1SEL2 |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;*/
    
    ///////ConfigureAdc();
	lcd_setup();

    /*UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;                     // SMCLK
    UCB0BR0 |= 0x01;                          // 1:1
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;                     // clear SW
    */
    //__enable_interrupt();

    //__delay_cycles(500000);
	delay_ms(1);
	lcd_init();
    //clearLCD();
	//__delay_cycles(1000000);
		lcd_command(0, 0b00011111);
    
    //writeStringToLCD("Nokia 5110");
    while(1) {
    	/*__delay_cycles(1000000);                   // Wait for ADC Ref to settle 
    	clearLCD();
    	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled    
    	value = ADC10MEM;
    	
    	char vstr[40];
    	sprintf(vstr, "%d", value);
    	writeStringToLCD(vstr);*/
    }

    // LCD test
    /*writeStringToLCD("Nokia 5110");

    __delay_cycles(2000000);
    setAddr(0, 0);

    int c = 0x20;
    while(c < (65 + 0x20)) {
        writeCharToLCD(c);
        c++;
    }

    __delay_cycles(2000000);
    clearLCD();

    c = 65 + 0x20;

    while(c < (96 + 0x20)) {
        writeCharToLCD(c);
        c++;
    }

    __delay_cycles(2000000);

    c = 0;
    clearBank(3);
    while(c < 64) {
        writeToLCD(LCD5110_DATA, 0xFF);
        c++;
        __delay_cycles(20000);
    }
    clearBank(4);
    while(c < 100) {
        writeToLCD(LCD5110_DATA, 0xFF);
        c++;
        __delay_cycles(20000);
    }
    clearBank(5);
    while(c < 184) {
        writeToLCD(LCD5110_DATA, 0xFF);
        c++;
        __delay_cycles(20000);
    }

    __delay_cycles(2000000);

    clearBank(3);
    writeGraphicToLCD(testBlock, NONE);
    writeGraphicToLCD(testBlock, FLIP_V);
    writeGraphicToLCD(testBlock, FLIP_H);
    writeGraphicToLCD(testBlock, ROTATE);
    writeGraphicToLCD(testBlock, FLIP_V | ROTATE);
    writeGraphicToLCD(testBlock, FLIP_H | ROTATE);
    writeGraphicToLCD(testBlock, ROTATE_90_CCW);
    writeGraphicToLCD(testBlock, ROTATE_180);

    clearBank(4);
    writeGraphicToLCD(testBlock2, NONE);
    writeGraphicToLCD(testBlock2, FLIP_H);
    writeGraphicToLCD(testBlock2, ROTATE);
    writeGraphicToLCD(testBlock2, ROTATE_90_CCW);



    clearBank(0);
    writeStringToLCD("For details,");
    clearBank(1);
    writeStringToLCD("visit 43oh.com");
    clearBank(2);*/
}







// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}
