//
// Created by Kevin Williams on 9/24/21.
//

#ifndef ROBOTKINEMATICS_CALCULATOR_H
#define ROBOTKINEMATICS_CALCULATOR_H
#include "math.h"
#define DEGREES 57.2958
#define RADIANS 0.017453298768179

static inline double oppositeSide(double hypotenuse, double angle_in_degrees) {
    return sin(angle_in_degrees * RADIANS) * hypotenuse;
}

static inline double adjacentSide(double hypotenuse, double angle_in_degrees) {
    return cos(angle_in_degrees * RADIANS) * hypotenuse;
}

static inline double normalizeAngle(double angle) {
    return angle > 360 ? angle - 360 : angle;
}
#endif //ROBOTKINEMATICS_CALCULATOR_H
