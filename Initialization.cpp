#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>

using namespace std;
using namespace filesystem;

// Structure to represent a commit
struct Commit {
    string hash;
    string message;
    string timestamp;
    string parentHash;
    unordered_map<string, string> fileHashes; // filename -> blob hash
};

class MiniGit {
private:
    path gitDir;
    string currentBranch;
    unordered_map<string, string> branches; // branch name -> commit hash
    unordered_map<string, Commit> commits; // commit hash -> Commit object
    
public:
    // Initialize a new MiniGit repository
    bool init() {
        // Check if already initialized
        if (exists(".minigit")) {
            cerr << "MiniGit repository already exists in this directory.\n";
            return false;
        }

        try {
            // Create .minigit directory
            create_directory(".minigit");
            
            // Create subdirectories
            create_directory(".minigit/objects"); // For storing file blobs
            create_directory(".minigit/refs");    // For storing branch references
            
            // Create initial files
            ofstream headFile(".minigit/HEAD");
            headFile << "ref: refs/heads/master\n";
            headFile.close();
            
            // Initialize data structures
            currentBranch = "master";
            branches["master"] = ""; // Empty hash for initial state
            
            cout << "Initialized empty MiniGit repository in " << absolute(".minigit") << "\n";
            return true;
        } catch (const filesystem_error& e) {
            cerr << "Error initializing MiniGit repository: " << e.what() << "\n";
            return false;
        }
    }

    // Helper function to check if repository is initialized
    bool isInitialized() const {
        return exists(".minigit");
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2 || string(argv[1]) != "init") {
        cerr << "Usage: ./minigit init\n";
        return 1;
    }

    MiniGit git;
    if (!git.init()) {
        return 1;
    }

    return 0;
}