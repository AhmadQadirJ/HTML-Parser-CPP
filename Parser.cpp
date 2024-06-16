#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

typedef struct tag *adr;
struct infotype {
    string namastate;
    bool finalstate;
};
struct tag {
    infotype info;
    adr target1;
    adr target2;
    adr target3;
};
struct List {
    adr first;
};

bool isValidToken(const string &token) {
    string arr[6] = {"html", "head", "title", "body", "p", "h1"};
    adr A, B, C, D, E, X, p;

    A = new tag;
    B = new tag;
    C = new tag;
    D = new tag;
    E = new tag;
    X = new tag;

    A->info.namastate = "A";
    A->info.finalstate = false;
    A->target1 = B;
    A->target2 = X;
    A->target3 = NULL;

    B->info.namastate = "B";
    B->info.finalstate = false;
    B->target1 = C;
    B->target2 = D;
    B->target3 = X;

    C->info.namastate = "C";
    C->info.finalstate = false;
    C->target1 = C;
    C->target2 = E;
    C->target3 = X;

    D->info.namastate = "D";
    D->info.finalstate = false;
    D->target1 = D;
    D->target2 = E;
    D->target3 = X;

    E->info.namastate = "E";
    E->info.finalstate = true;
    E->target1 = X;
    E->target2 = NULL;
    E->target3 = NULL;

    X->info.namastate = "X";
    X->info.finalstate = false;
    X->target1 = X;
    X->target2 = NULL;
    X->target3 = NULL;

    p = A;
    int i = 0;
    string sub;
    while (i < token.length()) {
        sub = tolower(token[i]);
        if (p == A) {
            if (sub == "<") {
                p = A->target1;
            } else {
                p = A->target2;
            }
        } else if (p == B) {
            if (sub == "/") {
                p = B->target1;
            } else if (isalpha(sub[0])) {
                p = B->target2;
            } else {
                p = B->target3;
            }
        } else if (p == C) {
            if (isalpha(sub[0]) || (sub[0] == '1')) {
                p = C->target1;
            } else if (sub == ">") {
                p = C->target2;
            } else {
                p = C->target3;
            }
        } else if (p == D) {
            if (isalpha(sub[0]) || (sub[0] == '1')) {
                p = D->target1;
            } else if (sub == ">") {
                p = D->target2;
            } else {
                p = D->target3;
            }
        } else if (p == E) {
            p = E->target1;
        } else if (p == X) {
            p = E->target1;
        }
        i++;
    }

    if (p->info.finalstate) {
        string lastcheck;
        if (token[1] == '/') {
            lastcheck = token.substr(2, token.length() - 3);
        } else {
            lastcheck = token.substr(1, token.length() - 2);
        }

        for (const string &validToken : arr) {
            if (validToken == lastcheck) {
                return true;
            }
        }
    }
    return false;
}

// Function to extract tokens from the HTML content
vector<string> extractTokens(const string &htmlContent) {
    vector<string> tokens;
    size_t pos = 0;
    while ((pos = htmlContent.find('<', pos)) != string::npos) {
        size_t endPos = htmlContent.find('>', pos);
        if (endPos == string::npos) break; // Invalid HTML
        string token = htmlContent.substr(pos, endPos - pos + 1); 
        if (isValidToken(token)){
            tokens.push_back(token);
        }
        pos = endPos + 1;
    }
    return tokens;
}

// Function to parse according to CFG rules
bool parseS(vector<string>& tokens, size_t& index);
bool parseA(vector<string>& tokens, size_t& index);
bool parseB(vector<string>& tokens, size_t& index);
bool parseC(vector<string>& tokens, size_t& index);
bool parseD(vector<string>& tokens, size_t& index);

bool parseS(vector<string>& tokens, size_t& index) {
    size_t savedIndex = index;
    if (index < tokens.size() && tokens[index] == "<html>") {
        index++;
        if (parseA(tokens, index) && index < tokens.size() && tokens[index] == "</html>") {
            index++;
            return true;
        }
        index = savedIndex;
        if (index < tokens.size() && tokens[index] == "<html>") {
            index++;
            if (parseB(tokens, index) && index < tokens.size() && tokens[index] == "</html>") {
                index++;
                return true;
            }
        }
        index = savedIndex;
        if (index < tokens.size() && tokens[index] == "<html>") {
            index++;
            if (parseA(tokens, index) && parseB(tokens, index) && index < tokens.size() && tokens[index] == "</html>") {
                index++;
                return true;
            }
        }
    }
    index = savedIndex;
    return false;
}

bool parseA(vector<string>& tokens, size_t& index) {
    if (index < tokens.size() && tokens[index] == "<head>") {
        index++;
        if (parseD(tokens, index) && index < tokens.size() && tokens[index] == "</head>") {
            index++;
            return true;
        }
    }
    return true; // Epsilon production
}

bool parseB(vector<string>& tokens, size_t& index) {
    if (index < tokens.size() && tokens[index] == "<body>") {
        index++;
        if (parseC(tokens, index) && index < tokens.size() && tokens[index] == "</body>") {
            index++;
            return true;
        }
    }
    return true; // Epsilon production
}

bool parseC(vector<string>& tokens, size_t& index) {
    while (true) {
        size_t savedIndex = index;
        if (index < tokens.size() && tokens[index] == "<h1>") {
            index++;
            if (index < tokens.size() && tokens[index] == "</h1>") {
                index++;
                continue;
            }
        }
        index = savedIndex;
        if (index < tokens.size() && tokens[index] == "<p>") {
            index++;
            if (index < tokens.size() && tokens[index] == "</p>") {
                index++;
                continue;
            }
        }
        break;
    }
    return true; // Epsilon production
}

bool parseD(vector<string>& tokens, size_t& index) {
    if (index < tokens.size() && tokens[index] == "<title>") {
        index++;
        if (parseD(tokens, index) && index < tokens.size() && tokens[index] == "</title>") {
            index++;
            return true;
        }
    }
    return true; // Epsilon production
}

// Function to validate the HTML structure using CFG
bool validateHTML(vector<string>& tokens) {
    size_t index = 0;
    return parseS(tokens, index) && index == tokens.size();
}

int main() {
    while (true) {
        string filename;
        cout << "Enter the HTML file name (or type 'exit' to quit): ";
        cin >> filename;
        if (filename == "exit") {
            break;
        }

        ifstream htmlFile(filename);
        if (!htmlFile.is_open()) {
            cerr << "Failed to open the file." << endl;
            continue;
        }

        stringstream buffer;
        buffer << htmlFile.rdbuf();
        string htmlContent = buffer.str();
        htmlFile.close();

        vector<string> tokens = extractTokens(htmlContent);
        bool isValid = validateHTML(tokens);

        cout << "Tokens:" << endl;
        for (const auto &token : tokens) {
            cout << token << endl;
        }

        cout << "Status: " << (isValid ? "Accepted" : "Rejected") << endl;
    }

    return 0;
}
