#include "Man_Encode.h"

/********************************************************************
 *      Function Name:  Man_Encode                                  *
 *      Return Value:   no                                          *
 *      Parameters:     character to transmit                       *
 *      Description:    Convert char to Manchester Code (2 chars)   *
 *                      MSB is first to convert                     *
 ********************************************************************/

void Man_Encode(register char character) {
	register unsigned char bitcount = 8;

	while (bitcount--) {
		if (character & 0x80) {
			On_Man_Encode_Zero();
			On_Man_Encode_One();

		} else {
			On_Man_Encode_One();
			On_Man_Encode_Zero();
		}
		character <<= 1;
	}
}
