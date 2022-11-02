#include <bits/stdc++.h>

using namespace std;

class FMIndex{
private:
    string BWT;
    map<char,int> C;
    map<char,vector<int>> OCC;
    string alphabet;

    void init_alphabet(){
        for(auto &p: C){
            alphabet += p.first;
        }
    }

    void buildC(){
        init_alphabet();

        vector<int> aux;
        for(auto &p: C){
            aux.push_back(p.second);
        }
        int i=0;
        int sum=0;
        for(auto &p: C){
            p.second = sum;
            sum += aux[i];
            i++;
        }
    }

    void buildOCC(){
        for(auto &p: C){
            OCC[p.first] = vector<int>(BWT.size(),0);
        }
        for(auto &p: OCC){
            int count = 0;
            for(int j=0;j<BWT.size();j++){
                if(BWT[j] == p.first) count++;
                p.second[j] = count;
            }
        }
    }

    void build(const string &s){
        string t = s;
        t.push_back('$');
        int n = t.size();
        vector<string> M(n);
        for(int i = 0; i < n; i++){
            C[t[0]]++;
            M[i] = t;
            rotate(t.begin(),t.begin()+n-1,t.end());
        }
        sort(M.begin(),M.end());
        for(int i = 0; i < n; i++){
            BWT.push_back(M[i][n-1]);
        }

        buildC();
        buildOCC();
    }




public:
    FMIndex(const string &s){
        build(s);
    }
    void print(){

        cout << "BWT"<< endl;
        cout << BWT << endl;
        cout << endl;

        cout << "C" << endl;
        for(auto &p : C){
            cout << p.first << " " << p.second << endl;
        }
        cout << endl;

        cout << "OCC" << endl;
        for(auto &p : OCC){
            cout << p.first << " ";
            for(int i = 0; i < p.second.size(); i++){
                cout << p.second[i] << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "alphabet" << endl;
        cout << alphabet << endl;
    }
    int count(const string &s){
        int i = s.size()-1;
        char c = s[i];
        int sp = C[c];
        char next;
        int ep;
        if(c == alphabet[alphabet.size()-1]){
            ep = BWT.size()-1;
        }else{
            next = alphabet[alphabet.find(c)+1];
            ep = C[next]-1;
        }

        while(sp <= ep && i >= 1){
            i--;
            c = s[i];
            sp = C[c] + OCC[c][sp-1];
            ep = C[c] + OCC[c][ep]-1;
        }
        return ep-sp+1;

    }

};