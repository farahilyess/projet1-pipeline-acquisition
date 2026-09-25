#include "main.h"
#include"stm32f446xx.h"

void usart_init(void){
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

void usart_send(uint8_t msg){ // on a seulement 8 bits de data à envoyer donc uint8_t
// retourner erreur plus tard
	while ((USART2->SR & 0x80 ) == 0){ // 0x80 correspond à bit 7 soit TXE à 1

	}

	USART2->DR = msg;
}

void i2c_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // Activation clock pour I2C1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN ; // Activation clock GPIOB car I2C sur PB8 et PB9

	// Configuration de PB8 et PB9 en alternate function
	GPIOB->MODER &= ~((3UL << 16) | (3UL << 18));
	GPIOB->MODER |= ((1UL << 17) | (1UL << 19));

	// Configuration mode AF4 pour I2C1
	GPIOB->AFR[1] &= ~ ((0xFUL << 0) | (0xFUL << 4));
	GPIOB->AFR[1] |= ((0x4UL << 0) | (0x4UL << 4));

	GPIOB->OTYPER |= ((1UL << 8) |(1UL << 9));

	I2C1->CR2 &= ~ (0x3FUL << 0);
	I2C1->CR2 |= (16UL << 0); // 16 MHz

	I2C1->CCR &= ~ ((1UL << 15) | (1UL << 14) | (0xFFFUL << 0));
	I2C1->CCR |=  (80UL << 0);

	I2C1->TRISE = 17U;

	I2C1->CR1 |= (1UL << 0) ; // Activation peripherique

}

int main(void){

	usart_init();
	i2c_init();
	I2C1->CR1 |= (1UL << 8); // START
	while ((I2C1->SR1 & 0x01UL ) == 0){

	}
	uint8_t msg = '1';
	usart_send(msg);

	while(1){

	}
}
