#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"
#include "semphr.h"
#include "LCD.h"
//#include "adc.h"

void initpll(void);
void task1(void *p);
void task2(void *p);
void task3(void *p);

int volt,i;
int main()
{	
  initpll();
//	initserial(); 
		IO1DIR|=0xFF000000;
    lcd_init();
		// volt=adc(0,6);
		xTaskCreate(task1,"level_indicator",128,NULL,3,NULL);
		xTaskCreate(task2,"percentage",128,NULL,2,NULL);
		vTaskStartScheduler();
	
 

}

void task1(void *p)
{
	int tmp;
	while(1) 
	{
		int i;
		volt=24;
		//volt=volt/100;
					cmd(0x80);
		show("level=");
		tmp=volt/20;
		for(i=0;i<tmp;i++)
		{
			cmd(0x86+i);
			dat(0xff);
			
		}
   		vTaskDelay(1000);
  }
}

void task2(void *p)
{
	while(1)
	{
   int temp,i;
		volt=12;
		//volt=volt/100;
		temp=volt;
		cmd(0xc0);
		show("batt=");
		cmd(0xc5);
		i=temp/10;
		dat((0x30+i));
		cmd(0xc6);
		i=temp%10;
		dat((0x30+i));
		cmd(0xc7);
		show("%");
		vTaskDelay(1000);
  }
}


void initpll(void)
{
	PLL0CON=0x01;
	PLL0CFG=0x24;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	while(!(PLL0STAT&1<<10));
	PLL0CON=0x03;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	VPBDIV=0x01;
}