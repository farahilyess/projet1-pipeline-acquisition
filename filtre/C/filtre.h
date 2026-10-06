#ifndef FILTRE_H
#define FILTRE_H

#include <stdint.h>

#define FILTER_SUCCESS (0U)
#define FILTER_ANGLE_POINTER_IS_NULL (1U)
#define FILTER_ANGULAR_SPEED_OUT_OF_RANGE (2U)


uint32_t filtreCalculAngles(float* angle_X, float* angle_Y, 
                   float vit_angulaire_X, float vit_angulaire_Y,
                   float accel_X, float accel_Y, float accel_Z,
                   float delta_t, 
                   float coeff);



#endif 

