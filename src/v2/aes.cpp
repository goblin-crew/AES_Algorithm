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

// key: 6162636465666768696A6B6C6D6E6F70
// IV: 7172737475767778797A626C61626C61
// Plaintext: 48656C6C6F20576F726C6421207468697320697320656E6372797074656420746578742E --> "Hello World! this is encrypted text."
