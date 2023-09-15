#include<LPC214x.h>

void delay_ms(unsigned int m)
{
	T0CTCR=0x00;
	T0PR=60000-1;
	T0TC=0;
	T0TCR=0x01;//ENABLE TIMER
	while(T0TC < m);
	
	T0TCR=0x00;//diable timer
}

void delay_us(unsigned int m)
{
	T0CTCR=0x00;
	T0PR=60-1;
	T0TC=0;
	T0TCR=0x01;//ENABLE TIMER
	while(T0TC < m);
	
	T0TCR=0x00;//diable timer
}
