/*#include "stm32l4xx.h"                  // Device header

void adc_init()
{
	RCC->AHB2ENR|=RCC_AHB2ENR_ADCEN|RCC_AHB2ENR_GPIOCEN; //adc and c port clock enable
	GPIOC->MODER|=(3<<0); // analog mode in co
	ADC1->CR&=~(1U<<29);//deep power mode is disable
	ADC1->CFGR |=(1<<3);// 10bit adc
	ADC1->CR|= (1U<<1); // adc disable
	ADC123_COMMON->CCR |=(1<<22); // VREF ENABLE
	ADC123_COMMON->CCR |=(3<<16); //adc clock mode
	
	ADC1->SMPR1=(7<<0); // SAMPLE TIME
	
	ADC1->SQR1=(1<<0); // set 1 sequence 
	ADC1->CFGR |=(1<<5)|(1<<13); // continuous
	ADC1->CR |=(1<<28)|(1<<0); // adc enable
	
}
void ADC_CONV()
{
	ADC1->ISR |=(1<<0);
	ADC1->CR |=(1<<2); // start conversion
	
}
unsigned int adc_read()
{
	while(!(ADC1->ISR & (1<<2))); // check end of conversion flag
	return(ADC1->DR);
}

int main()
{
	adc_init();
	while(1)
	{
	ADC_CONV();
	adc_read();
	}
}
*/
#include "stm32l4xx.h"

//void SystemInit(void);
void ADC_Init(void);
void GPIO_Init(void);
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
    // Initialize system
    //SystemInit();
    sysclock80MHZ();
    // Initialize ADC and GPIO
    ADC_Init();
    GPIO_Init();

    while (1)
    {
        // Start ADC conversion
        ADC1->CR |= ADC_CR_ADSTART;

        // Wait for conversion to complete
        while (!(ADC1->ISR & ADC_ISR_EOC));

        // Read ADC value
        uint32_t adcValue = ADC1->DR;

        // Process the ADC value here
    }
}

void ADC_Init(void)
{
    // Enable ADC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    // Configure ADC clock (PCLK2 divided by 2)
    ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

    // Enable ADC voltage regulator
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator startup
   // while (!(ADC1->ISR & ADC_ISR_REGENRDY));

    // Calibrate ADC
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // Configure ADC settings
    ADC1->CFGR &= ~ADC_CFGR_CONT; // Single conversion mode
    ADC1->CFGR &= ~ADC_CFGR_ALIGN; // Right alignment

    // Configure ADC channel
    //ADC1->CHSELR = ADC_CHSELR_CHSEL0; // Channel 0
		ADC1->SQR1|=(5<<6)|(1<<0);
    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
}

void GPIO_Init(void)
{
    // Enable GPIOA clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Configure PA0 as analog input
    GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE0_1;
}


void SystemInit()
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
	


////#include "stm32l4xx.h"                  // Device header

//static void delay( uint32_t ms )
//{
//  uint32_t i;
//  for( i = 0; i <= ms; i++ )
//  {
//    /* Clear the count */
//    TIM3->CNT = 0;

//    /* Wait UIF to be set */
//    while((TIM3->SR & TIM_SR_UIF) == 0);    /* This will generate 1ms delay */

//    /* Reset UIF */
//    TIM3->SR &= ~TIM_SR_UIF;
//  }
//}

// 

//static void SetSystemClockTo16Mhz(void)
//{
//  /* Enabling the HSI clock - If not enabled and ready */
//  if( (RCC->CR & RCC_CR_HSIRDY) == 0) 
//  {
//    RCC->CR |= RCC_CR_HSION;  /* HSION=1 */

//    /* Waiting until HSI clock is ready */
//    while( (RCC->CR & RCC_CR_HSIRDY) == 0);
//  }

//  /* Select AHB prescaler to 1 */
//  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

//  /* APB1 prescaler to 1 */
//  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

//  /* APB2 prescaler to 1 */
//  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

//  /* Select the HSI as system clock source */
// // RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//  RCC->CFGR |= RCC_CFGR_SW_HSI;

//  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
//  //FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_3WS;

//  /* Disabling HSE Clock*/
//  //RCC->CR &= ~RCC_CR_HSEON;
//}

// 

// 

//static void ConfigureTimer3(void)
//{
//  /* Enable the APB clock FOR TIM3  */
//  SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);

//  /* fCK_PSC / (PSC[15:0] + 1)
//     (16 MHz / (15+1)) = 1 MHz timer clock speed */
//  TIM3->PSC = 15;

