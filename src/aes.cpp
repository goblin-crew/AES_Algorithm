#include "aes_lib/aes.lib.h"

using namespace std;

enum Mode {
    ECB,
    CBC,
    PCBC,
    CFB,
    OFB
};

struct Args {
    Mode mode;
    bool encrypt;
    bytes key;
    bytes iv;
    bytes msg;

    Args(){
        this->encrypt = false;
    }

    void set_key(hexString val) {
        this->key = hexString_toBytes(val);
    }

    void set_iv(hexString val) {
        this->iv = hexString_toBytes(val);
    }

    void set_msg(hexString val) {
        this->msg = hexString_toBytes(val);
    }

    void set_mode(string val) {
        if(val == "ECB" || val == "ecb") {
            this->mode = ECB;
        }
        else if(val == "CBC" || val == "cbc") {
            this->mode = CBC;
        }
        else if(val == "PCBC" || val == "pcbc") {
            this->mode = PCBC;
        }
        else if(val == "CFB" || val == "cfb") {
            this->mode = CFB;
        }
        else if(val == "OFB" || val == "ofb") {
            this->mode = OFB;
        }
        else {
            cerr << "NO VALID MODE FOUND!!" << endl;
            exit;
        }
    }
};

int main(int argc, char* argv[]){
    Args args = Args();

    for (int i = 1; i < argc; ++i){
        if (string(argv[i]) == "encrypt"){
            args.encrypt = true;
        }
        else if (string(argv[i]) == "decrypt"){
            args.encrypt = false;
        }
        else if (string(argv[i]) == "--mode" || string(argv[i]) == "-m"){
            i += 1;
            args.set_mode(string(argv[i]));
        }
        else if (string(argv[i]) == "--key" || string(argv[i]) == "-k"){
            i += 1;
            args.set_key(string(argv[i]));
        }
        else if (string(argv[i]) == "--iv" || string(argv[i]) == "-i"){
            i += 1;
            args.set_iv(string(argv[i]));
        }
        else if (string(argv[i]) == "--message" || string(argv[i]) == "-msg"){
            i += 1;
            args.set_msg(string(argv[i]));
        }
    }

    bytes result;

    switch(args.mode)
    {
        case ECB:
            result = (args.encrypt)? AES::ECB::encrypt(args.msg, args.key) : AES::ECB::decrypt(args.msg, args.key);
            break;

        case CBC:
            result = (args.encrypt)? AES::CBC::encrypt(args.msg, args.key, args.iv) : AES::CBC::decrypt(args.msg, args.key, args.iv);
            break;

        case PCBC:
            result = (args.encrypt)? AES::PCBC::encrypt(args.msg, args.key, args.iv) : AES::PCBC::decrypt(args.msg, args.key, args.iv);
            break;

        case CFB:
            result = (args.encrypt)? AES::CFB::encrypt(args.msg, args.key, args.iv) : AES::CFB::decrypt(args.msg, args.key, args.iv);
            break;

        case OFB:
            result = (args.encrypt)? AES::OFB::encrypt(args.msg, args.key, args.iv) : AES::OFB::decrypt(args.msg, args.key, args.iv);
            break;
    }

    cout << "\n" << ((args.encrypt)? bytes_toHexString(result) : bytes_toString(result)) << "\n" << endl;

    return 0;
}

// key: 6162636465666768696A6B6C6D6E6F70
// IV: 7172737475767778797A626C61626C61
// Plaintext: 48656C6C6F20576F726C6421207468697320697320656E6372797074656420746578742E --> "Hello World! this is encrypted text."
