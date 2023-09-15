#include<LPC214x.h>

#include "clock.h"
#include "timer.h"
#include "UART.h"
unsigned char e[20]="hi i am murali";
int data;
	
int main()
{
	
	IO0DIR|=(0x01)<<4;
	IO1DIR&=~(0x01)<<1;
	init_PLL();
	UART_init();
	
	
	
	while(1)
	{
		UART_TX('e');
		delay_ms(1000);
		data=UART_RX();
		delay_ms(1000);
	}
	
}

