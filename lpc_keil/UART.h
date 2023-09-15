#include<LPC214x.h>

void UART_init()
{
	PINSEL0=0x05;
	U0LCR=0x83;
	U0DLL=0x87;
	U0DLM=0x01;
	U0LCR=0x03;
}

void UART_TX(unsigned char my_data)
{
	U0THR=my_data;
	while((U0LSR & (0x01<<5))==0);
}
void UART_TX_string(unsigned char *my_data)
{
	while(*my_data!=0)
	{
		UART_TX(*my_data);
		my_data++;
	}
}
unsigned char UART_RX(void)
{
	unsigned char my_data;
	while((U0LSR & (0x01<<0))==0);
	
	my_data=U0RBR;
	return my_data;
}


	
