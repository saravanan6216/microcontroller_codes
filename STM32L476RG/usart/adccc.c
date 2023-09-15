#include "stm32l4xx.h"                  // Device header
#include "sysclock.h"             // clock selection
#include "lcd.h"

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

void ADC_start_conv()
{
	
	// Start ADC conversion
        ADC1->CR |= 0x10000005;
}

void readADC_value()
{
	int data;
	int d;
	LCD_cmd(0xc0);
		while (!(ADC1->ISR & ADC_ISR_EOC)){};

        // Read ADC value
	data=ADC123_COMMON->CDR;
			d=data&0x0f;
			LCD_string(0x30+d);
			
			delay(1000);
			LCD_cmd(0x01);
//uint32_t adc2=(uint32_t)ADC1->DR*10000;
       
	
}
void ADC_dis()
{
	ADC1->CR &= ~0x00000001;
	
}

int main(void)
{

//    sys();
    ADC_Init();
    GPIO_Init();
	ConfigureTimer3();
	GPIO_INIT(PB);
	LCD_init();

	LCD_string("hi");
	
    while (1)
    {
  /*  ADC_start_conv();
    readADC_value();
			delay(1000);
			 ADC_dis();
			*/
			
			LCD_cmd(0xc0);
			integer(0xf9);
			
    }
}

