#ifndef predictor_H
#define predictor_H

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
using namespace std;

typedef struct {

    unsigned int count;


}counter;


class Predi{
  public:

    uint32_t t_predi;

    uint32_t index_bits;
    uint32_t set_size;
    uint32_t index_extract;
    uint32_t set_pos;

    uint32_t global_bhr_size;
    uint32_t hyb_k;
    uint32_t global_bhr;


    uint32_t predict;
    uint32_t mispredict;
    double mpredi_rate;


    std::string predictor_name;

    counter* b_counter;

    Predi(
        uint32_t K, 
        uint32_t M1,
        uint32_t N,
        uint32_t M2,
        uint32_t type_predi
    );

    //virtual ~Predi();

    uint32_t gshare_Get_set_pos(uint32_t);
    void global_bhr_update(uint32_t, bool);
    uint32_t bi_Get_set_pos(uint32_t);
    void bi_update(uint32_t, bool);
    void bi_predict(uint32_t, bool);
    uint32_t Get_pred_result(uint32_t, uint32_t);
    void print();
    void Cprint();

};

#endif