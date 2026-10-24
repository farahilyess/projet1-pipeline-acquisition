#include "usart.h"
#include "stm32f446xx.h"

void usartInit(void){
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Activation clock pour l'USART2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Activation clock GPIOA car USART2 sur PA2 et PA3

	// Configuration de PA2 et PA3 en alternate function
	GPIOA->MODER &= ~((1UL << 6) | (1UL << 4));
	GPIOA->MODER |= ((1UL << 5) | (1UL << 7));

	// Configuration mode AF7 pour USART2
	GPIOA->AFR[0] &= ~ ((0xFUL << 8) | (0xFUL << 12));
	GPIOA->AFR[0] |= ((0x7UL << 8) | (0x7UL << 12));

	USART2->CR1 |= (1UL << 13);
	USART2->CR1 &= ~ ((1UL << 12) | (1UL << 15)); // OVER8 désactivé

	USART2->CR2 &= ~ (3UL << 12); // Un stop bit

	USART2->BRR = ((8UL << 4) | (0xBUL)); // 8,6875 pour USARTDIV

	USART2->CR1 |= (1UL << 3);
}

void usartSend(uint8_t msg){ // on a seulement 8 bits de data à envoyer donc uint8_t
// retourner erreur plus tard
	while ((USART2->SR & 0x80UL) == 0UL){ // 0x80 correspond à bit 7 soit TXE à 1

	}

	USART2->DR = msg;
}
