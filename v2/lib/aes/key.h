#ifndef KEYS_H
#define KEYS_H

#include <bits/stdc++.h>

#include "../types/types.h"
#include "../constants/constants.h"
#include "../tools/converters.h"

using namespace std;

class Key {
    private:
        MasterKey master_key;
        RoundKeys round_keys;

        void expand();
        void generate();

    public:
        Key();
        Key(MasterKey master_key);

        RoundKey round(int round);
        MasterKey get();
};

#endif //KEYS_H
