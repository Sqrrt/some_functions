#include "texts_and_numbers.h"

using namespace std;

string textToNumber(const string& text) {
    ostringstream result;
    for (unsigned char c : text) {
        result << (int)c;
    }
    return result.str();
}

string numberToText(const string& numberString) {
    string text;
    for (size_t i = 0; i < numberString.length();) {
        int asciiValue = 0;

        string segment = numberString.substr(i, 3);
        asciiValue = stoi(segment);

        text += static_cast<char>(asciiValue);

        i += segment.length();
    }
    return text;
}