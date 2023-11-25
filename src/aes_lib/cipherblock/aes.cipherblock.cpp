#include "aes.cipherblock.h"

using namespace std;

//--------------------------------------------------------------------------------------


void RoundKey::expand(bytes b) {
    /*
        Expands and returns a list of key matrices for the given master_key.
    */

    matrix key_columns = bytes_toMatrix(b);
    int iteration_size = int((b.size() - (b.size() % MATRIX_LEN)) / MATRIX_LEN);  //integral_division(master_key.size(), MATRIX_LEN);

    int i = 1;
    while(key_columns.size() < ((N_ROUNDS + 1) * MATRIX_LEN)) 
    {
        bytes word = key_columns[int(key_columns.size() - 1)];

        if ((key_columns.size() % iteration_size) == 0) 
        {
            // Circular shift
            word.push_back(word.front()); word.erase(word.begin());

            // map to S-Box
            for (auto it=word.begin(); it != word.end(); ++it){
                *it = S_BOX[*it];
            }

            // XOR with first byte of R-CON, since the others bytes of R-CON are 0.
            word[0] ^= R_CON[i];
            i += 1;
        }
        else if(b.size() == 32 && (key_columns.size() % iteration_size) == MATRIX_LEN) 
        {
            /* 
                Run word through S-box in the fourth iteration when using a
                256-bit key.
            */

            for (auto it=word.begin(); it != word.end(); ++it){
                *it = S_BOX[*it];
            }
        }

        // XOR with equivalent word from previous iteration
        for (int j = 0; j < word.size(); ++j)
        {
            word[j] ^= key_columns[(key_columns.size() - iteration_size)][j];
        }

        key_columns.push_back(word);
    }

    // Group key words in 4x4 byte matrices
    for(int j=0; j < int((key_columns.size() - (key_columns.size() % MATRIX_LEN)) / MATRIX_LEN); ++j)
    {
        matrix rk;
        for(int x=(4 * j); x < (4 * (j + 1)); ++x) 
        {
            rk.push_back(key_columns[x]);
        }

        this->round_keys.push_back(rk);
    }
}

RoundKey::RoundKey(bytes b){
    this->expand(b);
}

vector<matrix>::iterator RoundKey::end(){
    return this->round_keys.end();
}

vector<matrix>::iterator RoundKey::begin(){
    return this->round_keys.begin();
}

matrix RoundKey::operator [] (int i){
    this->it = this->round_keys.begin() + i;
    return *this->it;
}

void RoundKey::operator ++ (){
    ++this->it;
}

void RoundKey::operator -- (){
    --this->it;
}


//--------------------------------------------------------------------------------------


Byte CipherBlock::xtime(Byte a) {
    return Byte((a & 0x80) ? (((a << 1) ^ 0x1B) & 0xFF) : (a << 1));
}

CipherBlock::CipherBlock(matrix mtx) {
    this->value = mtx;
}

CipherBlock::CipherBlock(bytes b) {
    this->value = bytes_toMatrix(b);
}

bytes CipherBlock::to_bytes(){
    return matrix_toBytes(this->value);
}

string CipherBlock::to_string(){
    return matrix_toString(this->value);
}

hexString CipherBlock::to_hexString(){
    return matrix_toHexString(this->value);
}

void CipherBlock::add_roundkey(matrix roundkey) {
    for (int i = 0; i < MATRIX_LEN; ++i)
    {
        for (int j = 0; j < MATRIX_LEN; ++j)
        {
            this->value[i][j] ^= roundkey[i][j];
        }
    }
}

void CipherBlock::sub_bytes() {
    for (auto it=this->value.begin(); it != this->value.end(); ++it)
    {
        for (auto jt=it->begin(); jt != it->end(); ++jt)
        {
            *jt = S_BOX[*jt];
        }
    }
}

void CipherBlock::inv_sub_bytes() {
    for (auto it=this->value.begin(); it != this->value.end(); ++it)
    {
        for (auto jt=it->begin(); jt != it->end(); ++jt)
        {
            *jt = INV_S_BOX[*jt];
        }
    }
}