//  /* (1 MHz / 1000) = 1KHz = 1ms */
//  /* So, this will generate the 1ms delay */
//  TIM3->ARR = 1000-1;

//  /* Finally enable TIM3 module */
//  TIM3->CR1 = (1 << 0);
//	
//	
//}

//void ADC_init()
//{
//	/*RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN | RCC_AHB2ENR_GPIOAEN;
//	ADC123_COMMON ->CCR |=	(1<<18)|(2<<16);
//	ADC1->CFGR|=ADC_CFGR_CONT|(1<<3)/*ADC_CFGR_RES*/
//	//ADC1->CR|=ADC_CR_ADVREGEN|ADC_CR_ADEN;*/
//	// Enable the ADC clock
//    //RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

//    // Set the ADC prescaler (if needed)
//    // By default, it's divided by 1, but you can change it if needed.
//    // ADC->CCR |= ADC_CCR_PRESC_Msk; // Set the desired prescaler value
//    
//    // Configure the ADC parameters
//   // ADC1->CR &= ~ADC_CR_ADEN; // Disable ADC
//   // ADC1->CFGR &= ~ADC_CFGR_RES; // Set resolution to 12-bit (default)

//    // Configure ADC channel (for example, channel 1)
//    // Set the channel and its sampling time
//   // ADC1->SMPR1 |= ADC_SMPR1_SMP1_0 | ADC_SMPR1_SMP1_1; // 15 cycles sampling time for channel 1

//    // Configure the ADC scan mode (if needed)
//    // ADC1->CR |= ADC_CR_SCAN;

//    // Calibrate ADC
//    //ADC1->CR |= ADC_CR_ADCAL;
//    //while (ADC1->CR & ADC_CR_ADCAL) {
//        // Wait for calibration to complete
//    //}

//    // Enable ADC
//   // ADC1->ISR |= ADC_ISR_ADRDY;
//   // ADC1->CR |= ADC_CR_ADEN;
//   // while (!(ADC1->ISR & ADC_ISR_ADRDY)) {
//        // Wait for ADC to be ready
//   // }
//	 RCC->AHB2ENR|=RCC_AHB2ENR_ADCEN|RCC_AHB2ENR_GPIOCEN;
//	 GPIOC->MODER&=~(3U<<0);
//	 GPIOC->MODER|=(3<<0);
//	 
//	 ADC1->CR=0;
//	 ADC1->SMPR1=0;
//	 ADC123_COMMON->CCR |= (1<<16)|(1<<22);
//		ADC123_COMMON->CCR &=~(0x1FU<<1);
//		ADC1->CFGR|=(1<<3)|(1<<13)|(1<<26)|(1<<23);
//		ADC1->SQR1|=(1<<6);
//		
//		ADC1->CR |=(1<<0)|(1<<28);
//		
//		
//}
///*void start_adc()
//{
//	
//	ADC1->CR|=ADC_CR_ADSTART;
//	while(!(ADC1->ISR & ADC_ISR_ADRDY));
//}*/
//uint32_t ADC_ReadChannel() {
//   // Configure the sequence for regular channel conversion
//    //ADC1->SQR1 &= ~ADC_SQR1_L; // Clear the number of conversion bits
//    //DC1->SQR1 |= (0 << ADC_SQR1_L_Pos); // Set to 1 conversion

//    // Configure the first conversion in the sequence
//   // ADC1->SQR1 &= ~ADC_SQR1_SQ1; // Clear the first conversion channel bits
//   // ADC1->SQR1 |= (channel << ADC_SQR1_SQ1_Pos); // Set the desired channel for the first conversion


//    // Start the conversion
//   // ADC1->CR |= ADC_CR_ADSTART;

//    // Wait for the conversion to complete
//   // while (!(ADC1->ISR & ADC_ISR_EOC)) {
//        // Wait for end of conversion
//   // }

//    // Read the converted value
//   // return ADC1->DR;
//	//ADC1->CR|=ADC_CR_ADCAL;
//	ADC1->CR|=ADC_CR_ADSTART;
//	while(!(ADC1->ISR & ADC_ISR_EOC));
//	return ADC1->DR;
//}
//int main()
//{
//	volatile uint32_t a;
//	 SetSystemClockTo16Mhz();
//	ConfigureTimer3();
//	ADC_init();
//	// ADC_Init();

//    while (1) 
//			{
//        a = ADC_ReadChannel(); // Replace with the desired channel
//				//ADC1->CR&=~ADC_CR_ADCAL;
//        // Process the ADC value as needed
//    }
//	
//}