#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <inttypes.h>
#include <tuple>
#include <list>
#include <vector>
#include <bitset>
#include <string>
#include "predictor.h"
#include "chooser.h"
using namespace std;

branch::branch(uint32_t K, uint32_t M1, uint32_t N, uint32_t M2, uint32_t type_predi){

    index_bits = K;
    set_size = pow(2,K);

    bi_M2 = M2;
    gs_N = N;
    gs_M1 = M1;

    H_pre_result = 2;



    H_predict = 0;
    mispredict = 0;
    mpredi_rate = 0;

    bi_pre = new Predi(0, 0, 0, M2, 1);
    gs_pre = new Predi(0, M1, N, 0, 2);

    index_extract = 0;
    for(unsigned int i=0; i < index_bits; i++) {
        index_extract <<= 1;
        index_extract |= 1;
    }

    b_counter = new chooser[set_size];
    for(unsigned int i=0; i < set_size; i++){
        b_counter[i].count = 1;
    }

}

uint32_t branch::Get_set_pos(uint32_t addr){
    uint32_t pos = (addr >> 2) & index_extract;
    return pos;
}


void branch::Cpredict(uint32_t addr, bool taken){
    set_pos = Get_set_pos(addr);
    H_predict++;
    bi_pre->bi_predict(addr, 1);
    gs_pre->bi_predict(addr, 2);

    uint32_t a = gs_pre->Get_pred_result(addr, 2);
    uint32_t b = bi_pre->Get_pred_result(addr, 1);

    if(b_counter[set_pos].count >= 2){
        H_pre_result = gs_pre->Get_pred_result(addr, 2);
        gs_pre->bi_update(addr, taken);
    }else{
        H_pre_result = bi_pre->Get_pred_result(addr, 1);
        bi_pre->bi_update(addr, taken);
    }

    if(taken && (H_pre_result < 2)){
        mispredict++;
    }else if(!taken && (H_pre_result >= 2)){
        mispredict++;
    }
    
    
    gs_pre->global_bhr_update(addr, taken);


 
    if(taken){
        if(a >=2 && b < 2){
            if(b_counter[set_pos].count < 3){
                b_counter[set_pos].count++;
            }
        }else if(a < 2 && b >= 2){
            if(b_counter[set_pos].count > 0){
                b_counter[set_pos].count--;
            }
        }
    }else{
        if(a < 2 && b >= 2){
            if(b_counter[set_pos].count < 3){
                b_counter[set_pos].count++;
            }
        }else if(a >=2 && b < 2){
            if(b_counter[set_pos].count > 0){
                b_counter[set_pos].count--;
            }
        } 
    }

}

void branch::print(){
    mpredi_rate = (double)(mispredict) / (double)(H_predict) * 100;
    printf("OUTPUT\n");
    cout << " number of predictions:    " << H_predict << endl;
	cout << " number of mispredictions: " << mispredict << endl;
	cout << " misprediction rate:       " << fixed << setprecision(2) << mpredi_rate << "%"<< endl;
    printf("FINAL CHOOSER CONTENTS\n");
    for(unsigned int i=0; i < set_size; i++){
        if(i <= 9){
            cout << " " << i << "  " << b_counter[i].count << endl;
        }else if(i > 9 && i <= 99){
            cout << " " << i << " " << b_counter[i].count << endl;
        }else{
            cout << " " << i << "  " << b_counter[i].count << endl;
        }
    }
    gs_pre->Cprint();
    bi_pre->Cprint();
    //cout << " misprediction rate:       " << fixed << setprecision(2) << mpredi_rate << "%"<< endl;
}