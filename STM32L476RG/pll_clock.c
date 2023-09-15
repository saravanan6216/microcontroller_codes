#include "stm32l4xx.h"                  // Device header
void pll_clock_config_80MHZ()
{
	RCC->CFGR|=0xf;
	RCC->PLLCFGR|=0x1000a02;
	RCC->CR |=0x3000563;
	
	
	
	
	
}
int main()
{
	pll_clock_config_80MHZ();
	while(1)
	{
		
	}
	
	
	
}