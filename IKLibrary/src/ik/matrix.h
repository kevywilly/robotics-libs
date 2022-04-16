//
// Created by Kevin Williams on 4/15/22.
//

#ifndef IK_MATRIX_H
#define IK_MATRIX_H

#include <stdlib.h>

typedef struct ik_matrix {
    int h;
    int w;
    double *data;
} ik_matrix_t;

ik_matrix_t ik_matrix_init(int h, int w) {
    ik_matrix_t m = {.h = h, .w = w};
    m.data = (double *) malloc(m.h * m.w * sizeof(double));
    return m;
}

double ik_matrix_dot(double *a, double *b, int len, int step) {
    double r = 0;
    while (len--) {
        r += *a++ * *b;
        b += step;
    }
    return r;
}

ik_matrix_t ik_matrix_mul(ik_matrix_t *a, ik_matrix_t *b) {
    double *p, *pa;
    int i, j;
    ik_matrix_t r = ik_matrix_init(a->w, a->h);
    if (a->w != b->h) return r;
    p = r.data;
    for (pa = a->data, i = 0; i < a->h; i++, pa += a->w)
        for (j = 0; j < b->w; j++)
            *p++ = ik_matrix_dot(pa, b->data + j, a->w, b->w);
    return r;
}

ik_matrix_t ik_matrix_copy(ik_matrix_t a) {
    ik_matrix_t b = ik_matrix_init(a.h, a.w);
    double *pa = a.data;
    double *pb = b.data;

    for (int i = 0; i < a.h * a.w; i++) {
        *pb++ = *pa++;
    }

    return b;
}

void ik_matrix_print(ik_matrix_t *m) {
    for (int i = 0; i < m->h * m->w; i++) {
        if (i % m->w == 0)
            printf("\n");
        printf("%f\t", m->data[i]);
    }
    printf("\n");
}

#endif // IK_MATRIX_H