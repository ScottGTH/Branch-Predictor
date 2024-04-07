#ifndef chooser_H
#define chooser_H

#define ADDRESS_LEN 32 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include <math.h>
#include "inttypes.h"
#include <bitset>
#include <vector>
#include <iostream>
#include <iomanip>
#include "predictor.h"
using namespace std;

typedef struct {

    unsigned int count;


}chooser;


class branch{
  public:
    
    uint32_t index_bits;
    uint32_t set_size;
    uint32_t index_extract;
    uint32_t set_pos;
    uint32_t H_pre_result;

    uint32_t t_predi;

    uint32_t bi_M2;
    uint32_t gs_N;
    uint32_t gs_M1;

    Predi* bi_pre;
    Predi* gs_pre;

    uint32_t H_predict;
    uint32_t mispredict;
    double mpredi_rate;

    chooser* b_counter;

    branch(
        uint32_t K, 
        uint32_t M1,
        uint32_t N,
        uint32_t M2,
        uint32_t type_predi
    );

    uint32_t Get_set_pos(uint32_t );
    void Cpredict(uint32_t, bool);
    void chooser_update(uint32_t, bool);
    void print();

};

#endif