#ifndef AES_TYPES_H
#define AES_TYPES_H

#include<bits/stdc++.h>

using namespace std;


typedef uint8_t Byte;
typedef vector<Byte> bytes;
typedef vector<bytes> matrix;
typedef string hexString;

bytes matrix_toBytes(matrix);
bytes string_toBytes(string);
bytes hexString_toBytes(hexString);

bytes bytes_xor(bytes a, bytes b);

matrix bytes_toMatrix(bytes);
matrix hexString_toMatrix(hexString);
matrix string_toMatrix(string);

hexString bytes_toHexString(bytes);
hexString matrix_toHexString(matrix);
hexString string_toHexString(string);

string bytes_toString(bytes);
string matrix_toString(matrix);
string hexString_toString(hexString);

#endif //AES_TYPES_LIB_H
