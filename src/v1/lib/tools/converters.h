#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <bits/stdc++.h>
#include "../types/types.h"
#include "../constants/constants.h"

using namespace std;

string bytes2string(ByteArray ba);
ByteArray string2bytes(string str);

Matrix bytes2matrix(ByteArray ba);
ByteArray matrix2bytes(Matrix mtx);

HexString bytes2hex(ByteArray ba);
ByteArray hex2bytes(HexString hstr);

HexString string2hex(string str);
string hex2string(HexString hstr);

#endif //CONVERTERS_H
