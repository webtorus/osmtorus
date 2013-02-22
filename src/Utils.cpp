#include <vector>
#include <string>
#include <sstream>

#include "include/Utils.hpp"

using namespace std;

vector<string>& split(const string& s, char delim, vector<string>& elems) 
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}


vector<string> split(const string& s, char delim) 
{
    vector<string> elems;

    return split(s, delim, elems);
}

void replaceAll(string& str, const string& from, const string& to) 
{
    if(from.empty())
        return;

    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}