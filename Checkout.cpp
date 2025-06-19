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

unordered_map<string, Commit> commits;
unordered_map<string, string> branches;
string head = "main";

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

void checkout(const string &target) {
    string commitHash = branches.count(target) ? branches[target] : target;
    if (!commits.count(commitHash)) {
        cout << "Invalid target" << endl;
        return;
    }

    Commit &c = commits[commitHash];
    for (auto &[file, hash] : c.files) {
        string content = readFile(".minigit/objects/" + hash);
        writeFile(file, content);
    }

    if (branches.count(target)) head = target;
    else writeFile(".minigit/HEAD", commitHash);

    cout << "Checked out to " << target << endl;
}