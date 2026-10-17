#include "main.h"
#include "stm32f446xx.h"

#define MPU6050_I2C_ADDR (0x68UL)
#define MPU6050_REG_WHO_AM_I (0x75UL)
#define MPU6050_REG_PWR_MGMT (0x6BUL)
#define MPU6050_ACCEL_Z_MSB (0x3FUL)
#define MPU6050_ACCEL_Z_LSB (0x40UL)


#define I2C_WRITE_MODE (0UL)
#define I2C_READ_MODE (1UL)
#define I2C_READ_SUCCESS (0UL)


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
	while ((USART2->SR & 0x80 ) == 0){ // 0x80 correspond à bit 7 soit TXE à 1

	}

	USART2->DR = msg;
}

void usartConvertHexa(uint8_t entree, uint8_t* MSB, uint8_t* LSB){

	*MSB = (entree >> 4);
	*LSB = (entree & 0x0F);

	if(0 <= *MSB && *MSB <= 9){
		*MSB = *MSB + '0';
	}
	else{
		*MSB = (*MSB - 10) + 'A';
	}

	if(0 <= *LSB && *LSB <= 9){
		*LSB = *LSB + '0';
	}
	else{
		*LSB = (*LSB - 10) + 'A';
	}

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

static void i2cStart(void){
	I2C1->CR1 |= (1UL << 8); // START

	while ((I2C1->SR1 & 0x01UL ) == 0){

	}
}

void i2cRead(uint32_t i2c_registre_a_lire, uint8_t* valeur_lue){

	//uint32_t read_result; // initialiser après comme dans le filtre en C

	i2cStart();

	I2C1->DR = (MPU6050_I2C_ADDR << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

	while ((I2C1->SR1 & 0x02UL ) == 0){

	}

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->DR = i2c_registre_a_lire;

	while ((I2C1->SR1 & 0x04UL ) == 0){

	}

	i2cStart();

	I2C1->DR = (MPU6050_I2C_ADDR << 1) | (I2C_READ_MODE);

	while ((I2C1->SR1 & 0x02UL ) == 0){

	}

	I2C1->CR1 &=~ (1UL << 10);

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->CR1 |= (1UL << 9);

	while ((I2C1->SR1 & 0x40UL ) == 0){

	}


	*valeur_lue = (uint8_t)I2C1->DR;

}

void i2cWrite(uint32_t i2c_registre_a_lire){
	i2cStart();

	I2C1->DR = (MPU6050_I2C_ADDR << 1) | (I2C_WRITE_MODE); // On concatene l'adresse avec le mode ecriture / lecture

	while ((I2C1->SR1 & 0x02UL ) == 0){

	}

	(void)I2C1->SR1;
	(void)I2C1->SR2;

	I2C1->DR = i2c_registre_a_lire;

	while ((I2C1->SR1 & 0x04UL ) == 0){

	}

	I2C1->DR = 0x01;

	while ((I2C1->SR1 & 0x04UL ) == 0){

	}

	I2C1->CR1 |= (1UL << 9);

}


int main(void){

	uint8_t accel_Z_MSB, accel_Z_LSB;
	uint8_t M_MSB, M_LSB, L_MSB, L_LSB;
	float accel_Z_float_g;
	usartInit();
	i2cInit();

	i2cWrite(MPU6050_REG_PWR_MGMT); // Mise en marche des mesures, a mettre dnas l init ?

	i2cRead(MPU6050_ACCEL_Z_MSB, &accel_Z_MSB);
	i2cRead(MPU6050_ACCEL_Z_LSB, &accel_Z_LSB);

	// Affichage en hexa de l acceleration selon Z
	usartConvertHexa(accel_Z_MSB, &M_MSB, &M_LSB);
	usartConvertHexa(accel_Z_LSB, &L_MSB, &L_LSB);
	usartSend(M_MSB);
	usartSend(M_LSB);
	usartSend(L_MSB);
	usartSend(L_LSB);

	int16_t accel_Z = (int16_t)(((uint16_t) accel_Z_MSB << 8) | accel_Z_LSB);
	accel_Z_float_g = ((float) accel_Z) / 16384.0f;


	while(1){

	}
}
