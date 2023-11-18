#include <bits/stdc++.h>

#include "types/types.h"
#include "constants/constants.h"
#include "tools/converters.h"
#include "aes/key.h"
#include "aes/statematrix.h"

using namespace std;

vector<ByteArray> get_blocks(ByteArray b) {
    vector<ByteArray> blocks;

    int remainder = b.size() % KEY_LENGHT;
    int full_blocks = (b.size() - remainder) / KEY_LENGHT;

    for (int i=0; i < full_blocks; ++i)
    {
        ByteArray block;
        
        for (int j=(i * KEY_LENGHT); j < ((i * KEY_LENGHT) + KEY_LENGHT); ++j)
        {
            block.push_back(b[j]);
        }

        blocks.push_back(block);
    }

    if (remainder > 0) {
        ByteArray remainder_block;
        for (int i=(b.size() - remainder); i < b.size(); ++i)
        {
            remainder_block.push_back(b[i]);
        }

        for (int i=0; i < (KEY_LENGHT - remainder); ++i) 
        {
            remainder_block.push_back(Byte(0x00));
        }

        blocks.push_back(remainder_block);
    }
    

    return blocks;
}

ByteArray aes128ecb_encrypt(MasterKey master_key, ByteArray msg) {
    ByteArray result;
    
    vector<ByteArray> blocks = get_blocks(msg); 

    for(auto it=blocks.begin(); it != blocks.end(); ++it) 
    {
        StateMatrix state = StateMatrix(*it);
        state.encrypt(master_key);

        ByteArray state_ba = state.as_ByteArray();

        for (auto jt=state_ba.begin(); jt != state_ba.end(); ++jt){
            result.push_back(*jt);
        }
    }

    return result;
}

ByteArray aes128ecb_decrypt(MasterKey master_key, ByteArray ciphertext) {
    ByteArray result;
    
    vector<ByteArray> blocks = get_blocks(ciphertext); 

    for(auto it=blocks.begin(); it != blocks.end(); ++it) 
    {
        StateMatrix state = StateMatrix(*it);
        state.decrypt(master_key);

        ByteArray state_ba = state.as_ByteArray();

        for (auto jt=state_ba.begin(); jt != state_ba.end(); ++jt){
            result.push_back(*jt);
        }
    }
    
    return result;
}