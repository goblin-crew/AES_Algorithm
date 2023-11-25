#ifndef AES_MODES_H
#define AES_MODES_H

#include <bits/stdc++.h>
#include "../aes.cipherblock.lib.h"

using namespace std;

namespace AES 
{
    namespace ECB
    {
        bytes encrypt(bytes value, bytes key);
        bytes decrypt(bytes value, bytes key);
    }

    namespace CBC
    {
        bytes encrypt(bytes value, bytes key, bytes iv);
        bytes decrypt(bytes value, bytes key, bytes iv);
    }

    namespace PCBC
    {
        bytes encrypt(bytes value, bytes key, bytes iv);
        bytes decrypt(bytes value, bytes key, bytes iv);
    }

    namespace CFB
    {
        bytes encrypt(bytes value, bytes key, bytes iv);
        bytes decrypt(bytes value, bytes key, bytes iv);
    }

    namespace OFB
    {
        bytes encrypt(bytes value, bytes key, bytes iv);
        bytes decrypt(bytes value, bytes key, bytes iv);
    }
}

#endif //AES_MODES_H
