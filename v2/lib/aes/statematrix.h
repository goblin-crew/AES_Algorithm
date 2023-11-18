#ifndef STATEMATRIX_H
#define STATEMATRIX_H

#include <bits/stdc++.h>

#include "../types/types.h"
#include "../constants/constants.h"
#include "../tools/converters.h"

#include "key.h"

using namespace std;

class StateMatrix {
    private:
        Byte xtime(Byte a);

    public:
        Matrix matrix;
        
        StateMatrix(Matrix mtx);
        StateMatrix(ByteArray b);

        ByteArray as_ByteArray();
        string as_String();
        HexString as_HexString();

        void add_roundkey(RoundKey roundkey);

        void sub_bytes();

        void inv_sub_bytes();

        void shift_rows();

        void inv_shift_rows();

        void mix_columns();

        void inv_mix_columns();

        void encrypt(MasterKey master_key);

        void decrypt(MasterKey master_key);
};

#endif //STATEMATRIX_H
