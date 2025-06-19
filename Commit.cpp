#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Commit {
    string hash;
    string parent;
    string parent2; // for merge
    string message;
    time_t timestamp;
    unordered_map<string, string> files; // filename -> blob hash
};

unordered_map<string, string> stagingArea;
unordered_map<string, Commit> commits;
unordered_map<string, string> branches;
string head = "main";

string sha1(const string &content) {
    hash<string> hasher;
    return to_string(hasher(content));
}

void writeFile(const string &path, const string &content) {
    ofstream ofs(path);
    ofs << content;
}

void commit(const string &msg) {
    Commit c;
    c.timestamp = time(nullptr);
    c.message = msg;
    c.parent = branches[head];
    c.parent2 = "";
    
    if (!c.parent.empty())
        c.files = commits[c.parent].files;

    for (auto &[file, hash] : stagingArea) {
        c.files[file] = hash;
    }

    stringstream ss;
    ss << msg << c.timestamp;
    for (auto &[k, v] : c.files) ss << k << v;
    c.hash = sha1(ss.str());

    commits[c.hash] = c;
    branches[head] = c.hash;
    stagingArea.clear();

    writeFile(".minigit/commits_" + c.hash + ".txt", msg);
    cout << "Committed as " << c.hash << endl;
}