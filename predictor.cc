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
using namespace std;

Predi::Predi(uint32_t K, uint32_t M1, uint32_t N, uint32_t M2, uint32_t type_predi){
    
    global_bhr_size = N;

    hyb_k = K;

    global_bhr = 0;

    t_predi = type_predi;
    // type_predi = 0 -> default, type_predi = 1 -> bimodal, type_predi = 2 -> gshare, type_predi = 3 -> hybrid
    if(t_predi == 1){
        //type_predi = 1;
        index_bits = M2;
        set_size = pow(2,M2);
    }else if(t_predi == 2){
        //type_predi = 2; 
        index_bits = M1;
        set_size = pow(2,M1);
    }//else if(K != 0){
        //type_predi = 3;
    //}

    if(N == 0){
        t_predi = 1;
    }

    predict = 0;
    mispredict = 0;
    mpredi_rate = 0;

    /*uint32_t temp_index_extract = 1 << index_bits;
    index_extract = temp_index_extract - 1;*/
    index_extract = 0;
    for(unsigned int i=0; i < index_bits; i++) {
        index_extract <<= 1;
        index_extract |= 1;
    }

    b_counter = new counter[set_size];
    for(unsigned int i=0; i < set_size; i++){
        b_counter[i].count = 2;
    }

}

uint32_t Predi::gshare_Get_set_pos(uint32_t addr){
    uint32_t temp_addr = (addr >> 2) & index_extract;
    /*uint32_t temp_left_addr = (temp_addr >> (index_bits - global_bhr_size)) ^ global_bhr;
    uint32_t left_addr = temp_left_addr << (index_bits - global_bhr_size);
    uint32_t temp_right_addr = temp_addr << global_bhr_size;
    uint32_t right_addr = temp_right_addr >> global_bhr_size;
    uint32_t pos = left_addr | right_addr;*/ 
    uint32_t pos = temp_addr ^ (global_bhr << (index_bits - global_bhr_size));
    return pos;
}


uint32_t Predi::bi_Get_set_pos(uint32_t addr){
    uint32_t pos = (addr >> 2) & index_extract;
    return pos;
}

void Predi::global_bhr_update(uint32_t addr, bool taken){
    if(taken){
        global_bhr = (global_bhr >> 1) + (1 << (global_bhr_size -1)); 
    }else if(!taken){
        global_bhr = global_bhr >> 1;
    }
}

void Predi::bi_update(uint32_t addr, bool taken){
    if(t_predi == 1){
        set_pos = bi_Get_set_pos(addr);
    }else if(t_predi == 2){
        set_pos = gshare_Get_set_pos(addr);
    }
    if(taken && b_counter[set_pos].count < 3){
        b_counter[set_pos].count++;
        return;
    }else if(!taken && b_counter[set_pos].count > 0){
        b_counter[set_pos].count--;
        return;
    }
    return;
}

void Predi::bi_predict(uint32_t addr, bool taken){
    
    predict++;
    if(t_predi == 1){
        set_pos = bi_Get_set_pos(addr);
    }else if(t_predi == 2){
        set_pos = gshare_Get_set_pos(addr);
    }

    if(b_counter[set_pos].count >= 2){
        if(!taken){
            mispredict++;
        }
    }else if(b_counter[set_pos].count < 2){
        if(taken){
            mispredict++;
        }
    }
    //bi_update(taken);
}

uint32_t Predi::Get_pred_result(uint32_t addr, uint32_t Ct_predi){
    if(Ct_predi == 1){
        set_pos = bi_Get_set_pos(addr);
    }else if(Ct_predi == 2){
        set_pos = gshare_Get_set_pos(addr);
    }
    uint32_t temp_result = b_counter[set_pos].count;
    return temp_result;
}

void Predi::print(){
    mpredi_rate = (double)(mispredict) / (double)(predict) * 100;
    printf("OUTPUT\n");
    cout << " number of predictions:    " << predict << endl;
	cout << " number of mispredictions: " << mispredict << endl;
	cout << " misprediction rate:       " << fixed << setprecision(2) << mpredi_rate << "%"<< endl;
    if(t_predi == 1){
        printf("FINAL BIMODAL CONTENTS\n");
    }else if(t_predi == 2){
        printf("FINAL GSHARE CONTENTS\n");
    }
    for(unsigned int i=0; i < set_size; i++){
        if(i <= 9){
            cout << " " << i << "  " << b_counter[i].count << endl;
        }else if(i > 9 && i <= 99){
            cout << " " << i << " " << b_counter[i].count << endl;
        }else{
            cout << " " << i << "  " << b_counter[i].count << endl;
        }
    }
    //cout << " misprediction rate:       " << fixed << setprecision(2) << mpredi_rate << "%"<< endl;
}

void Predi::Cprint(){
    if(t_predi == 1){
        printf("FINAL BIMODAL CONTENTS\n");
    }else if(t_predi == 2){
        printf("FINAL GSHARE CONTENTS\n");
    }
    for(unsigned int i=0; i < set_size; i++){
        if(i <= 9){
            cout << " " << i << "  " << b_counter[i].count << endl;
        }else if(i > 9 && i <= 99){
            cout << " " << i << " " << b_counter[i].count << endl;
        }else{
            cout << " " << i << "  " << b_counter[i].count << endl;
        }
    }

}