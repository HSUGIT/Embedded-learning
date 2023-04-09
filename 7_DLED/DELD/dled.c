#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"

unsigned char dledcode[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char dledpos[4] = {0b0001, 0b0010, 0b0100, 0b1000};
unsigned char dledbuf[4] = {0x00, 0x00, 0x00, 0x00};

void set_wcode(uint8_t wcode)
{
	P1 = wcode;
}

void set_dcode(uint8_t dcode)
{
	dcode <<= 2;
	P5 &= 0b11000011;
	P5 |= dcode;
}

void dled_get_hexnum(uint16_t num)
{	
	char i;
	
	for(i = 3; i >= 0; i--)
	{
		dledbuf[i] = num % 16;
		num /= 16;
	}
}

void dled_show_hexnum(uint16_t num)
{
	static char i = 3;
	
	set_dcode(dledpos[i]);
	set_wcode(dledcode[dledbuf[i--]]);
	i &= 0b0011;
}