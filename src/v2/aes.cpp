#include "aes_lib/aes.lib.h"

using namespace std;

int main(int argc, char* argv[]){
    hexString key_str = string(argv[1]);
    hexString iv_str = string(argv[2]);
    hexString msg_str = string(argv[3]);

    bytes key = toBytes(key_str);
    bytes iv = toBytes(iv_str);
    bytes msg = toBytes(msg_str);

    msg = AES::CBC::encrypt(msg, key, iv);

    cout << "\n" << toHexString(msg) << "\n";

    return 0;
}