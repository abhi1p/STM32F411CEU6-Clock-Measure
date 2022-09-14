/*
 * HSE_measure.c
 *
 *  Created on: Aug 17, 2022
 *      Author: PC
 */
#include <stdint.h>
#include <stdio.h>

#define RCC_base_addr		(0x40023800UL)
#define RCC_CFGR_offset		(0x08UL)
#define RCC_CR				(RCC_base_addr) //clock control
#define RCC_CFGR			(RCC_base_addr+RCC_CFGR_offset) //clock configure
#define RCC_AHB1ENR			(RCC_base_addr+0x30)

#define GPIOA_base_Addr		(0x40020000UL)
#define GPIOA_MODER			(GPIOA_base_Addr) //GPIOA mode set
#define GPIOA_AFRH			(GPIOA_base_Addr+0x24) //GPIOA alt fun set


int main()
{
	volatile uint32_t *const pRCC_CR=(uint32_t*)RCC_CR; //RCC clock control reg
	volatile uint32_t *const pRCC_CFGR=(uint32_t*)RCC_CFGR; //RCC clock configure reg
	volatile uint32_t *const pGPIOA_MODER=(uint32_t*)GPIOA_MODER; //GPIOA mode set reg
	volatile uint32_t *const pGPIOA_AFRH=(uint32_t*)GPIOA_AFRH; //altfun high reg
	volatile uint32_t *const pRCC_AHB1ENR=(uint32_t*)RCC_AHB1ENR; //AHB1 clock enable reg

	//RCC MCO1 HSE select
	*pRCC_CFGR&=~(3<<21);
	*pRCC_CFGR|=(2<<21);

	printf("MCO1 HSE clock source selected\n");

	//RCC MCO1 pre-scaler set to 4 ->110
	*pRCC_CFGR&=~(7<<24);
	*pRCC_CFGR|=(4<<24);

	printf("MCO1 pre-scaler set to 4\n");

	//GPIOA AHB1ENR clock enable
	*pRCC_AHB1ENR|=(1<<0);

	printf("AHB1 GPIOA clock enable\n");

	//HSE clock enable
	*pRCC_CR|=(1<<16);

	printf("HSE clock enabled\n");

	//wait till HSE is stable
	while(!((*pRCC_CR>>17)&1));

	printf("HSE clock is stable\n");

	//switch system clock to HSE
	*pRCC_CFGR|=(1<<0);

	uint8_t flag=((*pRCC_CFGR>>2)&3);
	if(flag==0)
	{
		printf("System clock not switched to HSE\n");
	}
	else if(flag==1)
	{
		printf("Switched system clock to HSE crystal oscillator\n");
	}
	else
	{
		printf("Not Applicable\n");
	}

	//GPIOA mode set for PA8
	*pGPIOA_MODER&=~(3<<16);
	*pGPIOA_MODER|=(2<<16);

	printf("PA8 mode set to altfun mode\n");

	//PA8 altfun0 set
	*pGPIOA_AFRH&=~(0xf<<0);

	printf("PA8 altfun0 set\n");

	while(1);
	return 0;
}
