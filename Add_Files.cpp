#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

unordered_map<string, string> stagingArea;

string sha1(const string &content) {
    hash<string> hasher;
    return to_string(hasher(content));
}

void writeFile(const string &path, const string &content) {
    ofstream ofs(path);
    ofs << content;
}

string readFile(const string &path) {
    ifstream ifs(path);
    stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}


void add(const string &filename) {
    string content = readFile(filename);
    string hash = sha1(content);
    stagingArea[filename] = hash;
    writeFile(".minigit/objects/" + hash, content);
    cout << "Staged " << filename << endl;
}