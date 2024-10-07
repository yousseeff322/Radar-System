

#ifndef IR_INTERFACE_H
#define IR_INTERFACE_H

#define STOP		69
#define MODE		70
#define MUTE		71
#define ON			68
#define NEXT		64
#define BACK		67
#define EQ			128
#define VOL_DOWN	21
#define VOL_UP		9
#define ZERO		72
#define RPT			25
#define U_SD		13
#define ONE			12
#define TWO			24
#define THREE		94
#define FOUR		132
#define FIVE		28
#define SIX			90
#define SEVEN		66
#define EIGHT		82
#define NINE		74

#define NO_KEY_PRESSED 255  // Define value for no key pressed


void HIR_voidGetFrame(void);
void HIR_voidExtractKey(void);
void HIR_voidGetKey(void);
u8 	 HIR_u8GetPressedKey(void);   // Add this function to get the last pressed key
void HIR_voidSetCallBack(void(*ptr)(u8 Copy_u8Key));

#endif
