#include <p18cxxx.h>
#include <timers.h>
#include <usart.h>
#include <stdio.h> 
#include "../../../../src/rx/Man_Decode.h"

char rxbuf[1 + (4*sizeof(long))];

void printrxbuf() {
	char i = sizeof(rxbuf);
	while(i--)
		_usart_putc(rxbuf[i]);
}

char calcperiods() {
	unsigned char time = TMR0L;
	TMR0L = 0;

	/* 8Mhz/4/32/1200(bp/s) = 52 */

	if (time < 52/2) return 0;
	if (time < 3*(52 / 2)) return 1;

	return 2;
}

void main(){
	OpenTimer0(TIMER_INT_OFF & T0_8BIT & T0_SOURCE_INT & T0_PS_1_32);
	OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF &
		USART_ASYNCH_MODE & USART_EIGHT_BIT &
		USART_CONT_RX & USART_BRGH_HIGH, 12  ); // 38400 b/s

	for(;;) {
		memset(rxbuf, 0, sizeof(rxbuf));
		for(;;) {
			char ds_B;

			if (rxbuf[1] == '\r' && rxbuf[0] == '\n') {
				printrxbuf();
				break;
			}

			if ( PORTBbits.RB0 ) {
    			if ( ds_B ) continue;
				ds_B = 1;
				Man_Decode_Stable_Zero(calcperiods());
			} else {
    			if ( !ds_B ) continue;
				ds_B = 0;
				Man_Decode_Stable_One(calcperiods());
			}
		}
	}
}

void shiftrxbuf() {
	*(( unsigned long* )&rxbuf[12]) <<= 1;
	if (rxbuf[11] & 0x80) rxbuf[12] |= 1;
	*(( unsigned long* )&rxbuf[8]) <<= 1;
	if (rxbuf[7] & 0x80) rxbuf[8] |= 1;
	*(( unsigned long* )&rxbuf[4]) <<= 1;
	if (rxbuf[3] & 0x80) rxbuf[4] |= 1;
	*(( unsigned long* )&rxbuf) <<= 1;
}

void On_Man_Decode_Add_0(){
	shiftrxbuf();
	rxbuf[0] &= 0xFE;
}

void On_Man_Decode_Add_1(){
	shiftrxbuf();
	rxbuf[0] |= 1;
}