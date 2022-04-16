//
// Created by Kevin Williams on 4/15/22.
//

#ifndef IK_LINK_H
#define IK_LINK_H

#define RADIANS 0.0174533

#include "matrix.h"
#include "dh.h"

typedef struct ik_link {
    const char *id;
    double theta;
    double min_theta;
    double max_theta;
    ik_dh_t dh;
    ik_matrix_t matrix;
} ik_link_t;

void ik_build_matrix(ik_link_t *link) {
    double *data = (double *) malloc(16 * sizeof(double));
    float cosTheta = cos(link->theta * RADIANS);
    float sinTheta = sin(link->theta * RADIANS);
    float cosAlpha = cos(link->dh.alpha * RADIANS);
    float sinAlpha = sin(link->dh.alpha * RADIANS);
    data[0] = cosTheta;
    data[1] = -sinTheta * cosAlpha;
    data[2] = sinTheta * sinAlpha;
    data[3] = link->dh.r * cosTheta;
    data[4] = sinTheta;
    data[5] = cosTheta * cosAlpha;
    data[6] = -cosTheta * sinAlpha;
    data[7] = link->dh.r * sinTheta;
    data[8] = 0;
    data[9] = sinAlpha;
    data[10] = cosAlpha;
    data[11] = link->dh.d;
    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 1;

    link->matrix.data = data;
}

ik_link_t ik_link_init(const char *id, double theta, double min_theta, double max_theta, ik_dh_t dh) {
    ik_link_t link = {
            .id = id,
            .theta = theta,
            .min_theta = min_theta,
            .max_theta = max_theta,
            .dh = dh,
            .matrix = ik_matrix_init(4, 4)
    };
    ik_build_matrix(&link);
    return link;
}


#endif //IK_LINK_H