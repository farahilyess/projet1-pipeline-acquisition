#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "MPU6050.h"

static void convertHexa(uint8_t entree, char* MSB_sortie, char* LSB_sortie){

	uint8_t quartet_haut = (entree >> 4);
	uint8_t quartet_bas = (entree & 0x0FU);

	// Doit aussi etre positif mais comme ce sont des uint la verif est inutile
	if(quartet_haut <= 9U){
		*MSB_sortie = quartet_haut + '0';
	}
	else{
		*MSB_sortie = (quartet_haut - 10U) + 'A';
	}
	// Idem
	if(quartet_bas <= 9U){
		*LSB_sortie = quartet_bas + '0';
	}
	else{
		*LSB_sortie = (quartet_bas - 10U) + 'A';
	}

}

int main(void){

	uint32_t nb_octets_a_lire = MPU6050_NB_OF_REGISTERS_TO_READ;
	uint8_t tableau_mesures[MPU6050_NB_OF_REGISTERS_TO_READ];
	uint32_t taille_tableau_mesures = sizeof(tableau_mesures) / sizeof(tableau_mesures[0]);
    
	usartInit();
	i2cInit();
	i2cWrite(MPU6050_ADDR, MPU6050_REG_PWR_MGMT, MPU6050_PWR_MGMT_WAKEUP); // Lancement des mesures MPU6050
	i2cMultiByteRead(MPU6050_ADDR, MPU6050_INITIAL_REGISTER, tableau_mesures, taille_tableau_mesures, nb_octets_a_lire);

	while(1){

	}
}
