#include "stm32l4xx.h"                  // Device header
#include <stdio.h>
 static void delay( uint32_t ms )
{
  uint32_t i;
  for( i = 0; i <= ms; i++ )
  {
    /* Clear the count */
    TIM3->CNT = 0;

    /* Wait UIF to be set */
    while((TIM3->SR & TIM_SR_UIF) == 0);    /* This will generate 1ms delay */

    /* Reset UIF */
    TIM3->SR &= ~TIM_SR_UIF;
  }
}

 

static void SetSystemClockTo16Mhz(void)
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

 

 

static void ConfigureTimer3(void)
{
  /* Enable the APB clock FOR TIM3  */
  SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);

  /* fCK_PSC / (PSC[15:0] + 1)
     (16 MHz / (15+1)) = 1 MHz timer clock speed */
  TIM3->PSC = 15;

  /* (1 MHz / 1000) = 1KHz = 1ms */
  /* So, this will generate the 1ms delay */
  TIM3->ARR = 999;

  /* Finally enable TIM3 module */
  TIM3->CR1 = (1 << 0);

}


void tx(unsigned char  a)
{
		USART2->TDR=a; // data sending
		
    while(!(USART2->ISR&(1<<6))); //status register empty the buffer 
}
/*uint16_t rx()
{
	uint16_t b;
	while(!(USART2->ISR&(1<<5))); 
	b=USART2->RDR;
	return b;
}*/


	
void txx(unsigned char *s)
{
	while(*s)
	{
		tx(*s++);
		//delay(1);
	}
}

unsigned char rx()
{

	unsigned char a;
	while(!(USART2->ISR & (1<<5)));
	a=(unsigned char)USART2->RDR;
	
	return a;

}

void rxx()
{
	unsigned char a[10];
	int i=0;
	
	while((a[i-1]!='x') && (i<10))
	{while(!(USART2->ISR & (1<<5)));
	a[i]=(unsigned char)USART2->RDR;
	//	a[i]=rx();
		tx(a[i]);
	//delay(10);
		i++;
	
	}
//	a[i-1]='\0';
	//txx(a);
	
}
void anbu()
{
	unsigned char a[10];
	int i=0;
	int t=1;
	do
	{
		while(!(USART2->ISR & (1<<5)));
	a[i]=(unsigned char)USART2->RDR;
		if((a[i]=='x') && i==10)
		{
			t=0;
			USART2->ICR|=(1<<12);
			break;
		}
		tx(a[i]);
		i++;
	}while(t);
}
/*
void rxx(unsigned char b)
{
	unsigned char *str=" ";
	while(*str!='\0')
	{
		*str=rx();
		++str;
	}
	txx(str);
}*/
void gpio_init_uart()
{
	RCC->APB1ENR1|=(1<<17); //USART2 ENABLE CLOCK
   RCC->AHB2ENR|=(1<<0);//PORT A ENABLE CLOCK
	// GPIOA->MODER&=~(2<<4)|(2<<6);
	 GPIOA->MODER&=~((3<<4)|(3<<6));
	 GPIOA->MODER|=(2<<4)|(2<<6); //PA2 TX
	 GPIOA->OSPEEDR |=(3<<4)|(3<<6);
	  GPIOA->AFR[0]|=(7<<8);
	  GPIOA->AFR[0]|=(7<<12);

}	

void uart2_config()
{
	 USART2->CR1=0x00000000;
		//USART2->CR1|=(1<<0);
	 USART2->CR1&= ~(1U<<12);
	 
	 USART2->BRR=0X0683; //9600 BAUDRATE
   
	 
//
	 //USART2->BRR=(7<<0)|(24<<4);
	  USART2->CR1|=(1<<5)|(1<<2)|(1<<3)|(1<<0);
   
	 
   // USART2->CR1|=(1<<5)|(1<<2)|(1<<3)|(1<<0); //ENABLE TX
}
void uart_start()
{
	USART2->CR1|=(1<<5)|(1<<2)|(1<<3)|(1<<0);
	
}
void uart_stop()
{
	 USART2->CR1=0x00000000;
	
}
void uart_interrupt_config()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	EXTI->IMR1|= EXTI_IMR1_IM27;
NVIC_EnableIRQ(USART2_IRQn);
}
void TX_interrupt(char c)
{
	
	USART2->TDR=c; // data sending
		while(!(USART2->ISR&(1<<6))); 
    //while(!(USART2->CR1&()); //status register empty the buffer 
}
int main()
 {
	SetSystemClockTo16Mhz();
	ConfigureTimer3();
   gpio_init_uart();
	
	
			uart2_config();
//			
   uart_interrupt_config();
			
			 
    while(1)
			
    {
			//uart_start();
			//uart2_config();
			//uart_interrupt_config();
			//NVIC_EnableIRQ(USART2_IRQn);
			//printf("hi printf\n");
			tx('6');
        //txx("anbu");
			
			//char *t=rx();
			//txx(t);
			//rxx();
		//	anbu();
			//uart_stop();
			delay(1000);
    }


}
 unsigned char b;
void USART2_IRQHandler()
{
	//USART1->CR1|=(1<<8);
	//anbu();
	//USART1->CR1&=~(1<<8);
	//txx("hi IRQ  ");
	while(!(USART2->ISR & (1<<5)));
	b=(unsigned char)USART2->RDR;
	//delay(1000);
	//txx("hi IRQ 2 ");
	//b=USART2->RDR;
	//USART1->CR1|=(1<<5);
	 //USART2->CR1&= ~(1<<7)|(1<<5);
	tx(b);
	//USART2->ICR |=USART_ICR_ORECF;
	NVIC_DisableIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
}