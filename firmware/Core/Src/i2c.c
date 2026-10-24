#include "i2c.h"
#include "stm32f446xx.h"

#define I2C_WRITE_MODE (0UL)
#define I2C_READ_MODE (1UL)

static void i2cStart(void){
	I2C1->CR1 |= (1UL << 8); // START

	while ((I2C1->SR1 & 0x01UL ) == 0UL){ }
}

void i2cInit(void){
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

	I2C1->CR1 |= (1UL << 0); // Activation peripherique

}

void i2cRead(uint8_t adresse_capteur, uint8_t registre_a_lire, uint8_t* valeur_lue){

	//uint32_t read_result; // initialiser après comme dans le filtre en C

	i2cStart();

	I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

	while ((I2C1->SR1 & 0x02UL ) == 0UL){ }

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->DR = registre_a_lire;

	while ((I2C1->SR1 & 0x04UL ) == 0UL){ }

	i2cStart();

	I2C1->DR = (adresse_capteur << 1) | (I2C_READ_MODE);

	while ((I2C1->SR1 & 0x02UL ) == 0UL){ }

	I2C1->CR1 &=~ (1UL << 10);

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->CR1 |= (1UL << 9);

	while ((I2C1->SR1 & 0x40UL ) == 0UL){ }

	*valeur_lue = (uint8_t)I2C1->DR;

}

void i2cMultiByteRead(uint8_t adresse_capteur, uint8_t registre_initial, uint8_t* tableau_mesures, uint32_t taille_tableau, uint32_t nb_octets_a_lire){
	//uint32_t read_result; // initialiser après comme dans le filtre en C

	/*
	 * Conditions d'utilisation : le nombre d'octets à lire minimum avec cette fonction est de 3
	 */

	// Pour empecher les accès hors tableau
	if (nb_octets_a_lire > taille_tableau){
		// faire un retour d erreur 
	}
	
	i2cStart();

	I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

	while ((I2C1->SR1 & 0x02UL ) == 0UL){ }

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->DR = registre_initial;

	while ((I2C1->SR1 & 0x04UL ) == 0UL){ }

	i2cStart();

	I2C1->CR1 |= (1UL << 10); // Pour continuer la lecture

	I2C1->DR = (adresse_capteur << 1) | (I2C_READ_MODE);

	while ((I2C1->SR1 & 0x02UL ) == 0UL){ }

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	for(uint32_t i = 0; i < (nb_octets_a_lire - 3UL); i++){
		while ((I2C1->SR1 & 0x40UL ) == 0UL){ }
		tableau_mesures[i] = (uint8_t)I2C1->DR;
	}

	while ((I2C1->SR1 & 0x04UL ) == 0UL){ }

	I2C1->CR1 &=~ (1UL << 10); // Pour arreter la lecture

	tableau_mesures[nb_octets_a_lire - 3UL] = (uint8_t)I2C1->DR;

	I2C1->CR1 |= (1UL << 9);

	tableau_mesures[nb_octets_a_lire - 2UL] = (uint8_t)I2C1->DR;

	while ((I2C1->SR1 & 0x40UL ) == 0UL){ }

	tableau_mesures[nb_octets_a_lire - 1UL] = (uint8_t)I2C1->DR;

}

void i2cWrite(uint8_t adresse_capteur, uint8_t registre_a_modifier, uint8_t valeur_a_ecrire){
	i2cStart();

	I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

	while ((I2C1->SR1 & 0x02UL ) == 0UL){ }

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->DR = registre_a_modifier;

	while ((I2C1->SR1 & 0x04UL ) == 0UL){ }

	I2C1->DR = valeur_a_ecrire;

	while ((I2C1->SR1 & 0x04UL ) == 0UL){ }

	I2C1->CR1 |= (1UL << 9);

}
