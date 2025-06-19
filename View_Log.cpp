#include <iostream>
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

void log() {
    string current = branches[head];
    while (!current.empty()) {
        Commit &c = commits[current];
        cout << "Commit: " << current << endl;
        cout << "Message: " << c.message << endl;
        cout << "Time: " << ctime(&c.timestamp);
        cout << "-----------" << endl;
        current = c.parent;
    }
}