#include "aes_lib/aes.lib.h"

using namespace std;

int main(int argc, char* argv[]){
    bytes key = hexString_toBytes(string(argv[1]));
    bytes iv = hexString_toBytes(string(argv[2]));
    bytes msg = hexString_toBytes(string(argv[3]));

    msg = AES::CBC::encrypt(msg, key, iv);

    cout << "\n" << bytes_toHexString(msg) << "\n";

    return 0;
}