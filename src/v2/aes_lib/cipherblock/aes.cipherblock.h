#ifndef AES_CIPHERBLOCK_H
#define AES_CIPHERBLOCK_H

#include<bits/stdc++.h>

#include "../types/aes.types.h"
#include "aes.cipherblock.constants.h"

using namespace std;


typedef class RoundKey {
    private:
        vector<matrix> round_keys;
        void expand(bytes);

    public:
        vector<matrix>::iterator it;

        RoundKey(bytes);

        vector<matrix>::iterator end();
        vector<matrix>::iterator begin();

        matrix operator [] (int);
        void operator ++ ();
        void operator -- ();

} RoundKey;


typedef class CipherBlock {
    private:
        Byte xtime(Byte a);

    public:
        matrix value;

        CipherBlock(bytes);
        CipherBlock(matrix);

        bytes to_bytes();
        string to_string();
        hexString to_hexString();

        void add_roundkey(matrix roundkey);

        void sub_bytes();

        void inv_sub_bytes();

        void shift_rows();

        void inv_shift_rows();

        void mix_columns();

        void inv_mix_columns();

        void encrypt(bytes master_key);

        void decrypt(bytes master_key);

} CipherBlock;

vector<bytes> split_2_blocks(bytes b);
bytes cipherblock_encrypt(bytes block, bytes master_key);
bytes cipherblock_decrypt(bytes block, bytes master_key);

#endif //AES_BLOCKCIPHER_LIB_H
