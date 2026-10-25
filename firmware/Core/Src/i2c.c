#include "i2c.h"
#include "stm32f446xx.h"

#define I2C_WRITE_MODE (0UL)
#define I2C_READ_MODE (1UL)

#define I2C_WAIT_SUCCESS (0UL)
#define I2C_START_SUCCESS (0UL)


static uint32_t i2cWait(uint32_t drapeau_a_attendre){
	uint32_t resultat = I2C_WAIT_SUCCESS;
	uint32_t compteur = 0;
	const uint32_t nb_tours_max = 4000; // 4000 tours ≈ 1,2 à 2,4 ms à 16 MHz (> 10 × un octet I2C). A revoir si la fréquence change 

	while (((I2C1->SR1 & drapeau_a_attendre ) == 0UL) && (compteur < nb_tours_max)){ 
		compteur++;
	}

	if ((I2C1->SR1 & drapeau_a_attendre ) == 0UL){
		resultat = I2C_WAIT_FAIL;
	}
	else{
		// rien, l'attente s'est bien passée
	}

	return resultat;
}

static uint32_t i2cStart(void){
	uint32_t resultat_start = I2C_START_SUCCESS;
	I2C1->CR1 |= (1UL << 8); // START

	resultat_start = i2cWait(0x01UL);

	return resultat_start;
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

uint32_t i2cRead(uint8_t adresse_capteur, uint8_t registre_a_lire, uint8_t* valeur_lue){
	uint32_t resultat_read = I2C_READ_SUCCESS; 
	resultat_read = i2cStart();

	if (resultat_read == I2C_START_SUCCESS){
		I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

		resultat_read = i2cWait(0x02UL);
	}

	if (resultat_read == I2C_WAIT_SUCCESS){

		(void)I2C1->SR1;
		(void)I2C1->SR2;

		I2C1->DR = registre_a_lire;

		resultat_read = i2cWait(0x04UL);
	}

	if (resultat_read == I2C_WAIT_SUCCESS){	
		resultat_read = i2cStart();
	}

	if (resultat_read == I2C_START_SUCCESS){	
		I2C1->DR = (adresse_capteur << 1) | (I2C_READ_MODE);

		resultat_read = i2cWait(0x02UL);
	}

	if (resultat_read == I2C_WAIT_SUCCESS){

		I2C1->CR1 &=~ (1UL << 10);

		(void)I2C1->SR1;
		(void)I2C1->SR2;

		I2C1->CR1 |= (1UL << 9);

		resultat_read = i2cWait(0x40UL);

	}
	
	if (resultat_read == I2C_WAIT_SUCCESS){
		*valeur_lue = (uint8_t)I2C1->DR;
		resultat_read = I2C_READ_SUCCESS;
	}
	
	return resultat_read;
}

uint32_t i2cMultiByteRead(uint8_t adresse_capteur, uint8_t registre_initial, uint8_t* tableau_mesures, uint32_t taille_tableau, uint32_t nb_octets_a_lire){
	uint32_t resultat_read = I2C_MULTI_BYTE_READ_SUCCESS; 

	/* Conditions d'utilisation : le nombre d'octets à lire minimum avec cette fonction est de 3 */

	// Pour empecher les accès hors tableau
	if (nb_octets_a_lire > taille_tableau){
		resultat_read =  I2C_ERROR_TAB_SIZE;
	}

	if (resultat_read == I2C_MULTI_BYTE_READ_SUCCESS){
		resultat_read = i2cStart();
	}

	if (resultat_read == I2C_START_SUCCESS){
		I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

		resultat_read = i2cWait(0x02UL);
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		(void)I2C1->SR1;
		(void)I2C1->SR2;

		I2C1->DR = registre_initial;

		resultat_read = i2cWait(0x04UL); 
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		resultat_read = i2cStart();
	}

	if (resultat_read == I2C_START_SUCCESS){
		I2C1->CR1 |= (1UL << 10); // Pour continuer la lecture

		I2C1->DR = (adresse_capteur << 1) | (I2C_READ_MODE);

		resultat_read = i2cWait(0x02UL);
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		(void)I2C1->SR1;
		(void)I2C1->SR2;

		for(uint32_t i = 0; i < (nb_octets_a_lire - 3UL); i++){
			resultat_read = i2cWait(0x40UL); 
			if (resultat_read == I2C_WAIT_SUCCESS){
				tableau_mesures[i] = (uint8_t)I2C1->DR;
			}
			else{
				break;
			}
		}
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		resultat_read = i2cWait(0x04UL); 
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		I2C1->CR1 &=~ (1UL << 10); // Pour arreter la lecture

		tableau_mesures[nb_octets_a_lire - 3UL] = (uint8_t)I2C1->DR;

		I2C1->CR1 |= (1UL << 9);

		tableau_mesures[nb_octets_a_lire - 2UL] = (uint8_t)I2C1->DR;

		resultat_read = i2cWait(0x40UL); 
	}

	if (resultat_read == I2C_WAIT_SUCCESS){
		tableau_mesures[nb_octets_a_lire - 1UL] = (uint8_t)I2C1->DR;
		resultat_read = I2C_MULTI_BYTE_READ_SUCCESS;
	}

	return resultat_read;
}

uint32_t i2cWrite(uint8_t adresse_capteur, uint8_t registre_a_modifier, uint8_t valeur_a_ecrire){
	uint32_t resultat_write = I2C_WRITE_SUCCESS;
	resultat_write = i2cStart();

	if (resultat_write == I2C_START_SUCCESS){
		I2C1->DR = (adresse_capteur << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

		resultat_write = i2cWait(0x02UL);
	}

	if (resultat_write == I2C_WAIT_SUCCESS){
		(void)I2C1->SR1;
		(void)I2C1->SR2;

		I2C1->DR = registre_a_modifier;

		resultat_write = i2cWait(0x04UL);
	}

	if (resultat_write == I2C_WAIT_SUCCESS){
		I2C1->DR = valeur_a_ecrire;

		resultat_write = i2cWait(0x04UL);
	}

	if (resultat_write == I2C_WAIT_SUCCESS){
		I2C1->CR1 |= (1UL << 9);
		resultat_write = I2C_WRITE_SUCCESS;
	}

	return resultat_write;
}
