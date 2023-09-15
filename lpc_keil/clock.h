#include<LPC214x.h>

void init_PLL()
{
	PLL0CON=0x01;//Enable clock
	PLL0CFG=0x24;//set up pll for 60Mhz
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	while((PLL0STAT & 0x400)==0);
	
	PLL0CON=0x03;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	VPBDIV=0x01;
}

