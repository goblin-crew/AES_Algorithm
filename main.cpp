# include "lib/aes128.lib.h"

using namespace std;

int main(int argc, char* argv[]) {
    ByteArray result;

    if (string(argv[1]) == "encrypt") 
    {
        if (argc > 2) {
            MasterKey key;
            ByteArray bytes;

            for(int i = 2; i < argc; ++i)
            {
                if (string(argv[i]) == "-p" || string(argv[i]) == "--plain")
                {
                    i += 1;
                    bytes = string2bytes(string(argv[i]));
                }
                else if (string(argv[i]) == "-b" || string(argv[i]) == "--bytes")
                {
                    i += 1;
                    bytes = hex2bytes(string(argv[i]));
                }
                else if (string(argv[i]) == "-k" || string(argv[i]) == "--key")
                {
                    i += 1;
                    key = hex2bytes(string(argv[i]));
                }
            }

            result = aes128ecb_encrypt(key, bytes);

            cout << endl;
            cout << bytes2hex(result) << endl;
            cout << endl;
        }
    }
    else if (string(argv[1]) == "decrypt") 
    {
        if (argc > 2) {
            MasterKey key;
            ByteArray bytes;
            bool out_hex = false;

            for(int i = 2; i < argc; ++i)
            {
                if (string(argv[i]) == "-b" || string(argv[i]) == "--bytes")
                {
                    i += 1;
                    bytes = hex2bytes(string(argv[i]));
                }
                else if (string(argv[i]) == "-k" || string(argv[i]) == "--key")
                {
                    i += 1;
                    key = hex2bytes(string(argv[i]));
                }
                else if (string(argv[i]) == "-h" || string(argv[i]) == "--out-hex")
                {
                    out_hex = true;
                }
            }

            result = aes128ecb_decrypt(key, bytes);

            cout << endl;
            if (out_hex) 
            {
                cout << bytes2hex(result) << endl;
            }
            else
            {
                cout << bytes2string(result) << endl;
            }
            cout << endl;
        }
    }
    else 
    {
        cout << "No valid operation found!!";
    }

    return 0;

    /*
    clear text: crypto{MYAES128}
    key: c32c5ca6b5805e0cdb8da57a2ab6fe5c
    ciphertext: d14f146aa42b4fb6a1c40842298f12dd
    */
}