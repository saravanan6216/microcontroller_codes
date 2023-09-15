#include<LPC214x.h>
//unsigned int my_data;
int result;
	float voltage;
	//char volt[18];
unsigned int ADC()
{
	
	/*PINSEL0|=0x00300;
	
	AD0CR=0x00000000;
	AD0CR|=(0x01<<6);
	AD0CR|=13<<8;
	AD0CR|=0x01<<16;
	AD0CR|=0x01<<21;
	
	
	AD0CR|=0x01<<24;
	delay_ms(2);
	while((AD0GDR&(0x01<<31))==0)
		;
	
	my_data=AD0DR6;
	my_data=(my_data>>6)&0x3FF;
	return my_data;
}
*/
// uint32_t 
 
	//LCD_Init();
	PINSEL1 |= 0x01000000; /* P0.28 as AD0.1 */
	AD0CR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */
	//while(1)
	//{
		AD0CR = AD0CR | (1<<24); /* Start Conversion */
		while ( !(AD0DR1 & 0x80000000) ); /* Wait till DONE */
		result = AD0DR1;
// 		result = (result>>6);
// 		result = (result & 0x000003FF);
// 		voltage = ( (result/1023.0) * 3.3 ); /* Convert ADC value to equivalent voltage */
		//LCD_Command(0x80);
		//sprintf(volt, "Voltage=%.2f V  ", voltage);
		//LCD_String(volt);
		//memset(volt, 0, 18);
	return (unsigned int)result;
}