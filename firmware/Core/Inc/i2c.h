#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2cInit(void);
void i2cRead(uint8_t adresse_capteur, uint8_t registre_a_lire, uint8_t* valeur_lue);
void i2cMultiByteRead(uint8_t adresse_capteur, uint8_t registre_initial, uint8_t* tableau_mesures, uint32_t taille_tableau, uint32_t nb_octets_a_lire);
void i2cWrite(uint8_t adresse_capteur, uint8_t registre_a_modifier, uint8_t valeur_a_ecrire);

#endif
