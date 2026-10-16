#include "filtre.h"
#include <stddef.h> 
#include <math.h>
#include <stdbool.h>

// Cette fonction vérifie que le paramètre est dans l'intervalle attendue, pour ensuite faire une vérification inverse pour avoir une erreur en cas de NaN
static bool filtreIntervalleVerification(float parametre, float param_min, float param_max){

    bool resultat = false;

    if (param_min <= parametre && parametre <= param_max){
        resultat = true;
    }

    return resultat;
}


static uint32_t filtreInputVerification(float* angle_X, float* angle_Y, float vit_angulaire_X, float vit_angulaire_Y, float accel_X, float accel_Y, float accel_Z, float delta_t, float coeff){

    uint32_t resultat = FILTER_INPUT_VERIFICATION_SUCCESS; 

    if (angle_X == NULL || angle_Y == NULL){
        resultat = FILTER_ANGLE_POINTER_IS_NULL;
    }
    else if (!(filtreIntervalleVerification(*angle_X, FILTER_MIN_ANGLE, FILTER_MAX_ANGLE) && filtreIntervalleVerification(*angle_Y, FILTER_MIN_ANGLE, FILTER_MAX_ANGLE))){
        resultat = FILTER_ANGLE_OUT_OF_RANGE;
    }
    else if (!(filtreIntervalleVerification(vit_angulaire_X, FILTER_MIN_ANGULAR_SPEED, FILTER_MAX_ANGULAR_SPEED) && filtreIntervalleVerification(vit_angulaire_Y, FILTER_MIN_ANGULAR_SPEED, FILTER_MAX_ANGULAR_SPEED))){
        resultat = FILTER_ANGULAR_SPEED_OUT_OF_RANGE;
    }
    else if (!(filtreIntervalleVerification(accel_X, FILTER_MIN_ACCELERATION, FILTER_MAX_ACCELERATION) && filtreIntervalleVerification(accel_Y, FILTER_MIN_ACCELERATION, FILTER_MAX_ACCELERATION) && filtreIntervalleVerification(accel_Z, FILTER_MIN_ACCELERATION, FILTER_MAX_ACCELERATION))) {
        resultat = FILTER_ACCELERATION_OUT_OF_RANGE;
    }
    else if (!(filtreIntervalleVerification(delta_t, FILTER_MIN_TIME_INTERVAL, FILTER_MAX_TIME_INTERVAL))){
        resultat = FILTER_TIME_INTERVAL_OUT_OF_RANGE;
    }
    else if (!(filtreIntervalleVerification(coeff, 0.0f, 1.0f))){
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

