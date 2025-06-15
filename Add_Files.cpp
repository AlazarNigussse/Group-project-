#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <openssl/sha.h> // For SHA-1 hashing

using namespace std;
using namespace filesystem;

class MiniGit {
private:
    path objectsDir = ".minigit/objects/";
    unordered_map<string, string> stagingArea; // filename -> hash

    // Helper function to compute SHA-1 hash of file content
    string computeFileHash(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            return "";
        }

        // Read file content
        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();

        // Compute SHA-1 hash
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1((const unsigned char*)content.c_str(), content.size(), hash);

        // Convert to hex string
        stringstream hexHash;
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
            hexHash << hex << setw(2) << setfill('0') << (int)hash[i];
        }

        return hexHash.str();
    }

    // Helper to store blob in objects database
    bool storeBlob(const string& filename, const string& hash) {
        path blobPath = objectsDir / hash.substr(0, 2) / hash.substr(2);
        create_directories(blobPath.parent_path());

        ofstream blobFile(blobPath);
        if (!blobFile) {
            cerr << "Error: Could not create blob file" << endl;
            return false;
        }

        ifstream sourceFile(filename);
        blobFile << sourceFile.rdbuf();
        return true;
    }

public:
    // Add file to staging area
    bool add(const string& filename) {
        if (!exists(filename)) {
            cerr << "Error: File " << filename << " does not exist" << endl;
            return false;
        }

        string hash = computeFileHash(filename);
        if (hash.empty()) return false;

        if (!storeBlob(filename, hash)) {
            return false;
        }

        stagingArea[filename] = hash;
        cout << "Added " << filename << " (hash: " << hash << ")" << endl;
        return true;
    }

    // Check if repository is initialized
    bool isInitialized() const {
        return exists(".minigit") && is_directory(".minigit");
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3 || string(argv[1]) != "add") {
        cerr << "Usage: ./minigit add <filename>" << endl;
        return 1;
    }

    MiniGit git;
    if (!git.isInitialized()) {
        cerr << "Error: MiniGit repository not initialized" << endl;
        return 1;
    }

    if (!git.add(argv[2])) {
        return 1;
    }

    return 0;
}