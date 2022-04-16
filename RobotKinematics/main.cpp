#include <iostream>
#include <cstdio>
#include "RobotKinematics/Joint.h"
#include "RobotKinematics/Chain.h"
#include "RobotKinematics/IK.h"

void print(Joint *joint);
void print(Matrix * matrix);
void print(Pos3D pos3d) {
    printf("<pos3d x: %f, y: %f, z: %f>\n", pos3d.x, pos3d.y, pos3d.z);
}
void printPosition(Chain * chain) {
    print(chain->position);
}
void printAngles(Chain * chain) {
    printf("<angles");
    for(int i=0; i < chain->numJoints; i++) {
        printf(" %d: %f", i, chain->joints[i]->getTheta());
    }
    printf(">\n");
}
void printDeltas(Chain * chain) {
    printf("Deltas\n");
    for(int i=0; i < chain->numJoints; i++) {
        printf("Joint(%d) ", i);
        print(chain->deltas[i]);
    }
    printf("\n");
}
void print(Chain * chain) {
    printf("=========================\n");
    printPosition(chain);
    printAngles(chain);
}

Joint * coxa;
Joint * femur;
Joint * tibia;
Chain * chain;

void genDataset() {
    int count = 0;
    for(float i=coxa->getMinTheta(); i < coxa->getMaxTheta(); i = i + 5) {
        for(float j=femur->getMinTheta(); j < femur->getMaxTheta(); j = j+15) {
            for(float k=tibia->getMinTheta(); k < tibia->getMaxTheta(); k+=15) {
                chain->moveAngles(new float[3]{i,j,k});
                chain->buildMatrix();
                printf("{");
                Pos3D pos = chain->position;
                printf("%f, %f, %f, ", pos.x, pos.y, pos.z);
                for(int l=0; l < chain->numJoints; l++) {
                    printf("%f", chain->getRadians(l));
                    if(l < chain->numJoints-1)
                        printf(", ");
                }
                printf("},\n");
                count +=1;
            }
        }
    }
    printf("%d", count);
}
int main() {


    coxa = new Joint("coxa", 0, -10, 90, DHParameters{24, 90, 0});
    femur = new Joint("femur", 0, -90, 90, DHParameters{38, -180, 0});
    tibia = new Joint("tibia", 90,0, 90+45,DHParameters{77, 0, 0});
    Joint * joints[3] = {coxa, femur, tibia};


    // initial
    chain = new Chain(3, joints);
    print(chain);
    print(chain->position);
    print(chain->matrix);

    Pos3D cpos = chain->position;

    genDataset();


    return 0;
}

void print(Joint * joint) {
    printf("<%s theta: %f>\n", joint->id, joint->getTheta());
    printf("\t<dh d:%f r: %f alpha: %f>\n", joint->getDh().d, joint->getDh().r, joint->getDh().alpha);
    print(joint->matrix);
}

void print(Matrix * a) {
    int i, j;
    float *p = a->x;
    for (i = 0; i < a->h; i++, putchar('\n'))
        for (j = 0; j < a->w; j++)
            printf("\t%7.3f", *p++);
    putchar('\n');

}
