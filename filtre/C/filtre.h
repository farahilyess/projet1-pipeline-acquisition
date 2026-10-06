#ifndef FILTRE_H
#define FILTRE_H

#include <stdint.h>

#define FILTER_SUCCESS (0U)
#define FILTER_INPUT_VERIFICATION_SUCCESS (0U)
#define FILTER_ANGLE_POINTER_IS_NULL (1U)
#define FILTER_ANGULAR_SPEED_OUT_OF_RANGE (2U)
#define FILTER_ACCELERATION_OUT_OF_RANGE (3U)
#define FILTER_TIME_INTERVAL_OUT_OF_RANGE (4U)
#define FILTER_COEFFICIENT_OUT_OF_RANGE (5U)


uint32_t filtreCalculAngles(float* angle_X, float* angle_Y, 
                   float vit_angulaire_X, float vit_angulaire_Y,
                   float accel_X, float accel_Y, float accel_Z,
                   float delta_t, 
                   float coeff);

uint32_t filtreInputVerification (float* angle_X, float* angle_Y, 
                   float vit_angulaire_X, float vit_angulaire_Y,
                   float accel_X, float accel_Y, float accel_Z,
                   float delta_t, 
                   float coeff);


#endif 

