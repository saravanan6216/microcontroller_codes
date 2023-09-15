#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"
#include "semphr.h"
#include "lcd.h"

void initpll(void);
void task1(void *p);
void task2(void *p);
void task3(void *p);


int main()
{	
  initpll();
//	initserial();  
    lcd_init();
		for(int i=0;i<99;i++)
	{
		volt=i;
		
	
		xTaskCreate(task1,"level_indicator",128,NULL,3,NULL);
		xTaskCreate(task2,"percentage",128,NULL,2,NULL);
		vTaskStartScheduler();
	
  while(1);

}

void task1(void *p)
{
	int tmp;
	while(1) {
		int i;
		show("level indicator");
		tmp=volt/100;
		for(int i=0;i<tmp;i++)
		{
			cmd(0xc0+i);
			data(0xff);
			vTaskDelay(100);
		}
    cmd(0x01);
		vTaskDelay(105);
  }
}

void task2(void *p)
{
	while(1)
	{
   int temp,i;
		temp=volt;
		show("battery %");
		i=temp/10;
		cmd(0xc0);
		show(0x30+i);
		cmd(0xc1);
		i=temp%10;
		show(0x30+i);
		cmd(0xc2);
		show("%");
		vTaskDelay(100);
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