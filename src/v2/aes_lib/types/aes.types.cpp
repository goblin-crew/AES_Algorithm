#include "aes.types.h"

using namespace std;

//----------------------------------------------------------------------------------

bytes toBytes(matrix mtx)
{
    bytes b;

    for(int i=0; i < mtx.size(); ++i)
    {
        copy(mtx[i].begin(), mtx[i].end(), back_inserter(b));
    }

    return b;
}

bytes toBytes(hexString hstr){
    bytes ba;

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

bytes toBytes(string str){
    bytes ba;

    for (int i=0; i < str.length(); ++i) {
        ba.push_back(uint8_t(str[i]));
    }

    return ba;
}

bytes bytes_xor(bytes a, bytes b){
    bytes x;

    for (int i=0; i < a.size() && i < b.size(); ++i){
        x.push_back(a[i] ^ b[i]);
    }

    return x;
}

//----------------------------------------------------------------------------------

matrix toMatrix(bytes ba){
    matrix mtx;

    for (int i = 0; i < ba.size(); i+=MATRIX_LEN)
    {
        bytes row;

        for (int j=i; j < (i + MATRIX_LEN); ++j) 
        {
            row.push_back(ba[j]);
        }

        mtx.push_back(row);
    }

    return mtx;
}

matrix toMatrix(hexString hstr){
    return toMatrix(toBytes(hstr));
}

matrix toMatrix(string str){
    return toMatrix(toBytes(str));
}

//----------------------------------------------------------------------------------

hexString toHexString(bytes ba){
    stringstream hex_ss;

    for (auto it=ba.begin(); it != ba.end(); ++it) 
    {
        hex_ss << std::setfill('0') << std::setw(2) << std::hex << int(*it);
    }

    hexString hstr = hex_ss.str();

    return hstr;
}

hexString toHexString(matrix mtx){
    return toHexString(toBytes(mtx));
}

hexString toHexString(string str){
    return toHexString(toBytes(str));
}

//----------------------------------------------------------------------------------

string toString(bytes ba){
    stringstream ss;

    for (auto it=ba.begin(); it != ba.end(); ++it)
    {
        ss << char(*it);
    }

    return ss.str();
}

string toString(matrix mtx){
    return toString(toBytes(mtx));
}

string toString(hexString hstr){
    return toString(toBytes(hstr));
}
