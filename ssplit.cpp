#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

/*
Credits:

Evan Teran of Stack Overflow:
http://stackoverflow.com/a/236803/5129091
*/
