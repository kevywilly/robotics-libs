//
// Created by Kevin Williams on 4/15/22.
//

#ifndef IK_POS_H
#define IK_POS_H

# include <math.h>
# include <stdlib.h>

typedef struct ik_pos {
    double x;
    double y;
    double z;
} ik_pos_t;

ik_pos_t ik_pos_init(double x, double y, double z) {
    return (ik_pos_t) {.x=x, .y=y, .z=z};
};

ik_pos_t pos3d_pow2(ik_pos_t a, ik_pos_t b);

ik_pos_t pos3d_sub(ik_pos_t a, ik_pos_t b);

ik_pos_t pos3d_add(ik_pos_t a, ik_pos_t b);

ik_pos_t pos3d_mult(ik_pos_t a, ik_pos_t b);

float pos3d_dist(ik_pos_t a, ik_pos_t b);

ik_pos_t pos3d_pow2(ik_pos_t a, ik_pos_t b) {
    return (ik_pos_t) {
            .x = a.x * b.x,
            .y = a.y * b.y,
            .z = a.z * b.z
    };
}

ik_pos_t pos3d_add(ik_pos_t a, ik_pos_t b) {
    return (ik_pos_t) {
            .x = a.x + b.x,
            .y = a.y + b.y,
            .z = a.z + b.z
    };
}

ik_pos_t pos3d_mult(ik_pos_t a, ik_pos_t b) {
    return (ik_pos_t) {
            .x = a.x * b.x,
            .y = a.y * b.y,
            .z = a.z * b.z
    };
}

ik_pos_t pos3d_sub(ik_pos_t a, ik_pos_t b) {
    return (ik_pos_t) {
            .x = a.x - b.x,
            .y = a.y - b.y,
            .z = a.z - b.z
    };
}

float pos3d_dist(ik_pos_t a, ik_pos_t b) {
    ik_pos_t pow2 = pos3d_pow2(a, b);
    return sqrt(pow2.x + pow2.y + pow2.z);
}

void ik_pos3d_print(ik_pos_t p) {
    printf("Position: (%f, %f, %f)\n", p.x, p.y, p.z);
}

#endif // IK_POS_H