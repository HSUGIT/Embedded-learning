#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

unsigned int KEY_GetSta(void)
{
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0);
		Delay_ms(20);
		
		return 1;
	}
	return 0;
}

