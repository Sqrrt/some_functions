#include "sha256.h"
#include "numbers.h"
using namespace std;
string correct_binary(const string &x) {
    string bin;
    for (char c : x) {
        bin += bitset<8>(c).to_string();
    }
    uint64_t message_length = bin.length();
    bin += "1";
    int padLength = (512 - ((message_length + 64) % 512)) % 512 - 1;
    bin += string(padLength, '0');
    bin += bitset<64>(message_length).to_string();
    return bin;
}
vector<string> split_to_blocks(const string& blocks, int maxLength){
    vector<string> result;
    size_t inputLength = blocks.length();

    for (size_t i = 0; i < inputLength; i += maxLength) {
        result.push_back(blocks.substr(i, maxLength));
    }

    return result;
}
vector<uint32_t> split_to_uint32(const string& binary_str) {
    vector<uint32_t> result;
    size_t len = binary_str.size();
    for (size_t i = 0; i < len; i += 32) {
        string block = binary_str.substr(i, 32);
        if (block.size() < 32) {
            block = string(32 - block.size(), '0') + block;
        }
        uint32_t value = bitset<32>(block).to_ulong();
        result.push_back(value);
    }
    return result;
}


uint32_t get_fractional_bits(double number) {
    double fractionalPart = number - floor(number);
    auto result = static_cast<uint32_t>(fractionalPart * (1ULL << 32));
    return result;
}
uint32_t right_rotate(uint32_t word, int center) {
    return uint32_t (word >> center) | (word << (32 - center));
}
uint32_t s0(uint32_t word){
    return right_rotate(word, 7) ^ right_rotate(word, 18) ^ (word >> 3);
}
uint32_t s1(uint32_t word){
    return right_rotate(word, 17) ^ right_rotate(word, 19) ^ (word >> 10);
}
uint32_t sigma0(uint32_t a){
    return right_rotate(a, 2) ^ right_rotate(a, 13) ^ right_rotate(a, 22);
}
uint32_t sigma1(uint32_t e){
    return right_rotate(e, 6) ^ right_rotate(e, 11) ^ right_rotate(e, 25);
}
uint32_t Choice(uint32_t e, uint32_t f, uint32_t g){
    return (e & f) ^ ((~e) & g);
}
uint32_t Majority(uint32_t a, uint32_t b, uint32_t c){
    return (a & b) ^ (a & c) ^ (b & c);
}
uint32_t Temp1(uint32_t e, uint32_t f, uint32_t g, uint32_t h, uint32_t word, uint32_t m){
    return h + sigma1(e) + Choice(e,f,g) + m + word;
}
uint32_t Temp2(uint32_t a,uint32_t b,uint32_t c){
    return sigma0(a) + Majority(a,b,c);
}
vector<uint32_t> to_blocks(vector<uint32_t> &blocks){
    for (int i = 16; i < 64; i++){
        blocks[i] = blocks[i - 16] + s0(blocks[i - 15]) + blocks[i - 7] + s1(blocks[i - 2]);
    }
    return blocks;
}
vector<uint32_t> mas_hashes64() {
    vector<uint32_t> mas;
    int limit = 312;
    vector<int> random = sieve_eratosthenes(limit);
    random = sieve_eratosthenes(limit);
    mas.reserve(random.size());
    for(const auto elem : random){
        mas.push_back(get_fractional_bits(pow(elem, 1.0 / 3.0)));
    }
    return mas;
}
vector<uint32_t> get_random(){
    int limit = 20;
    vector<uint32_t> res;
    vector<int> primes = sieve_eratosthenes(limit);
    res.reserve(primes.size());
    for(const auto elem : primes){
        res.push_back(get_fractional_bits(pow(elem, 1.0/2.0)));
    }
    return res;
}
vector<uint32_t> compress(vector<uint32_t> mas_of_words, vector<uint32_t > mas_hashes, vector<uint32_t> random) {
    uint32_t a = random[0];
    uint32_t b = random[1];
    uint32_t c = random[2];
    uint32_t d = random[3];
    uint32_t e = random[4];
    uint32_t f = random[5];
    uint32_t g = random[6];
    uint32_t h = random[7];

    for (int i = 0; i < 64; i++) {
        uint32_t t1 = Temp1(e, f, g, h, mas_of_words[i], mas_hashes[i]);
        uint32_t t2 = Temp2(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    vector<uint32_t> result = {a + random[0],
                               b + random[1],
                               c + random[2],
                               d + random[3],
                               e + random[4],
                               f + random[5],
                               g + random[6],
                               h + random[7]};
    return result;
}

string sha256(const string &message) {
    vector<uint32_t> mas_hashes = mas_hashes64();
    string bin_mes = correct_binary(message);
    vector<string> blocks = split_to_blocks(bin_mes);
    vector<uint32_t> hashes = get_random();
    for(const auto& block : blocks) {
        vector<uint32_t> blocks_b = split_to_uint32(block);
        blocks_b.insert(blocks_b.end(), 48, 0);
        blocks_b = to_blocks(blocks_b);
        hashes = compress(blocks_b, mas_hashes, hashes);
    }
    string result;
    ostringstream oss;
    for (const uint32_t &hash : hashes) {
        oss << hex << setw(8) << setfill('0')<< hash;
    }
    result = oss.str();
    return result;
}
