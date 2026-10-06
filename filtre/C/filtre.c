#include "filtre.h"
#include <math.h>
#include <stddef.h> 


uint32_t filtreInputVerification(float* angle_X, float* angle_Y, float vit_angulaire_X, float vit_angulaire_Y, float accel_X, float accel_Y, float accel_Z, float delta_t, float coeff){

    uint32_t resultat = FILTER_INPUT_VERIFICATION_SUCCESS; 

    if (angle_X == NULL || angle_Y == NULL){
        resultat = FILTER_ANGLE_POINTER_IS_NULL;
    }
    else if  ((35.0f < vit_angulaire_X) || (vit_angulaire_X < - 35.0f) || (35.0f < vit_angulaire_Y) || (vit_angulaire_Y < - 35.0f)){
        resultat = FILTER_ANGULAR_SPEED_OUT_OF_RANGE;
    }
    else if ((157.0f < accel_X) || (accel_X < - 157.0f ) || (157.0f < accel_Y) || (accel_Y < - 157.0f ) || (157.0f < accel_Z) || (accel_Z < - 157.0f )) {
        resultat = FILTER_ACCELERATION_OUT_OF_RANGE;
    }
    else if ((1.0f < delta_t) || (delta_t < 0.0f )){
        resultat = FILTER_TIME_INTERVAL_OUT_OF_RANGE;
    }
    else if ((1.0f < coeff) || (coeff < 0.0f )){
        resultat = FILTER_COEFFICIENT_OUT_OF_RANGE;
    }

    return resultat;
}


uint32_t filtreCalculAngles(float* angle_X, float* angle_Y, float vit_angulaire_X, float vit_angulaire_Y, float accel_X, float accel_Y, float accel_Z, float delta_t, float coeff){
    float angle_accelero_X, angle_accelero_Y;


    uint32_t filtre_verification_resultat = filtreInputVerification(angle_X, angle_Y, vit_angulaire_X, vit_angulaire_Y, accel_X, accel_Y, accel_Z, delta_t, coeff);

    uint32_t filtre_resultat = filtre_verification_resultat;
    
    if (filtre_verification_resultat == FILTER_INPUT_VERIFICATION_SUCCESS){
        angle_accelero_X = atan2f(accel_Y, sqrtf(accel_X * accel_X + accel_Z * accel_Z));
        angle_accelero_Y = atan2f(accel_X, sqrtf(accel_Y * accel_Y + accel_Z * accel_Z));

        *angle_X = coeff * (*angle_X + vit_angulaire_X * delta_t) + (1.0f - coeff) * angle_accelero_X;
        *angle_Y = coeff * (*angle_Y + vit_angulaire_Y * delta_t) + (1.0f - coeff) * angle_accelero_Y;

        filtre_resultat = FILTER_SUCCESS;
    }

    return filtre_resultat;
 }

