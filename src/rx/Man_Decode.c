#include "Man_Decode.h"

static bool ds_LB;

/********************************************************************
 *      Function Name:  Man_Decode_Stable_Zero                      *
 *      Return Value:   no                                          *
 *      Parameters:     Stable digital input periods. Ideal 1 or 2  *
 *      Description:    Convert signal from Manchester Code.        *
 *                      Fire according On_Man_Decode_Add_1()        *
 *                      callback event.                             *
 ********************************************************************/

void Man_Decode_Stable_Zero(register unsigned char periods) {
	if ( periods ) {
		if ( !--periods ) {
			if ( ds_LB ) {
				On_Man_Decode_Add_1();
				ds_LB = 1;
			}
		} else if ( !--periods ) {
			On_Man_Decode_Add_1();
			ds_LB = 1;
		}
	}
}

/********************************************************************
 *      Function Name:  Man_Decode_Stable_One                       *
 *      Return Value:   no                                          *
 *      Parameters:     Stable digital input periods. Ideal 1 or 2  *
 *      Description:    Convert signal from Manchester Code.        *
 *                      Fire according On_Man_Decode_Add_0()        *
 *                      callback event.                             *
 ********************************************************************/

void Man_Decode_Stable_One(register unsigned char periods) {
	if ( periods ) {
		if ( !--periods ) {
			if ( !ds_LB ) {
				On_Man_Decode_Add_0();
				ds_LB = 0;
			}
		} else if ( !--periods ) {
			On_Man_Decode_Add_0();
			ds_LB = 0;
		}
	}
}
