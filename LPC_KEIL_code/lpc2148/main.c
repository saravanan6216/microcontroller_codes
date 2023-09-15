#include<LPC214x.h>

#include "clock.h"
#include "timer.h"
#include "UART.h"
#include "ADC.h"
#include "LCD.h"
#include "DAC.h"
#include "interrupt.h"
#include "SPI.h"
unsigned char e[20]="hi i am murali";
int ADC_data;
	char val_j[3];
int main()
{
	
	IO0DIR|=(0x01)<<4;
	//IO1DIR&=~(0x01)<<1;
	init_PLL();
	//UART_init();
	//UART_TX_string(e);
		//delay_ms(1000);
	 //init_interrupt();
	
	while(1)
	{
		IOSET0|=0x01<<4;
		delay_ms(50);
		IOCLR0|=0x01<<4;
		delay_ms(50);
		IOSET0|=0x01<<4;
		delay_ms(50);
		IOPIN0^=0x01<<4;
		delay_ms(50);
		//IO1SET|=(0x01)<<31;
		//delay_ms(1000);
		//IO1CLR|=(0x01)<<31;delay_ms(1000);
	}
	
}

/*int main(void)
{
	
	uint8_t j;
	j = 0;
	init_PLL();
	//LCD_INIT();
	LCD_STRING("Good Day! hello");
	LCD_CMD(0xC0);
	LCD_STRING("Val of j is:");
	
	UART_init();
	//delay_ms(1000);
	ADC_data=ADC();
	sprintf(e,"\nVal of j is: %d \n ",ADC_data);
	UART_TX_string(e);
	for(j=0;j<10;j++)
	{		
		sprintf(val_j,"%d ",j);
		LCD_STRING(val_j);	
		delay_ms(100);
		LCD_CMD(0xCC);
		
	}
	return 0;
}
/*int main()
{
	//init_PLL();
	//IO0DIR|=0XFFFF;
    //lcd_init();
    //cmd(0x80);
    LCD_Init();
	LCD_Command(0x80);
	LCD_String("EMBETRONICX");
	//cmd(0xC0);
	//lcd_string("EMBETRONICX");
	//sprintf(d,"value %d",k);
	//lcd_string(d);
	//lcd_string("ADC");
	/*
 // pinMode(10,output);
	
    while(1) {
      // ADC_data= ADC();
			/*if(ADC_data>0)
			{
				digitalWrite(10,HIGH);
			}
			//sprintf(e,"%d",ADC_data);
			//UART_TX(ADC_data);
			delay_ms(2000);
}}*/uint16_t rx1;char receive, transmit[7];	char c;unsigned char x[50];
		int a;
int main(void)
{
	int i,j;
	i = 0;
	j = 0;
	init_PLL();
	UART_init();
	//memset(transmit,0,7);
	//LCD_INIT();
	//SPI_Init();
	//LCD_STRING("Master send");
	//SPI_Write(0x01);

	/*for(i=0;i<16;i++)
	{
		sprintf(transmit,"i = %d",i);		
		while(transmit[j] != 0)
		{			
			SPI_Write(transmit[j]);
			delay_ms(100);
			j++;
		}
		SPI_Write('\0');
		LCD_CMD(0xC0);
		LCD_STRING(transmit);	
		memset(transmit,0,7);
		j = 0;*/
		//SPI_Write('a');
		//delay_ms(1000);
		UART_TX_string("hi saravanan");
		a=ADC();
		
		sprintf(x,"adc val =%d",a);
		UART_TX_string(x);
		IO0DIR|=0xF0;
		while(1)
		{
		c=UART_RX();delay_ms(1000);
		if(c==1)
		{
			
			IO0SET|=0xF0;
			
	}
	else
	{
		IO0CLR|=0xF0;
	}}
	return 0;
}

