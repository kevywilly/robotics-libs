//
// Created by Kevin Williams on 4/15/22.
//

#ifndef IK_DH_H
#define IK_DH_H

typedef struct ik_dh {
    double r;
    double alpha;
    double d;
} ik_dh_t;

ik_dh_t ik_dh_init(double r, double alpha, double d) {
    return (ik_dh_t) {.r = r, .alpha = alpha, .d = d};
};


#endif // IK_DH_H
