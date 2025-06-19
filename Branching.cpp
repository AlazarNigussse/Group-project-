#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<string, string> branches;
string head = "main";

void branch(const string &name) {
    branches[name] = branches[head];
    cout << "Created branch " << name << endl;
}