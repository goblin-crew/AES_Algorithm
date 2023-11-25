#ifndef AES_TYPES_H
#define AES_TYPES_H

#include<bits/stdc++.h>

using namespace std;


typedef uint8_t Byte;
typedef vector<Byte> bytes;
typedef vector<bytes> matrix;
typedef string hexString;

bytes toBytes(matrix);
bytes toBytes(hexString);
bytes toBytes(string);

matrix toMatrix(bytes);
matrix toMatrix(hexString);
matrix toMatrix(string);

hexString toHexString(bytes);
hexString toHexString(matrix);
hexString toHexString(string);

string toString(bytes);
string toString(matrix);
string toString(hexString);

#endif //AES_TYPES_LIB_H
