#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

using namespace std;

vector<string> split(const string& s, char delim);

vector<string>& split(const string& s, char delim, vector<string>& elems);

void replaceAll(string& str, const string& from, const string& to);

#endif