#include <vector>
#include <string>
#include <sstream>

#include "include/Utils.hpp"

using namespace std;

vector<string>& split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}


vector<string> split(const string& s, char delim) {
    vector<string> elems;

    return split(s, delim, elems);
}