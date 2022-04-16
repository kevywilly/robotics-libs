#include <iostream>
#include "Kinematics3D/kinematics3d.h"

// https://github.com/kousheekc/Kinematics

using namespace kinematics3d;
using namespace std;

void printAngles(Kinematics * k) {
    Angles angle = k->getAngles();
    cout << "<angle";
    cout << " femur: " << angle.theta1;
    cout << " tibia: " << angle.theta2;
    cout << " coxa: " << angle.theta3;
    cout << " >";
    cout << endl;

}

void printPositions(Kinematics * k) {
    Pos3d position = k->getPosition();
    cout << "<position ";
    cout << " x: " << position.x;
    cout << " y: " << position.y;
    cout << " z: " << position.z;
    cout << " >";
    cout << endl << endl;
}

void print(Kinematics * k) {
    printAngles(k);
    printPositions(k);
}
int main() {
    cout << "Hello, Robot!" << endl << endl;

    Kinematics * rf = new Kinematics(38,77);
    print(rf);


    rf->moveToAngle(10,-100,0);
    print(rf);

    Pos3d home = rf->getPosition();


    rf->moveToPosition(home + Pos3d{0,10,10});
    print(rf);

    rf->moveToPosition(home + Pos3d{-5,10,0});
    print(rf);

    rf->moveToPosition(home);
    print(rf);

    return 0;
}
