#include "aes.types.h"
#include <fstream>

using namespace std;

//----------------------------------------------------------------------------------

bytes matrix_toBytes(matrix mtx)
{
    bytes b;

    for(int i=0; i < mtx.size(); ++i)
    {
        copy(mtx[i].begin(), mtx[i].end(), back_inserter(b));
    }

    return b;
}

bytes hexString_toBytes(hexString hstr){
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

bytes string_toBytes(string str){
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

bytes read_file_bytes(string path) {
    char* file_path = new char[path.length() + 1]; 
    strcpy(file_path, path.c_str()); 

    std::ifstream file(file_path, std::ios::binary);

    file.unsetf(std::ios::skipws);
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    bytes b;
    b.reserve(fileSize);
    b.insert(b.begin(), std::istream_iterator<Byte>(file), std::istream_iterator<Byte>());

    return b;
}

void write_file_bytes(string path, bytes b){
    char* file_path = new char[path.length() + 1]; 
    strcpy(file_path, path.c_str());

    const std::vector<uint8_t>  buffer(b); // let's assume that i'ts filled with values
    std::ofstream out(file_path, std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    out.close();
}

//----------------------------------------------------------------------------------

matrix bytes_toMatrix(bytes ba){
    matrix mtx;

    for (int i = 0; i < ba.size(); i+=4)
    {
        bytes row;

        for (int j=i; j < (i + 4); ++j) 
        {
            row.push_back(ba[j]);
        }

        mtx.push_back(row);
    }

    return mtx;
}

matrix hexString_toMatrix(hexString hstr){
    return bytes_toMatrix(hexString_toBytes(hstr));
}

matrix string_toMatrix(string str){
    return bytes_toMatrix(string_toBytes(str));
}

//----------------------------------------------------------------------------------

hexString bytes_toHexString(bytes ba){
    stringstream hex_ss;

    for (auto it=ba.begin(); it != ba.end(); ++it) 
    {
        hex_ss << std::setfill('0') << std::setw(2) << std::hex << int(*it);
    }

    hexString hstr = hex_ss.str();

    return hstr;
}

hexString matrix_toHexString(matrix mtx){
    return bytes_toHexString(matrix_toBytes(mtx));
}

hexString string_toHexString(string str){
    return bytes_toHexString(string_toBytes(str));
}

//----------------------------------------------------------------------------------

string bytes_toString(bytes ba){
    stringstream ss;

    for (auto it=ba.begin(); it != ba.end(); ++it)
    {
        ss << char(*it);
    }

    return ss.str();
}

string matrix_toString(matrix mtx){
    return bytes_toString(matrix_toBytes(mtx));
}

string hexString_toString(hexString hstr){
    return bytes_toString(hexString_toBytes(hstr));
}
