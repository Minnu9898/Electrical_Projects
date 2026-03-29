
#include "stm32f4xx.h"
#include "adc.h"


#define  ADC1EN  			(1U<<8)
#define  GPIOAEN			(1U<<0)

#define ADC_CH1				(1U<<0) // Channel 1 need to be set in Seq register at position SQ1
#define ADC_SEQ_LEN_1		 0x00
#define CR2_ADON			(1U<<0)

#define CR2_SWSTART			(1U<<30)
#define SR_EOC 				(1U<<1)
#define CR2_CONT			(1U<<1)

void pa1_adc_init(void)
{


	RCC ->AHB1ENR |=GPIOAEN; //Enable clock access to GPIOA
	GPIOA->MODER |=(1U<<2);  //PA1 MODE=ANALOG
	GPIOA->MODER |=(1U<<3);

	RCC ->APB2ENR |= ADC1EN; //Enable clock access to ADC
	/* ADC CONFIGURATIONS
	 * conversion sequence start -SQR3 Register
	 * conversion sequence length
	 * Enable ADC module
	 */
	ADC1->SQR3 = ADC_CH1;        	//sequence
	ADC1->SQR1 = ADC_SEQ_LEN_1; 	//length
	ADC1->CR2 |= CR2_ADON;			//Enable ADC



}

void start_conversion(void)
{
	//Enable continous convertion
	ADC1->CR2  |=CR2_CONT;
	ADC1->CR2 |=CR2_SWSTART; //Start ADC Conversion


}

uint32_t adc_read(void)
{
	/*before reading we need to wait till the conversion is complete */
	while (!(ADC1->SR & SR_EOC))
	{

	}
	return ADC1->DR;

}
