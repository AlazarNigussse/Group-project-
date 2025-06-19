#include <iostream>
#include <fstream>

#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

unordered_map<string, string> branches;

void writeFile(const string &path, const string &content) {
    ofstream ofs(path);
    ofs << content;
}

void init() {
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/objects");
    fs::create_directory(".minigit/refs");
    writeFile(".minigit/HEAD", "main");
    branches["main"] = "";
    cout << "Initialized empty MiniGit repository" << endl;
}