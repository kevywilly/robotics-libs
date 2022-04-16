//
// Created by Kevin Williams on 9/27/21.
//

#ifndef ROBOTKINEMATICS_POS3D_H
#define ROBOTKINEMATICS_POS3D_H

enum Axis {
    X, Y, Z
};
typedef struct Pos3D {
    double x=0, y=0, z=0;

    Pos3D() {}

    Pos3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Pos3D(Axis axis, double value) : x(0), y(0), z(0) {
        if (axis == Axis::X)
            this->x = value;
        else if (axis == Axis::Y)
            this->y = value;
        else
            this->z = value;
    }

    double at(Axis a) {
        switch (a) {
            case X:
                return x;
            case Y:
                return y;
            default:
                return z;
        }
    }

    double distanceTo(Pos3D Pos3D) const {
        return sqrt(pow((Pos3D.x - x), 2) + pow((Pos3D.y - y), 2) + pow((Pos3D.z - z), 2));
    }

    inline Pos3D copy() { return Pos3D(x, y, z); }

    float sum() {
        return x+y+z;
    }
    Pos3D operator+(const Pos3D &second) {
        return Pos3D{this->x + second.x, this->y + second.y, this->z + second.z};
    }

    Pos3D operator*(const Pos3D &second) {
        return Pos3D{this->x * second.x, this->y * second.y, this->z * second.z};
    }

    Pos3D operator-(const Pos3D &second) {
        return Pos3D{this->x - second.x, this->y - second.y, this->z - second.z};
    }

    bool operator==(const Pos3D &second) {
        return (this->x == second.x && this->y == second.y && this->z == second.z);
    }

    Pos3D div(const Pos3D &second) {
        Pos3D p = {0,0,0};
        p.x = second.x == 0 ? 0 : this->x/second.x;
        p.y = second.y == 0 ? 0 : this->y/second.y;
        p.z = second.z == 0 ? 0 : this->z/second.z;
        return p;
    }
} Point3D, Angle3D;
#endif //ROBOTKINEMATICS_POS3D_H
