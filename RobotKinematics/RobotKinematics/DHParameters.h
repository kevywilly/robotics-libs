//
// Created by Kevin Williams on 9/24/21.
//

#ifndef ROBOTKINEMATICS_DHPARAMETERS_H
#define ROBOTKINEMATICS_DHPARAMETERS_H
struct DHParameters {
    float r=0;
    float alpha=0;
    float d=0;

    DHParameters copy() {
        return DHParameters{r,alpha,d};
    }
};
#endif //ROBOTKINEMATICS_DHPARAMETERS_H
