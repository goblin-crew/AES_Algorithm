// only implemented AES-128 --> 16byte key

#include <bits/stdc++.h>
#include <random>

#define MTX_LEN 4
#define KEY_LENGHT 16
#define N_ROUNDS 10

using namespace std;

typedef vector<vector<int>> Matrix;
typedef vector<int> Bytes;

string bytes2str(Bytes key){
    stringstream key_ss;

    for(auto it=key.begin(); it != key.end(); ++it){
        key_ss << char(*it);
    }

    return key_ss.str();
}

Bytes string2bytes(string str){
    Bytes b;

    for (int i = 0; i < str.length(); ++i){
        b.push_back(int(str[i]));
    }

    return b;
}

Matrix bytes2matrix(Bytes bytes) {
    Matrix mtx;
    for (int i = 0; i < bytes.size(); i+=MTX_LEN){
        vector<int> row;
        for (int j=i; j < (i + MTX_LEN); ++j) {
            row.push_back(bytes[j]);
        }
        mtx.push_back(row);
    }
    return mtx;
}

Bytes matrix2bytes(Matrix matrix) {
    Bytes b;
    for (auto it=matrix.begin(); it != matrix.end(); ++it){
        for (auto jt=it->begin(); jt != it->end(); ++jt) {
            b.push_back(*jt);
        }
    }
    return b;
}

void print_matrix(Matrix mtx){
    for(auto it=mtx.begin(); it != mtx.end(); it++){
        for(auto jt=it->begin(); jt != it->end(); jt++){
            cout << *jt << " ";
        }

        cout << endl;
    }

    cout << endl;
}

int integral_division(int a, int b) {
    return int((a - (a % b)) / b);
}

Bytes hex2bytes(string hex_str) {
    Bytes b;

    for (int i=0; i < hex_str.length(); i += 2){
        unsigned int x;
        stringstream hex_ss;
        stringstream num_ss;

        hex_ss << hex_str[i] << hex_str[i + 1];
        num_ss << std::hex << hex_ss.str();
        num_ss >> x;

        b.push_back(static_cast<int>(x));
    }

    return b;
}

string bytes2hex(Bytes b){
    stringstream hex_ss;

    for (auto it=b.begin(); it != b.end(); ++it) {
        hex_ss << std::setfill ('0') << std::setw(2) << std::hex << *it;
    }

    return hex_ss.str();
}

namespace AES {
    /*
        These S-Boxes are almost always the same, 
        it would be no problem to use other S-Boxes, 
        but maybee they would be more insecure,
        since the "standard" S-Boxes where sientifically developed to provide the securest possible S-Box
    */

    const Bytes S_BOX{
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
    };

    const Bytes INV_S_BOX{
        0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
        0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
        0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
        0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
        0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
        0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
        0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
        0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
        0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
        0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
        0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
        0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
        0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
        0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
        0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
    };


    Bytes generate_key() {
        Bytes k;

        for (int i=0; i < KEY_LENGHT; ++i){
            // generate random number
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(33, 126);

            k.push_back(distr(gen));
        }

        return k;
    }

    class StateMatrix {
        private:
            int xtime(int a) {
                if (a & 0x80) {
                    return (((a << 1) ^ 0x1B) & 0xFF);
                }
                else {
                    return (a << 1);
                }
            }

        public:
            Matrix matrix;
            
            StateMatrix(Matrix mtx){
                this->matrix = mtx;
            }

            void add_roundkey(Matrix roundkey){
                for (int i = 0; i < MTX_LEN; ++i){
                    for (int j = 0; j < MTX_LEN; ++j){
                        this->matrix[i][j] ^= roundkey[i][j];
                    }
                }
            }

            void sub_bytes() {
                for (int i = 0; i < MTX_LEN; ++i){
                    for (int j = 0; j < MTX_LEN; ++j) {
                        this->matrix[i][j] = S_BOX[this->matrix[i][j]];
                    }
                }
            }

            void inv_sub_bytes() {
                for (int i = 0; i < MTX_LEN; ++i){
                    for (int j = 0; j < MTX_LEN; ++j) {
                        this->matrix[i][j] = INV_S_BOX[this->matrix[i][j]];
                    }
                }
            }

