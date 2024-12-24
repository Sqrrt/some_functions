#include <stdexcept>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <bitset>
#include <iostream>
#include "sha3.h"
using namespace std;

vector<vector<vector<int>>> theta(vector<vector<vector<int>>> A, int w) {
    vector<vector<int>> C(5, vector<int>(w, 0));
    vector<vector<int>> D(5, vector<int>(w, 0));
    for (int i = 0; i < 5; i++) {
        for (int k = 0; k < w; k++) {
            C[i][k] = A[i][0][k] ^ A[i][1][k] ^ A[i][2][k] ^ A[i][3][k] ^ A[i][4][k];
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int k = 0; k < w; k++) {
            D[i][k] = C[(i + 4) % 5][k] ^ C[(i + 1) % 5][(k + w - 1) % w];
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < w; k++) {
                A[i][j][k] ^= D[i][k];
            }
        }
    }

    return A;
}
int positive_mod(int a, int b) {
    int result = a % b;
    if (result < 0) {
        result += b;
    }
    return result;
}
vector<vector<vector<int>>> rho(vector<vector<vector<int>>> A, int w) {
    vector<vector<vector<int>>> res(5, vector<vector<int>>(5, vector<int>(w, 0)));
    for (int k = 0; k < w; k++) {
        res[0][0][k] = A[0][0][k];
    }
    int i = 1, j = 0;
    for (int t = 0; t < 24; t++) {
        for (int k = 0; k < w; k++) {
            res[i][j][k] = A[i][j][positive_mod((k - (t + 1) * (t + 2) / 2),w)];
        }
        int h = j;
        j = (2 * i + 3 * j) % 5;
        i = h;
    }
    return res;
}
vector<vector<vector<int>>> pi(vector<vector<vector<int>>> A, int w) {
    vector<vector<vector<int>>> res(5, vector<vector<int>>(5, vector<int>(w, 0)));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < w; k++) {
                res[i][j][k] = A[(i + 3 * j) % 5][i][k];
            }
        }
    }

    return res;
}

vector<vector<vector<int>>> chi(vector<vector<vector<int>>> A, int w) {
    vector<vector<vector<int>>> res(5, vector<vector<int>>(5, vector<int>(w, 0)));

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < w; k++) {
                res[i][j][k] = A[i][j][k] ^ ((A[(i + 1) % 5][j][k] ^ 1) * A[(i + 2) % 5][j][k]);
            }
        }
    }

    return res;
}
bool rc(int t){
    if(t % 255 == 0){
        return true;
    }
    vector<bool> R = {true,false,false,false,false,false,false,false};
    for(int i = 0; i < t % 255; i++){
        R.insert(R.begin(),false);
        R[0] = R[0] ^ R[8];
        R[4] = R[4] ^ R[8];
        R[5] = R[5] ^ R[8];
        R[6] = R[6] ^ R[8];
        R.erase(R.end());
    }
    return R[0];
}
vector<vector<vector<int>>> yota(vector<vector<vector<int>>> A, int round_index, int l, int w){
    vector<int> RC(w,0);
    for(int i = 0; i < l; i++){
        RC[(int)pow(2,i) - 1] = rc(i + 7 * round_index);
    }
    for(int k = 0; k < w; k++){
        A[0][0][k] = A[0][0][k] ^ RC[k];
    }

    return A;
}
void print_v(vector<vector<vector<int>>> A){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 64; k++) {
                cout<<A[i][j][k] <<" ";
            }
            cout<<"\n";
        }
    }
}
vector<vector<vector<int>>> round(vector<vector<vector<int>>> A,int round_index){
    A = theta(A);
    //cout<<"after theta\n";
    //print_v(A);
    A = rho(A);
    //cout<<"after rho\n";
    //print_v(A);
    A = pi(A);
    //cout<<"after pi\n";
    //print_v(A);
    A = chi(A);
    //cout<<"after chi\n";
    //print_v(A);
    A = yota(A, round_index);
    //cout<<"after yota\n";
    //print_v(A);
    return A;
}

vector<vector<vector<int>>> to_arr(const string& A, int w){
    vector<vector<vector<int>>> res(5, vector<vector<int>>(5, vector<int>(w, 0)));
    for(int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < w; k++) {
                res[i][j][k] = (int)(unsigned char)A[w * (5*i+j) + k] - 48;
            }
        }
    }
    return res;
}
string to_str(vector<vector<vector<int>>> A, int w) {
    stringstream ss;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            uint64_t value = 0;
            for (int k = 0; k < w; k++) {
                value |= ((uint64_t)A[i][j][k]) << k;
            }
            ss << hex << setw(16) << setfill('0') << value;
        }
    }
    return ss.str();
}


string keccak_p(const string& S){
    int num_rounds = 24;
    int l = 6;
    int rounds = 12 + 2*l;
    vector<vector<vector<int>>> array = to_arr(S);
    for(int i = rounds - num_rounds; i < rounds - 1; i++){
        array = round(array, i);
    }
    string res = to_str(array);
    return res;
}

string pad10_1(int x, int m) {
    int j = (x - ((m + 2) % x)) % x;
    string res = "1" + string(j, '0') + "1";
    return res;
}

string trunc(const string& S, size_t d) {
    return S.substr(0, d);
}
string xor_strings(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) {
        throw invalid_argument("Strings must have the same length");
    }
    string result = "";
    for (size_t i = 0; i < s1.length(); ++i) {
        if (s1[i] == s2[i]) {
            result += '0';
        } else {
            result += '1';
        }
    }

    return result;
}

string sponge(string (*f)(const string&), string (*pad)(int, int), int r, const string& N, size_t d) {
    string P = N + pad(r, N.size());
    size_t n = P.size() / r;
    size_t c = 1600 - r;
    vector<string> P_blocks;
    for (size_t i = 0; i < n; ++i) {
        P_blocks.push_back(P.substr(i * r, r));
    }
    string S(r + c, '0');
    for (const auto& Pi : P_blocks) {
        string Pi_padded = Pi + string(c, '0');
        S = xor_strings(S,Pi_padded);
        S = f(S);
    }
    string Z;
    while (true) {
        Z += trunc(S, r);
        if (d <= Z.size()) {
            return Z.substr(0, d);
        }
        S = f(S);
    }
}
string string_to_binary_string(const string& input) {
    string binaryString = "";
    for (char c : input) {
        binaryString += bitset<8>(c).to_string();
    }
    return binaryString;
}
string sha3_256(const string& message){
    string m_bin = string_to_binary_string(message);
    int r = 1088;
    int d = 256/4;
    return sponge(keccak_p, pad10_1, r, m_bin, d);
}