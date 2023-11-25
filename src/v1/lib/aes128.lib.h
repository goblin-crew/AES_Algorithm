# ifndef AES128_LIB_H
# define AES128_LIB_H

#include <bits/stdc++.h>

#include "types/types.h"
#include "constants/constants.h"
#include "tools/converters.h"
#include "aes/key.h"
#include "aes/statematrix.h"

using namespace std;

vector<ByteArray> get_blocks(ByteArray b);
ByteArray aes128ecb_encrypt(MasterKey master_key, ByteArray msg);
ByteArray aes128ecb_decrypt(MasterKey master_key, ByteArray ciphertext);

# endif