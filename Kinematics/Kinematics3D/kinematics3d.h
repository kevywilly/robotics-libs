//
// Created by Kevin Williams on 9/23/21.
//

#ifndef KINEMATICS_KINEMATICS3D_H
#define KINEMATICS_KINEMATICS3D_H

namespace kinematics3d {
    struct Pos3d {
        float x;
        float y;
        float z;

        Pos3d operator+ (const Pos3d & p) {
            return Pos3d{x+p.x, y+p.y, z+p.z};
        }

    };

    struct Angles {
        float theta1;
        float theta2;
        float theta3;

        Angles operator+ (const Angles & a) {
            return Angles{theta1 + a.theta1, theta2 + a.theta2, theta3 + a.theta3};
        }
    };

    class Kinematics {
    private:
        int l1, l2;
        Pos3d position;
        Angles angles;

        Pos3d updatePosition(float x, float y, float z);

        Angles updateAngles(float theta1, float theta2, float theta3);

    public:
        Kinematics(int length1, int length2);

        void moveToAngle(float theta1, float theta2, float theta3);

        void moveToPosition(float x, float y, float z);

        void moveToPosition(Pos3d p) {
            moveToPosition(p.x, p.y,p.z);
        }

        void moveToOffsetPosition(Pos3d offset) {
            moveToPosition(position + offset);
        }

        void moveToAngles(Angles angles) {
            moveToAngle(angles.theta1, angles.theta2, angles.theta3);
        }

        void moveToOffsetAngles(Angles a) {
            moveToAngles(angles+a);
        }
        Pos3d getPosition();

        Angles getAngles();

        static inline float radians(float deg) {
            return 0.017453298768179*deg;
        }

        static inline float degrees(float rads) {
            return 57.2958 * rads;
        }
    };
}

#endif //KINEMATICS_KINEMATICS3D_H
