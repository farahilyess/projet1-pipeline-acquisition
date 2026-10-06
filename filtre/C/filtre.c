#include "filtre.h"
#include <math.h>
#include <stddef.h> 

uint32_t filtreCalculAngles(float* angle_X, float* angle_Y, float vit_angulaire_X, float vit_angulaire_Y, float accel_X, float accel_Y, float accel_Z, float delta_t, float coeff){
    float angle_accelero_X, angle_accelero_Y;

    // Faire une verif des valeurs entrees ici, à savoir que les entrees sont bien comprises entre x et y et que les pointeurs sont non nuls
    if (angle_X == NULL || angle_Y == NULL){
        return FILTER_ANGLE_POINTER_IS_NULL;
    }

    if  ((35.0f < vit_angulaire_X) || (vit_angulaire_X < - 35.0f) || (35.0f < vit_angulaire_Y) || (vit_angulaire_Y < - 35.0f)){
        return FILTER_ANGULAR_SPEED_OUT_OF_RANGE;
    }

    angle_accelero_X = atan2f(accel_Y, sqrtf(accel_X * accel_X + accel_Z * accel_Z));
    angle_accelero_Y = atan2f(accel_X, sqrtf(accel_Y * accel_Y + accel_Z * accel_Z));

    *angle_X = coeff * (*angle_X + vit_angulaire_X * delta_t) + (1.0f - coeff) * angle_accelero_X;
    *angle_Y = coeff * (*angle_Y + vit_angulaire_Y * delta_t) + (1.0f - coeff) * angle_accelero_Y;

    return FILTER_SUCCESS;

 }
