#include <stdio.h>
#include <stdlib.h>
#include "src/ik/pos.h"
#include "src/ik/dh.h"
#include "src/ik/matrix.h"
#include "src/ik/link.h"
#include "src/ik/chain.h"
#include <assert.h>
#include <math.h>

ik_link_t coxa0;
ik_link_t femur0;
ik_link_t tibia0;
ik_chain_t chain0;

#define DEGREES 57.296

ik_pos_t ik(ik_chain_t * chain, ik_pos_t position) {
    double d1 = chain->links[1].dh.r + chain->links[0].dh.r;
    double d2 = chain->links[2].dh.r;
    double x = position.x;
    double y = position.y;
    double z = position.z;
    double squares = x*x + y*y + z*z;
    double r = sqrt(squares);
    double base = atan(y/x);
    double elbow = acos((squares - d1*d1*d2*d2)/(2*d1*d2));
    double shoulder = asin(z/r) + atan(d2*sin(elbow)/(d1 + d2*cos(elbow)));
    return (ik_pos_t){base, elbow, shoulder};

}
int main() {
    coxa0 = ik_link_init("COXA0", 0, -10, 90, (ik_dh_init(24,90,0)));
    femur0 = ik_link_init("FEMUR0", 0, -90, 90, (ik_dh_init(38,-180,0)));
    tibia0 = ik_link_init("TIBIA0", 90,0, 90+45, (ik_dh_init(77,0,0)));

    ik_matrix_print(&coxa0.matrix);
    ik_matrix_print(&femur0.matrix);
    ik_matrix_print(&tibia0.matrix);

    ik_link_t links[3] = {coxa0, femur0, tibia0};

    chain0 = ik_chain_init("LEG0", links, 3);

    printf("\n");

    ik_pos3d_print(chain0.position);

    assert((int)chain0.position.x == 61);
    assert((int)chain0.position.y == 0);
    assert((int)chain0.position.z == -77);

    ik_pos_t angles_est = ik(&chain0, chain0.position);

    ik_pos3d_print(angles_est);

    double angles[3] = {0,0,0};
    ik_chain_calc_move_angles(&chain0, angles);

    ik_pos3d_print(chain0.position);

    assert((int)chain0.position.x == 139);
    assert((int)chain0.position.y == 0);
    assert((int)chain0.position.z == 0);


    return 0;

}