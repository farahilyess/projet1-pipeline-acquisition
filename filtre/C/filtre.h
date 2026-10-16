#ifndef FILTRE_H
#define FILTRE_H

#include <stdint.h>

#define FILTER_PI (3.14159265358979323846f)

#define FILTER_SUCCESS (0U)
#define FILTER_INPUT_VERIFICATION_SUCCESS (0U)
#define FILTER_ANGLE_POINTER_IS_NULL (1U)
#define FILTER_ANGULAR_SPEED_OUT_OF_RANGE (2U)
#define FILTER_ACCELERATION_OUT_OF_RANGE (3U)
#define FILTER_TIME_INTERVAL_OUT_OF_RANGE (4U)
#define FILTER_COEFFICIENT_OUT_OF_RANGE (5U)
#define FILTER_ANGLE_OUT_OF_RANGE (6U)
#define FILTER_OUTPUT_ANGLE_OUT_OF_RANGE (7U)

#define FILTER_MIN_ANGULAR_SPEED (-35.0f) // en rad/s
#define FILTER_MAX_ANGULAR_SPEED (35.0f)  // en rad/s
#define FILTER_MIN_ACCELERATION (-157.0f) // en m/s²
#define FILTER_MAX_ACCELERATION (157.0f)  // en m/s²
#define FILTER_MIN_TIME_INTERVAL (0.0f)   // en secondes
#define FILTER_MAX_TIME_INTERVAL (1.0f)   // en secondes
#define FILTER_MAX_ANGLE (FILTER_PI)      // en rad
#define FILTER_MIN_ANGLE (-FILTER_PI)     // en rad

uint32_t filtreCalculAngles(float* angle_X, float* angle_Y, 
                            float vit_angulaire_X, float vit_angulaire_Y,
                            float accel_X, float accel_Y, float accel_Z,
                            float delta_t, 
                            float coeff);

#endif 

