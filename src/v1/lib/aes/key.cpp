#include <bits/stdc++.h>

#include "../types/types.h"
#include "../constants/constants.h"
#include "../tools/converters.h"
#include "key.h"

using namespace std;

// private
void Key::expand() {
    /*
        Expands and returns a list of key matrices for the given master_key.
    */

    Matrix key_columns = bytes2matrix(this->master_key);
    int iteration_size = int((this->master_key.size() - (this->master_key.size() % MATRIX_LEN)) / MATRIX_LEN);  //integral_division(master_key.size(), MATRIX_LEN);

    int i = 1;
    while(key_columns.size() < ((N_ROUNDS + 1) * MATRIX_LEN)) 
    {
        ByteArray word = key_columns[int(key_columns.size() - 1)];

        if ((key_columns.size() % iteration_size) == 0) 
        {
            // Circular shift
            word.push_back(word.front()); word.erase(word.begin());

            // map to S-Box
            for (auto it=word.begin(); it != word.end(); ++it){
                *it = S_BOX[*it];
            }

            // XOR with first byte of R-CON, since the others bytes of R-CON are 0.
            word[0] ^= R_CON[i];
            i += 1;
        }
        else if(this->master_key.size() == 32 && (key_columns.size() % iteration_size) == MATRIX_LEN) 
        {
            /* 
                Run word through S-box in the fourth iteration when using a
                256-bit key.
            */

            for (auto it=word.begin(); it != word.end(); ++it){
                *it = S_BOX[*it];
            }
        }

        // XOR with equivalent word from previous iteration
        for (int j = 0; j < word.size(); ++j)
        {
            word[j] ^= key_columns[(key_columns.size() - iteration_size)][j];
        }

        key_columns.push_back(word);
    }

    // Group key words in 4x4 byte matrices
    for(int j=0; j < int((key_columns.size() - (key_columns.size() % MATRIX_LEN)) / MATRIX_LEN); ++j)
    {
        RoundKey rk;
        for(int x=(4 * j); x < (4 * (j + 1)); ++x) 
        {
            rk.push_back(key_columns[x]);
        }

        this->round_keys.push_back(rk);
    }
}

void Key::generate() {
    for (int i=0; i < KEY_LENGHT; ++i)
    {
        // generate random number
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(33, 126);

        this->master_key.push_back(Byte(distr(gen)));
    }
}


// public

Key::Key(){
    this->generate();
    this->expand();
}

Key::Key(MasterKey master_key){
    this->master_key = master_key;
    this->expand();
}


RoundKey Key::round(int round){
    return this->round_keys[round];
}

MasterKey Key::get(){
    return this->master_key;
}
