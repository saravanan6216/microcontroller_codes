#include "stm32l4xx.h"
#include "lcd.h"
#include <stdlib.h>
void sys(void);
void ADC_Init(void);
void GPIO_Init(void);
void ADC_dis();
void ADC_start_conv(void);
uint32_t readADC_value(void);uint32_t adcValue;
void sysclock80MHZ()
{
	
	/* Enabling the HSI clock - If not enabled and ready */
  if( (RCC->CR & RCC_CR_HSIRDY) == 0) 
  {
    RCC->CR |= RCC_CR_HSION;  /* HSION=1 */

    /* Waiting until HSI clock is ready */
    while( (RCC->CR & RCC_CR_HSIRDY) == 0);
  }
	RCC->PLLCFGR=0;
	 RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI |  // PLL source is HSI
                   //RCC_PLLCFGR_PLLM_0|RCC_PLLCFGR_PLLM_1 |      // Division factor for PLLM1 
                   RCC_PLLCFGR_PLLN_3 |RCC_PLLCFGR_PLLN_1 ;    // Multiplication factor for PLLN10
                  // RCC_PLLCFGR_PLLR_1       // Division factor for PLLR2

    // Enable PLL and wait for it to be ready
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    // Flash latency configuration
    FLASH->ACR = FLASH_ACR_LATENCY_4WS; // Adjust according to your setup

    // Select PLL as system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Configure other clock dividers as needed
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;

    // Update SystemCoreClock variable
    //SystemCoreClock = 80000000; // Adjust the value accordingly
	
}

int main(void)
{
	 volatile unsigned int adcValue=0;
	int a=64;
    // Initialize system
    //SystemInit();
    sys();
	//sysclock80MHZ();
    // Initialize ADC and GPIO
    ADC_Init();
    GPIO_Init();
	ConfigureTimer3();
	unsigned char *p;
	GPIO_INIT(PB);
	LCD_init();
	//LCD_cmd(0x89);
	
	
    while (1)
    {
        ADC_start_conv();

       // int adc;
	//adc=(int)ADC1->DR;

        // Read ADC value
     adcValue = readADC_value();
			p=(unsigned char*)adcValue;
		//	LCD_string(p);
		//	LCD_cmd(0x01);
			//LCD_cmd(0x80);
			//delay(1000);
			 ADC_dis();
			
				//LCD_cmd(0xC0);
			//LCD_string(adcValue);
        // Process the ADC value here
    }
}
uint32_t readADC_value()
{
	int data;
	float volt;
	while (!(ADC1->ISR & ADC_ISR_EOC)){};

        // Read ADC value
	data=ADC123_COMMON->CDR;
			integer(ADC123_COMMON->CDR);
		LCD_cmd(0x85);
		//LCD_string("bits");
		volt=0.0008056 * data;
		LCD_cmd(0xc0);
		 floatt(volt);
			LCD_cmd(0xc5);
	LCD_data('V');
			delay(1000);
			LCD_cmd(0x01);
		
//uint32_t adc2=(uint32_t)ADC1->DR*10000;
        return data;
	
}
void ADC_start_conv()
{
	
	// Start ADC conversion
        ADC1->CR |= 0x10000005;
}
void ADC_dis()
{
	ADC1->CR &= ~0x00000001;
	
}
void ADC_Init(void)
{
    // Enable ADC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    // Configure ADC clock (PCLK2 divided by 2)
    ADC123_COMMON->CCR =0x00020000;//|= ADC_CCR_CKMODE_0;

    // Enable ADC voltage regulator
    //ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator startup
    //while (!(ADC1->ISR & ADC_ISR_REGENRDY));

    // Calibrate ADC
    ADC1->CR =0;
    //while (ADC1->CR & ADC_CR_ADCAL);

    // Configure ADC settings
   // ADC1->CFGR &= ~ADC_CFGR_CONT; // Single conversion mode
   // ADC1->CFGR &= ~ADC_CFGR_ALIGN; // Right alignment
 ADC1->CFGR =0x80000000;
    // Configure ADC channel
    //ADC1->CHSELR = ADC_CHSELR_CHSEL0; // Channel 0
		ADC1->SQR1|=(1<<6);
    // Enable ADC
    //ADC1->CR |= ADC_CR_ADEN;
    //while (!(ADC1->ISR & ADC_ISR_ADRDY));
}

void GPIO_Init(void)
{
    // Enable GPIOA clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Configure PA0 as analog input
    GPIOC->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE0_1;
	GPIOC->ASCR|=1;
}


void sys()
{
	
		/* Enabling the HSI clock - If not enabled and ready */
  if( (RCC->CR & RCC_CR_HSIRDY) == 0) 
  {
    RCC->CR |= RCC_CR_HSION;  /* HSION=1 */

    /* Waiting until HSI clock is ready */
    while( (RCC->CR & RCC_CR_HSIRDY) == 0);
  }

  /* Select AHB prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

  /* APB1 prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

  /* APB2 prescaler to 1 */
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

  /* Select the HSI as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_HSI;

  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_3WS;

  /* Disabling HSE Clock*/
  RCC->CR &= ~RCC_CR_HSEON;
}
/*
  char buffer[50];  
    int a = 15, b = 25, res;  
    res = a + b;  
    sprintf(buffer, "The Sum of %d and %d is %d", a, b, res);  
    printf("%s", buffer);  */