            void shift_rows() {
                Bytes r0{this->matrix[0][0], this->matrix[1][1], this->matrix[2][2], this->matrix[3][3]};
                Bytes r1{this->matrix[1][0], this->matrix[2][1], this->matrix[3][2], this->matrix[0][3]};
                Bytes r2{this->matrix[2][0], this->matrix[3][1], this->matrix[0][2], this->matrix[1][3]};
                Bytes r3{this->matrix[3][0], this->matrix[0][1], this->matrix[1][2], this->matrix[2][3]};

                this->matrix[0] = r0;
                this->matrix[1] = r1;
                this->matrix[2] = r2;
                this->matrix[3] = r3;
            }

            void inv_shift_rows() {
                Bytes r0{this->matrix[0][0], this->matrix[3][1], this->matrix[2][2], this->matrix[1][3]};
                Bytes r1{this->matrix[1][0], this->matrix[0][1], this->matrix[3][2], this->matrix[2][3]};
                Bytes r2{this->matrix[2][0], this->matrix[1][1], this->matrix[0][2], this->matrix[3][3]};
                Bytes r3{this->matrix[3][0], this->matrix[2][1], this->matrix[1][2], this->matrix[0][3]};

                this->matrix[0] = r0;
                this->matrix[1] = r1;
                this->matrix[2] = r2;
                this->matrix[3] = r3;
            }

            void mix_columns(){
                for (int i=0; i < MTX_LEN; ++i){
                    int t = this->matrix[i][0] ^ this->matrix[i][1] ^ this->matrix[i][2] ^ this->matrix[i][3];
                    int u = this->matrix[i][0];

                    //cout << "t: " << t << " | " << "u:" << u << endl;

                    this->matrix[i][0] ^= t ^ this->xtime(this->matrix[i][0] ^ this->matrix[i][1]);
                    this->matrix[i][1] ^= t ^ this->xtime(this->matrix[i][1] ^ this->matrix[i][2]);
                    this->matrix[i][2] ^= t ^ this->xtime(this->matrix[i][2] ^ this->matrix[i][3]);
                    this->matrix[i][3] ^= t ^ this->xtime(this->matrix[i][3] ^ u);
                }

            }

            void inv_mix_columns(){
                for (int i=0; i < MTX_LEN; ++i){
                    int u = this->xtime(this->xtime(this->matrix[i][0] ^ this->matrix[i][2]));
                    int v = this->xtime(this->xtime(this->matrix[i][1] ^ this->matrix[i][3]));

                    this->matrix[i][0] ^= u;
                    this->matrix[i][1] ^= v;
                    this->matrix[i][2] ^= u;
                    this->matrix[i][3] ^= v;
                }

                this->mix_columns();
            }
    };


