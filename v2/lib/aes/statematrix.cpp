#include <bits/stdc++.h>

#include <bits/stdc++.h>

#include "../types/types.h"
#include "../constants/constants.h"
#include "../tools/converters.h"

#include "key.h"
#include "statematrix.h"

using namespace std;

// private
Byte StateMatrix::xtime(Byte a) {
    return Byte((a & 0x80) ? (((a << 1) ^ 0x1B) & 0xFF) : (a << 1));
}

// public
StateMatrix::StateMatrix(Matrix mtx) {
    this->matrix = mtx;
}

StateMatrix::StateMatrix(ByteArray b) {
    this->matrix = bytes2matrix(b);
}


ByteArray StateMatrix::as_ByteArray() {
    return matrix2bytes(this->matrix);
}

string StateMatrix::as_String() {
    return bytes2string(matrix2bytes(this->matrix));
}

HexString StateMatrix::as_HexString() {
    return bytes2hex(matrix2bytes(this->matrix));
}



void StateMatrix::add_roundkey(RoundKey roundkey) {
    for (int i = 0; i < MATRIX_LEN; ++i)
    {
        for (int j = 0; j < MATRIX_LEN; ++j)
        {
            this->matrix[i][j] ^= roundkey[i][j];
        }
    }
}

void StateMatrix::sub_bytes() {
    for (auto it=this->matrix.begin(); it != this->matrix.end(); ++it)
    {
        for (auto jt=it->begin(); jt != it->end(); ++jt)
        {
            *jt = S_BOX[*jt];
        }
    }
}

void StateMatrix::inv_sub_bytes() {
    for (auto it=this->matrix.begin(); it != this->matrix.end(); ++it)
    {
        for (auto jt=it->begin(); jt != it->end(); ++jt)
        {
            *jt = INV_S_BOX[*jt];
        }
    }
}

void StateMatrix::shift_rows() {
    Matrix tmp_matrix;

    for (int i=0; i < MATRIX_LEN; ++i)
    {
        ByteArray row;

        for (int j=0; j < MATRIX_LEN; ++j)
        {
            row.push_back(this->matrix[int((MATRIX_LEN + i + j) % MATRIX_LEN)][j]);
        }

        tmp_matrix.push_back(row);
    }

    this->matrix = tmp_matrix;
}

void StateMatrix::inv_shift_rows() {
    Matrix tmp_matrix;

    for (int i=0; i < MATRIX_LEN; ++i)
    {
        ByteArray row;

        for (int j=0; j < MATRIX_LEN; ++j)
        {
            row.push_back(this->matrix[int((MATRIX_LEN + i - j) % MATRIX_LEN)][j]);
        }

        tmp_matrix.push_back(row);
    }

    this->matrix = tmp_matrix;
}

void StateMatrix::mix_columns() {
    for (auto it=this->matrix.begin(); it != this->matrix.end(); ++it) 
    {
        auto jt = it->begin();

        Byte t = *jt ^ *(jt + 1) ^ *(jt + 2) ^ *(jt + 3);
        Byte u = *jt;

        for (int i=0; i < MATRIX_LEN; ++i)
        {
            *(jt + i) ^= t ^ this->xtime(*(jt + i) ^ (((i + 1) == MATRIX_LEN)? u : *(jt + (i + 1))));
        }
    }
}

void StateMatrix::inv_mix_columns() {
    for (auto it=this->matrix.begin(); it != this->matrix.end(); ++it) 
    {
        auto jt = it->begin();

        Byte u = this->xtime(this->xtime(*(jt + 0) ^ *(jt + 2)));
        Byte v = this->xtime(this->xtime(*(jt + 1) ^ *(jt + 3)));

        for (int i=0; i < MATRIX_LEN; ++i)
        {
            *(jt + i) ^= (((i % 2) == 0)? u : v);
        }
    }

    this->mix_columns();
}

void StateMatrix::encrypt(MasterKey master_key) {
    Key key = Key(master_key);

    this->add_roundkey(key.round(0));

    for (int i = 1; i <= N_ROUNDS; ++i) 
    {
        this->sub_bytes();
        this->shift_rows();

        if (i < N_ROUNDS) {
            this->mix_columns();
        }

        this->add_roundkey(key.round(i));
    }
}

void StateMatrix::decrypt(MasterKey master_key) {
    Key key = Key(master_key);

    this->add_roundkey(key.round(N_ROUNDS));

    for (int i=(N_ROUNDS - 1); i >= 0; i -= 1) 
    {
        this->inv_shift_rows();
        this->inv_sub_bytes();

        this->add_roundkey(key.round(i));

        if (i > 0) {
            this->inv_mix_columns();
        }
    }
}