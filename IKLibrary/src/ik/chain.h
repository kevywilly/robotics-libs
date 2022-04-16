//
// Created by Kevin Williams on 4/15/22.
//

#ifndef IK_CHAIN_H
#define IK_CHAIN_H

#include "link.h"
#include "matrix.h"
#include "pos.h"

typedef struct ik_chain {
    const char *id;
    int num_links;
    ik_link_t *links;
    ik_matrix_t *matrix;
    ik_pos_t position;
    double *angles;
} ik_chain_t;

void ik_chain_gen_fk(ik_chain_t *chain) {
    ik_matrix_t matrix = chain->links[0].matrix;
    for (int i = 0; i < chain->num_links; i++) {
        chain->angles[i] = chain->links[i].theta;
        ik_build_matrix(&chain->links[i]);
        if (i > 0)
            matrix = ik_matrix_mul(&matrix, &chain->links[i].matrix);
        chain->position = (ik_pos_t) {.x = matrix.data[3], .y = matrix.data[7], .z = matrix.data[11]};
    }
    chain->matrix = &matrix;
}

void ik_chain_calc_move_angles(ik_chain_t * chain, double * angles) {
    for(int i=0; i < chain->num_links; i++) {
        chain->links[i].theta = angles[i];
    }

    ik_chain_gen_fk(chain);
}

ik_chain_t ik_chain_init(const char *id, ik_link_t *links, int num_links) {
    ik_chain_t chain;
    chain.id = id;
    chain.links = links;
    chain.num_links = num_links;
    chain.angles = (double *) malloc(num_links * sizeof(double));
    for (int i = 0; i < num_links; i++) {
        chain.angles[i] = 0;
    }
    ik_chain_gen_fk(&chain);
    return chain;

}


#endif // IK_CHAIN_H