    vector<Matrix> expand_key(Bytes master_key) {
        /*
            Expands and returns a list of key matrices for the given master_key.
        */

       // Round Constants https://en.wikipedia.org/wiki/AES_key_schedule#Round_constants
        const Bytes R_CON{
            0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
            0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
            0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
            0xD4, 0xB3, 0x7D, 0xFA, 0xEF, 0xC5, 0x91, 0x39
        };

        Matrix key_columns = bytes2matrix(master_key);
        int iteration_size = integral_division(master_key.size(), MTX_LEN);

        int i = 1;
        while(key_columns.size() < ((N_ROUNDS + 1) * MTX_LEN)) {
            Bytes word = key_columns[int(key_columns.size() - 1)];

            if ((key_columns.size() % iteration_size) == 0) {
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
            else if(master_key.size() == 32 && (key_columns.size() % iteration_size) == MTX_LEN) {
                /* 
                    Run word through S-box in the fourth iteration when using a
                    256-bit key.
                */

                for (auto it=word.begin(); it != word.end(); ++it){
                    *it = S_BOX[*it];
                }
            }

            // XOR with equivalent word from previous iteration
            for (int j = 0; j < word.size(); ++j){
                word[j] ^= key_columns[(key_columns.size() - iteration_size)][j];
            }

            key_columns.push_back(word);
        }

        // Group key words in 4x4 byte matrices
        vector<Matrix> k_groups;
        for(int j=0; j < integral_division(key_columns.size(), MTX_LEN); ++j){
            Matrix k_m;
            for(int x=(4 * j); x < (4 * (j + 1)); ++x) {
                k_m.push_back(key_columns[x]);
            }

            k_groups.push_back(k_m);
        }

        return k_groups;
    }

    Bytes decrypt(Bytes key, Bytes val){
        vector<Matrix> round_keys = expand_key(key);
        StateMatrix state = StateMatrix(bytes2matrix(val));
        
        state.add_roundkey(round_keys[N_ROUNDS]);

        for (int round=(N_ROUNDS - 1); round >= 0; round -= 1) {
            state.inv_shift_rows();
            state.inv_sub_bytes();
            state.add_roundkey(round_keys[round]);
            
            if (round > 0) {
                state.inv_mix_columns();
            }
        }

        return matrix2bytes(state.matrix);
    }

    Bytes encrypt(Bytes key, Bytes val) {
        vector<Matrix> round_keys = expand_key(key);
        StateMatrix state = StateMatrix(bytes2matrix(val));

        state.add_roundkey(round_keys[0]);

        for (int round = 1; round <= N_ROUNDS; ++round) {
            state.sub_bytes();
            state.shift_rows();

            if (round < N_ROUNDS) {
                state.mix_columns();
            }
            
            state.add_roundkey(round_keys[round]);
        }

        return matrix2bytes(state.matrix);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        map<string, string> args;
        bool encrypt = false;
        bool decrypt = false;
        bool hasKey = false;
        bool is_plain_text = false;
        bool out_readable = false;

        for (int i=0; i < argc; ++i) {
            if (string(argv[i]) == "-e" || string(argv[i]) == "--encrypt") {
                encrypt = true;
                i += 1;
                string val(argv[i]);
                args.insert(make_pair("value", val));
            }
            else if (string(argv[i]) == "-d" || string(argv[i]) == "--decrypt") {
                decrypt = true;
                i += 1;
                string val(argv[i]);
                args.insert(make_pair("value", val));
            }
            else if (string(argv[i]) == "-k" || string(argv[i]) == "--key") {
                hasKey = true;
                i += 1;
                string val(argv[i]);
                args.insert(make_pair("key", val));
            }
            else if (string(argv[i]) == "-p" || string(argv[i]) == "--plainText") {
                is_plain_text = true;
            }
            else if (string(argv[i]) == "-r" || string(argv[i]) == "--readable") {
                out_readable = true;
            }
        }

        if (encrypt) {
            if (!hasKey) {
                args.insert(make_pair("key", bytes2hex(AES::generate_key())));
            }

            if (is_plain_text) {
                args["value"] = bytes2hex(string2bytes(args["value"]));
            }

            Bytes key = hex2bytes(args["key"]);
            Bytes value = hex2bytes(args["value"]);

            cout << "\n ENCRYPTED\n---------------" << endl;
            cout << bytes2hex(AES::encrypt(key, value)) << endl;
            cout << endl;
            cout << "KEY: " << args["key"] << endl;
            cout << endl;
        }
        else if (decrypt) {
            if (!hasKey) {
                cout << endl;
                cout << "No Key provided!" << endl;
                cout << endl;
            }
            else {
                if (is_plain_text) {
                    args["value"] = bytes2hex(string2bytes(args["value"]));
                }

                Bytes key = hex2bytes(args["key"]);
                Bytes value = hex2bytes(args["value"]);

                cout << "\n DECRYPTED\n---------------" << endl;

                if (out_readable) {
                    cout << bytes2str(AES::decrypt(key, value)) << endl;
                }
                else {
                    cout << bytes2hex(AES::decrypt(key, value)) << endl;
                }
                
                cout << endl;
                cout << "KEY: " << args["key"] << endl;
                cout << endl;
            }
        }
    }
    else {
        cout << endl;
        cout << "No arguments submitted!" << endl;
        cout << endl;
    }
    
    return 0;

}