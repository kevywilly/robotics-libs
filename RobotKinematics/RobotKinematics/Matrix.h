//
// Created by Kevin Williams on 9/24/21.
//

#ifndef ROBOTKINEMATICS_MATRIX_H
#define ROBOTKINEMATICS_MATRIX_H

typedef struct Matrix{
    int h, w;
    float *x;

    Matrix * operator* (Matrix * second) {
        return mat_mul(this, second);
    }


    Matrix(int h, int w) : h(h), w(w) {
        x = new float[h*w];
    }

    Matrix(int h, int w, float *x) : h(h), w(w), x(x) {}

    static Matrix * of(int h, int w, float value) {
        Matrix * m = new Matrix(h,w);
        for(int i=0; i < h*w; i++) {
            m->x[i] = 1;
        }
        return m;
    }

    static inline float dot(float *a, float *b, int len, int step)
    {
        float r = 0;
        while (len--) {
            r += *a++ * *b;
            b += step;
        }
        return r;
    }

    static inline Matrix * mat_mul(Matrix * a, Matrix * b)
    {
        float *p, *pa;
        int i, j;
        if (a->w != b->h) return nullptr;

        Matrix * r = new Matrix(a->w, a->h);

        p = r->x;
        for (pa = a->x, i = 0; i < a->h; i++, pa += a->w)
            for (j = 0; j < b->w; j++)
                *p++ = dot(pa, b->x + j, a->w, b->w);
        return r;
    }

    Matrix * copy() {
        Matrix * m = new Matrix(this->h, this->w);
        float * p = this->x;
        float * pp = m->x;
        for(int i=0; i < this->h*this->w; i++) {
            *pp++ = *p++;
        }
        return m;
    }

} Matrix;
#endif //ROBOTKINEMATICS_MATRIX_H
