#include <iostream>
#include <string>
#include <cctype>
using namespace std;

typedef struct tag *adr;

struct infotype{
    string namastate;
    bool finalstate;
};

struct tag{
    infotype info;
    adr target1;
    adr target2;
    adr target3;
};

struct List{
    adr first;
};


int main(){
    bool End = false;
    int i = 0;
    string token;
    string sub;
    string arr[6] = {"html", "head", "title", "body", "p", "h1"};
    string lastcheck;
    List DFA;
    adr A,B,C,D,E,X,p;
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

    DFA.first = A;
    p = A;

    cout<<"Masukkan Token: Ketik 'exit' Untuk Keluar"<<endl;
    cout<<"Token:";
    cin>>token;
    while (!End){
        i = 0;
        p = A;
        while (i<token.length()){
            sub = tolower(token[i]);
            if(p == A){
                if (sub == "<"){
                    p = A->target1;
                }else{
                    p = A->target2;
                }
            }else if(p == B){
                if(sub == "/"){
                    p = B->target1;
                }else if(isalpha(sub[0])){
                    p = B->target2;
                }else{
                    p = B->target3;
                }
            }else if(p == C){
                if (isalpha(sub[0]) || (sub[0] == '1')){
                    p = C->target1;
                }else if(sub == ">"){
                    p = C->target2;
                }else{
                    p = C->target3;
                }
            }else if(p == D){
                if(isalpha(sub[0]) || (sub[0] == '1')){
                    p = D->target1;
                }else if(sub == ">"){
                    p = D->target2;
                }else{
                    p = D->target3;
                }
            }else if(p == E){
                p =E->target1;
            }else if(p == X){
                p = E->target1;
            }
            i++;
        }
        if(p->info.finalstate){
            if(token[1] == '/'){
                lastcheck = token.substr(2, token.length() - 3);
            }else {
                lastcheck = token.substr(1, token.length()-2);
            };
            int j = 0;
            bool found = false;
            while(j<6 && !found){
                if(arr[j] == lastcheck){
                    found = true;
                }
                j++;
            }
            if(found){
                cout<<"ACCEPTED"<<endl;
            }else{
                cout<<"REJECTED"<<endl;
            }
        }else{
            cout<<"REJECTED"<<endl;
        }
        cout<<"Masukkan Token: Ketik 'exit' Untuk Keluar"<<endl;
        cout<<"Token:";
        cin>>token;
        if(token == "exit"){
            End = true;
        }

    }

}
