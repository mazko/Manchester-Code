#include <p18cxxx.h>
#include <timers.h>
#include "../../../../src/tx/Man_Encode.h"

void main(){
	unsigned short long i;

	PORTBbits.RB0 = 0;
	TRISB = 0;
	OpenTimer0(TIMER_INT_ON & T0_8BIT & T0_SOURCE_INT & T0_PS_1_8);

	for(;;) {

		/* 'n' => 01101110b, MSB = 0 see tests => Man_Decode.cpp */

		On_Man_Encode_One();  // Clay balance (1 - 1 = 0)
		On_Man_Encode_Zero(); // Sync balance (2 - 2 = 0)
		On_Man_Encode_Zero(); 
		On_Man_Encode_One();  // Sync
		On_Man_Encode_One();
		On_Man_Encode_Zero(); // Clay

		Man_Encode('n');
		Man_Encode('o');
		Man_Encode('n');
		Man_Encode('g');
		Man_Encode('r');
		Man_Encode('e');
		Man_Encode('e');
		Man_Encode('d');
		Man_Encode('y');
		Man_Encode('.');
		Man_Encode('r');
		Man_Encode('u');
		Man_Encode('\r');
		Man_Encode('\n');

		On_Man_Encode_Zero(); /* Tx off */

		for(i = 150000; i; i--);
	}
}

void On_Man_Encode_One(){
 	while(!INTCONbits.TMR0IF);
	TMR0L -= 104; // 4Mhz/4/8/1200bp/s = 104
	INTCONbits.TMR0IF = 0;
	PORTBbits.RB0 = 1;
}

void On_Man_Encode_Zero(){
	while(!INTCONbits.TMR0IF);
	TMR0L -= 104; // 4Mhz/4/8/1200bp/s = 104
	INTCONbits.TMR0IF = 0;
	PORTBbits.RB0 = 0;
}
