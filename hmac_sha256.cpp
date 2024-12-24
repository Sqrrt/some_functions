#include "hmac_sha256.h"
#include "sha256.h"


using namespace std;

vector<uint8_t> xor_vectors(const vector<uint8_t>& a, const vector<uint8_t>& b) {
    vector<uint8_t> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}
string string_to_hex(const string& input) {
    ostringstream hexStream;
    for (unsigned char c : input) {
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);
    }
    return hexStream.str();
}
vector<uint8_t> hex_to_uint8(const string& hexStr) {
    if (hexStr.length() % 2 != 0) {
        throw invalid_argument("Hex string must have an even length.");
    }
    vector<uint8_t> result;
    result.reserve(hexStr.length() / 2);
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        string byteString = hexStr.substr(i, 2);
        auto byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
        result.push_back(byte);
    }

    return result;
}

string hmac_sha256(const string& key, const string& message) {
    string key_bytes = string_to_hex(key);
    if (key_bytes.size() > BLOCK_SIZE) {
        key_bytes = sha256(key);
    }
    if (key_bytes.size() < BLOCK_SIZE) {
        key_bytes += string(BLOCK_SIZE - key_bytes.size(), '0');
    }

    vector<uint8_t> ipad(BLOCK_SIZE/2, 0x36);
    vector<uint8_t> opad(BLOCK_SIZE/2, 0x5C);

    vector<uint8_t> key_bytes8 = hex_to_uint8(key_bytes);

    vector<uint8_t> ikey_bytes8 = xor_vectors(ipad, key_bytes8);
    vector<uint8_t> okey_bytes8 = xor_vectors(opad, key_bytes8);

    string message_hex = string_to_hex(message);

    string kb8_1(ikey_bytes8.begin(), ikey_bytes8.end());
    kb8_1 = string_to_hex(kb8_1);
    kb8_1 += message_hex;

    string res1 = sha256(kb8_1);

    //cout<< res1<<"\n";
    string kb8_2(okey_bytes8.begin(), okey_bytes8.end());
    kb8_2 += res1;
    string res2 = sha256(kb8_2);
    return res2;
}