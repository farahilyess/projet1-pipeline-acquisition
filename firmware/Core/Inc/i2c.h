#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C_MULTI_BYTE_READ_SUCCESS (0UL)
#define I2C_READ_SUCCESS (0UL)
#define I2C_WRITE_SUCCESS (0UL)
#define I2C_WAIT_FAIL (1UL)
#define I2C_ERROR_TAB_SIZE (2UL)

void i2cInit(void);
uint32_t i2cRead(uint8_t adresse_capteur, uint8_t registre_a_lire, uint8_t* valeur_lue);
uint32_t i2cMultiByteRead(uint8_t adresse_capteur, uint8_t registre_initial, uint8_t* tableau_mesures, uint32_t taille_tableau, uint32_t nb_octets_a_lire);
uint32_t i2cWrite(uint8_t adresse_capteur, uint8_t registre_a_modifier, uint8_t valeur_a_ecrire);

#endif
