#ifndef __MANCHESTER_RX_H
#define __MANCHESTER_RX_H

void Man_Decode_Stable_Zero(unsigned char);
void Man_Decode_Stable_One(unsigned char);

extern void On_Man_Decode_Add_1();
extern void On_Man_Decode_Add_0();

#endif /* __MANCHESTER_RX_H */
