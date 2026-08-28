#include "main.h"
#include"stm32f446xx.h"


int main(void){

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Activation clock pour l'USART2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Activation clock GPIOA USART2 sur PA2 et PA3

	// Configuration de PA2 et PA3 en alternate function
	GPIOA->MODER &= ~((1UL << 6) | (1UL << 4));
	GPIOA->MODER |= ((1UL << 5) | (1UL << 7));

	// Configuration AF7 pour USART2
	GPIOA->AFR[0] &= ~ ((0xFUL << 8) | (0xFUL << 12));
	GPIOA->AFR[0] |= ((0x7UL << 8) | (0x7UL << 12));


	while(1){

	}
}
