//
// Created by Kevin Williams on 9/23/21.
//

#include "kinematics3d.h"
#include "math.h"

using namespace kinematics3d;

Kinematics::Kinematics(int length1, int length2){

    l1 = length1;
    l2 = length2;

    angles.theta1 = 90;
    angles.theta2 = -90;
    angles.theta3 = 0;

    position.x = length2;
    position.y = 0;
    position.z = length1;
}

Pos3d Kinematics::updatePosition(float x, float y, float z){
    Pos3d newPositions;
    newPositions.x = x;
    newPositions.y = y;
    newPositions.z = z;
    return newPositions;
}

Angles Kinematics::updateAngles(float theta1, float theta2, float theta3){
    Angles newAngles;
    newAngles.theta1 = theta1;
    newAngles.theta2 = theta2;
    newAngles.theta3 = theta3;
    return newAngles;
}

void Kinematics::moveToAngle(float theta1, float theta2, float theta3){
    float xPrime = l1 * cos(radians(theta1)) + l2 * cos(radians(theta1) + radians(theta2));
    float z = l1 * sin(radians(theta1)) + l2 * sin(radians(theta1) + radians(theta2));
    float x = xPrime * cos(radians(theta3));
    float y = xPrime * sin(radians(theta3));

    angles = updateAngles(theta1, theta2, theta3);
    position = updatePosition(x, y, z);
}

void Kinematics::moveToPosition(float x, float y, float z){
    float xPrime = sqrt(pow(x, 2) + pow(y, 2));

    float theta3 = degrees(atan(y/x));
    float theta2 = -degrees(acos((pow(xPrime, 2) + pow(z, 2) - pow(l1, 2) - pow(l2, 2))/(2 * l1 * l2)));
    float theta1 = degrees(atan(z/xPrime) - atan((l2 * sin(radians(theta2)))/(l1 + l2 * cos(radians(theta2)))));

    angles = updateAngles(theta1, theta2, theta3);
    position = updatePosition(x, y, z);
}

/*
void Kinematics::printPositions(){
    Serial.print("positions:");
    Serial.print("\t");
    Serial.print(position.x);
    Serial.print("\t");
    Serial.print(position.y);
    Serial.print("\t");
    Serial.print(position.z);
    Serial.println("\t");
}

void Kinematics::printAngles(){
    Serial.print("Angles:");
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(angles.theta1);
    Serial.print("\t");
    Serial.print(angles.theta2);
    Serial.print("\t");
    Serial.print(angles.theta3);
    Serial.println("\t");
}
*/
Pos3d Kinematics::getPosition(){
    return position;
}

Angles Kinematics::getAngles(){
    return angles;
}