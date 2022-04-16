//
// Created by Kevin Williams on 9/24/21.
//

#ifndef ROBOTKINEMATICS_CHAIN_H
#define ROBOTKINEMATICS_CHAIN_H

#include "pos3d.h"

class Chain {
public:
    int numJoints;
    Joint ** joints;
    Matrix * matrix = nullptr;
    Pos3D position;
    float * angles = nullptr;
    Pos3D * deltas = nullptr;

    Chain(int numJoints, Joint **joints) : numJoints(numJoints), joints(joints) {
        angles = new float[numJoints];
        buildMatrix();
    }

    void moveAngles(const float a[]) {
        for(int i=0; i < numJoints; i++) {
            joints[i]->setTheta(a[i]);
        }
        buildMatrix();
    }

    void moveAngle(int index, float value) {
        joints[index]->setTheta(value);
    }

    float * moveToPosition(Pos3D pos) {
        genDeltas();
        float threshold = 2;
        float * adj = new float[numJoints];
        Pos3D offset = pos - position;
        float min = 0;
        for(int i=0; i < numJoints; i++) {
            Pos3D delt = deltas[i];
            Pos3D dv = offset.div(deltas[i]);
            float a = (dv).sum();
            if(min == 0 || (abs(a) > 0 && abs(a) < min))
                min = abs(a);

            if(abs(a) == min)
                adj[i] = a;
            else
                adj[i] = 0;
        }
        return adj;
    }

    float * moveToOffset(Pos3D offset) {
        //genDangles();
        float threshold = 2;
        float * adj = new float[numJoints];

        for(int i=0; i < numJoints; i++) {
            adj[i] =  (offset * deltas[i]).sum();
        }

        return adj;
    }
    void moveAngleOffsets(const float a[]) {
        for(int i=0; i < numJoints; i++) {
            joints[i]->addTheta(a[i]);
        }
        buildMatrix();
    }
    float getTheta(int index) {
        return joints[index]->getTheta();
    }

    float getRadians(int index) {
        return joints[index]->getRadians();
    }
    void setTheta(int index, float value) {
        this->joints[index]->setTheta(value);
    }
    void genDeltas() {
        deltas = new Pos3D[numJoints];
        for(int i=0; i < numJoints; i++) {
            Chain * cpy = Chain::copy();
            cpy->setTheta(i, cpy->getTheta(i) + 1);
            cpy->buildMatrix();
            deltas[i] = cpy->position - position;
        }
    }
    Chain * copy() {
        Joint ** jcopy = new Joint*[numJoints];
        for(int i=0; i < numJoints; i++) {
            Joint * j = Chain::joints[i]->copy();
            jcopy[i] = j;
        }
        return new Chain(numJoints, jcopy);
    }

    void buildMatrix() {
        matrix = joints[0]->matrix->copy();
        for(int i=0; i<numJoints; i++) {
            angles[i] = joints[i]->getTheta();
            joints[i]->fillMatrix();
            if(i > 0)
                matrix = (*matrix * joints[i]->matrix);
        }
        position = Pos3D(matrix->x[3], matrix->x[7], matrix->x[11]);
    }

};
#endif //ROBOTKINEMATICS_CHAIN_H
