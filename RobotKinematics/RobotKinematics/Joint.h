//
// Created by Kevin Williams on 9/24/21.
//

// https://prajankya.me/dh/
// https://www.alanzucconi.com/2020/09/14/inverse-kinematics-in-3d/
//

#ifndef ROBOTKINEMATICS_JOINT_H
#define ROBOTKINEMATICS_JOINT_H

#include "DHParameters.h"
#include "Calculator.h"
#include "Matrix.h"

struct Joint {
private:
    float theta;
    float minTheta;
    float maxTheta;
    DHParameters dh;
public:
    const char * id;
    Matrix * matrix = nullptr;
    Joint() : Joint("joint", 0, 0,0,DHParameters{0,0,0}) {}
    Joint(const char* id, float theta, float minTheta, float maxTheta, DHParameters dh) : id(id), theta(theta), minTheta(minTheta), maxTheta(maxTheta), dh(dh) {
        fillMatrix();
    }

    const DHParameters &getDh() const {
        return dh;
    }

    float getTheta() const {
        return theta;
    }

    float getRadians() const {
        return theta * RADIANS;
    }

    float getMinTheta() const {
        return minTheta;
    }

    float getMidTheta() const {
        return minTheta + (maxTheta - minTheta)/2;
    }
    float getMaxTheta() const {
        return maxTheta;
    }

    float angleNorm(float v) {
        if(v < minTheta)
            return minTheta;
        if(v > maxTheta)
            return maxTheta;
        return v;

    }
    void setTheta(float v) {
        Joint::theta = angleNorm(v);
        Joint::fillMatrix();
    }

    void addTheta(float v) {
        Joint::setTheta(Joint::theta + v);
    }

    Joint * copy() {
        return new Joint(Joint::id, Joint::theta, Joint::minTheta, Joint::maxTheta, Joint::dh.copy());
    }

    void fillMatrix() {

        float *data = new float[16];
        float *d = new float[15];
        float cosTheta = cos(theta*RADIANS);
        float sinTheta = sin(theta*RADIANS);
        float cosAlpha = cos(dh.alpha*RADIANS);
        float sinAlpha = sin(dh.alpha*RADIANS);
        data[0] = cosTheta;
        data[1] = -sinTheta*cosAlpha;
        data[2] = sinTheta*sinAlpha;
        data[3] = dh.r*cosTheta;
        data[4] = sinTheta;
        data[5] = cosTheta*cosAlpha;
        data[6] = -cosTheta*sinAlpha;
        data[7] = dh.r*sinTheta;
        data[8] = 0;
        data[9] = sinAlpha;
        data[10] = cosAlpha;
        data[11] = dh.d;
        data[12] = 0;
        data[13] = 0;
        data[14] = 0;
        data[15] = 1;

        matrix = new Matrix(4,4, data);
    }
};
#endif //ROBOTKINEMATICS_JOINT_H
