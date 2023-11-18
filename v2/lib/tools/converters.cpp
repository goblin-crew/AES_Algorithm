#include <bits/stdc++.h>

#include "../types/types.h"
#include "../constants/constants.h"
#include "converters.h"

using namespace std;

string bytes2string(ByteArray ba) {
    stringstream ss;

    for (auto it=ba.begin(); it != ba.end(); ++it){
        ss << char(*it);
    }

    return ss.str();
}

ByteArray string2bytes(string str) {
    ByteArray ba;

    for (int i=0; i < str.length(); ++i) {
        ba.push_back(uint8_t(str[i]));
    }

    return ba;
}


Matrix bytes2matrix(ByteArray ba) {
    Matrix mtx;

    for (int i = 0; i < ba.size(); i+=MATRIX_LEN){
        ByteArray row;

        for (int j=i; j < (i + MATRIX_LEN); ++j) {
            row.push_back(ba[j]);
        }

        mtx.push_back(row);
    }

    return mtx;
}

ByteArray matrix2bytes(Matrix mtx) {
    ByteArray ba;

    for (auto it=mtx.begin(); it != mtx.end(); ++it){
        for (auto jt=it->begin(); jt != it->end(); ++jt) {
            ba.push_back(*jt);
        }
    }

    return ba;
}


HexString bytes2hex(ByteArray ba) {
    stringstream hex_ss;

    for (auto it=ba.begin(); it != ba.end(); ++it) {
        hex_ss << std::setfill ('0') << std::setw(2) << std::hex << *it;
    }

    HexString hstr = hex_ss.str();

    return hstr;
}

ByteArray hex2bytes(HexString hstr) {
    ByteArray ba;

    for (int i=0; i < hstr.length(); i += 2){
        unsigned int x;
        stringstream hex_ss;
        stringstream num_ss;

        hex_ss << hstr[i] << hstr[i + 1];
        num_ss << std::hex << hex_ss.str();
        num_ss >> x;

        ba.push_back(static_cast<Byte>(x));
    }

    return ba;
}



HexString string2hex(string str) {
    return bytes2hex(string2bytes(str));
}

string hex2string(HexString hstr) {
    return bytes2string(hex2bytes(hstr));
}