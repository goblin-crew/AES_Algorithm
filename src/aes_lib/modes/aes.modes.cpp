#include <bits/stdc++.h>
#include "../aes.cipherblock.lib.h"
#include "aes.modes.h"

using namespace std;
using namespace AES;


bytes ECB::encrypt(bytes value, bytes key){
    vector<bytes> plaintext_blocks = split_2_blocks(value);
    bytes ciphertext;

    for (auto it=plaintext_blocks.begin(); it != plaintext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(*it, key);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            ciphertext.push_back(*jt);
        }
    }

    return ciphertext;
}

bytes ECB::decrypt(bytes value, bytes key){
    vector<bytes> ciphertext_blocks = split_2_blocks(value);
    bytes plaintext;

    for (auto it=ciphertext_blocks.begin(); it != ciphertext_blocks.end(); ++it){
        bytes ct = cipherblock_decrypt(*it, key);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            plaintext.push_back(*jt);
        }
    }

    return plaintext;
}

//----------------------------------------------------------

bytes CBC::encrypt(bytes value, bytes key, bytes iv){
    vector<bytes> plaintext_blocks = split_2_blocks(value);
    bytes ciphertext;

    for (auto it=plaintext_blocks.begin(); it != plaintext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(bytes_xor(*it, iv), key);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            ciphertext.push_back(*jt);
        }

        iv = ct;
    }

    return ciphertext;
}

bytes CBC::decrypt(bytes value, bytes key, bytes iv){
    vector<bytes> ciphertext_blocks = split_2_blocks(value);
    bytes plaintext;

    for (auto it=ciphertext_blocks.begin(); it != ciphertext_blocks.end(); ++it){
        bytes ct = cipherblock_decrypt(*it, key);
        ct = bytes_xor(ct, iv);
        
        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            plaintext.push_back(*jt);
        }

        iv = *it;
    }

    return plaintext;
}

//----------------------------------------------------------

bytes PCBC::encrypt(bytes value, bytes key, bytes iv){
    vector<bytes> plaintext_blocks = split_2_blocks(value);
    bytes ciphertext;

    for (auto it=plaintext_blocks.begin(); it != plaintext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(bytes_xor(*it, iv), key);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            ciphertext.push_back(*jt);
        }

        iv = bytes_xor(ct, *it);
    }

    return ciphertext;
}

bytes PCBC::decrypt(bytes value, bytes key, bytes iv){
    vector<bytes> ciphertext_blocks = split_2_blocks(value);
    bytes plaintext;

    for (auto it=ciphertext_blocks.begin(); it != ciphertext_blocks.end(); ++it){
        bytes ct = cipherblock_decrypt(*it, key);
        ct = bytes_xor(ct, iv);
        
        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            plaintext.push_back(*jt);
        }

        iv = bytes_xor(ct, *it);
    }

    return plaintext;
}

//----------------------------------------------------------

bytes CFB::encrypt(bytes value, bytes key, bytes iv){
    vector<bytes> plaintext_blocks = split_2_blocks(value);
    bytes ciphertext;

    for (auto it=plaintext_blocks.begin(); it != plaintext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(iv, key);
        ct = bytes_xor(ct, *it);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            ciphertext.push_back(*jt);
        }

        iv = ct;
    }

    return ciphertext;
}

bytes CFB::decrypt(bytes value, bytes key, bytes iv){
    vector<bytes> ciphertext_blocks = split_2_blocks(value);
    bytes plaintext;

    for (auto it=ciphertext_blocks.begin(); it != ciphertext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(iv, key);
        ct = bytes_xor(ct, *it);
        
        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            plaintext.push_back(*jt);
        }

        iv = *it;
    }

    return plaintext;
}

//----------------------------------------------------------

bytes OFB::encrypt(bytes value, bytes key, bytes iv){
    vector<bytes> plaintext_blocks = split_2_blocks(value);
    bytes ciphertext;

    for (auto it=plaintext_blocks.begin(); it != plaintext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(iv, key);
        iv = ct;

        ct = bytes_xor(ct, *it);

        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            ciphertext.push_back(*jt);
        }
    }

    return ciphertext;
}

bytes OFB::decrypt(bytes value, bytes key, bytes iv){
    vector<bytes> ciphertext_blocks = split_2_blocks(value);
    bytes plaintext;

    for (auto it=ciphertext_blocks.begin(); it != ciphertext_blocks.end(); ++it){
        bytes ct = cipherblock_encrypt(iv, key);
        iv = ct;

        ct = bytes_xor(ct, *it);
        
        for (auto jt=ct.begin(); jt != ct.end(); ++jt) {
            plaintext.push_back(*jt);
        }
    }

    return plaintext;
}

//----------------------------------------------------------