void CipherBlock::shift_rows() {
    matrix tmp_matrix;

    for (int i=0; i < MATRIX_LEN; ++i)
    {
        bytes row;

        for (int j=0; j < MATRIX_LEN; ++j)
        {
            row.push_back(this->value[int((MATRIX_LEN + i + j) % MATRIX_LEN)][j]);
        }

        tmp_matrix.push_back(row);
    }

    this->value = tmp_matrix;
}

void CipherBlock::inv_shift_rows() {
    matrix tmp_matrix;

    for (int i=0; i < MATRIX_LEN; ++i)
    {
        bytes row;

        for (int j=0; j < MATRIX_LEN; ++j)
        {
            row.push_back(this->value[int((MATRIX_LEN + i - j) % MATRIX_LEN)][j]);
        }

        tmp_matrix.push_back(row);
    }

    this->value = tmp_matrix;
}

void CipherBlock::mix_columns() {
    for (auto it=this->value.begin(); it != this->value.end(); ++it) 
    {
        auto jt = it->begin();

        Byte t = *jt ^ *(jt + 1) ^ *(jt + 2) ^ *(jt + 3);
        Byte u = *jt;

        for (int i=0; i < MATRIX_LEN; ++i)
        {
            *(jt + i) ^= t ^ this->xtime(*(jt + i) ^ (((i + 1) == MATRIX_LEN)? u : *(jt + (i + 1))));
        }
    }
}

void CipherBlock::inv_mix_columns() {
    for (auto it=this->value.begin(); it != this->value.end(); ++it) 
    {
        auto jt = it->begin();

        Byte u = this->xtime(this->xtime(*(jt + 0) ^ *(jt + 2)));
        Byte v = this->xtime(this->xtime(*(jt + 1) ^ *(jt + 3)));

        for (int i=0; i < MATRIX_LEN; ++i)
        {
            *(jt + i) ^= (((i % 2) == 0)? u : v);
        }
    }

    this->mix_columns();
}

void CipherBlock::encrypt(bytes master_key) {
    RoundKey roundkey = RoundKey(master_key);
    this->add_roundkey(roundkey[0]);

    for (int i=0; i < N_ROUNDS; ++i)
    {
        ++roundkey;

        this->sub_bytes();
        this->shift_rows();

        if (i < (N_ROUNDS - 1)){
            this->mix_columns();
        }

        this->add_roundkey(*roundkey.it);
    }
}

void CipherBlock::decrypt(bytes master_key) {
    RoundKey roundkey = RoundKey(master_key);

    this->add_roundkey(roundkey[N_ROUNDS]);

    for (int i=0; i < N_ROUNDS; ++i)
    {
        --roundkey;

        this->inv_shift_rows();
        this->inv_sub_bytes();

        this->add_roundkey(*roundkey.it);

        if (i < (N_ROUNDS - 1)){
            this->inv_mix_columns();
        }
    }
}


//---------------------------------------------------------------------------


vector<bytes> split_2_blocks(bytes b) {
    vector<bytes> blocks;

    int remainder = b.size() % KEY_LENGHT;
    int full_blocks = (b.size() - remainder) / KEY_LENGHT;

    for (int i=0; i < full_blocks; ++i)
    {
        bytes block;
        
        for (int j=(i * KEY_LENGHT); j < ((i * KEY_LENGHT) + KEY_LENGHT); ++j)
        {
            block.push_back(b[j]);
        }

        blocks.push_back(block);
    }

    if (remainder > 0) {
        bytes remainder_block;

        for (int i=(b.size() - remainder); i < b.size(); ++i)
        {
            remainder_block.push_back(b[i]);
        }

        for (int i=0; i < (KEY_LENGHT - remainder); ++i) 
        {
            remainder_block.push_back(Byte(0x00));
        }

        blocks.push_back(remainder_block);
    }
    

    return blocks;
}

bytes cipherblock_encrypt(bytes block, bytes master_key){
    CipherBlock cb = CipherBlock(block);
    cb.encrypt(master_key);
    return cb.to_bytes();
}

bytes cipherblock_decrypt(bytes block, bytes master_key){
    CipherBlock cb = CipherBlock(block);
    cb.decrypt(master_key);
    return cb.to_bytes();
}