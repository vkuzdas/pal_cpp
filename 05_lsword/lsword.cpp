//
// Created by kuzdavoj@fel.cvut.cz on 11/19/2022.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <climits>
#include <sstream>
#include <cstring>

using namespace std;
using uint = unsigned int;
using ulli = unsigned long long int;
using vs_pair = pair<vector<uint>,string>;

const vector<char> from_int_char{'a','b','c','d','e'};


bool is_abcde(string s) {
    if(s[0]=='a' || s[0]=='b' || s[0]=='c' || s[0]=='d' || s[0]=='e')
        return true;
    return false;
}

vector<vector<uint>> readLine(uint curr_N, uint N, uint M, vector<bool>& is_final) {
    string line;
    getline(cin, line);  // precte celej line, format:  0 -     a 1    b 3   c
    istringstream iss(line);

    // extrahuj cislo stavu
    string curr_state_str = "";
    while(curr_state_str.empty()) {
        getline(iss, curr_state_str, ' ');
    }
    int curr_state = stoi(curr_state_str);

    // extrahuj is_final
    string final_str;
    getline(iss, final_str, ' ');
    is_final[uint(curr_state)] = final_str[0] == 'F';


    vector<vector<uint>> edge_dest{M};

    // alespon M-krat precti
    string token;
    int subvec_index = -1;
    while(getline(iss, token, ' ')) {
        if (token.empty()) continue;
        if (is_abcde(token)) {
            subvec_index = token[0] - 'a';
        } else {
            uint dest_state = uint(stoi(token));
            edge_dest[uint(subvec_index)].push_back(dest_state);
        }
    }
    return edge_dest;
}

vector<vector<vector<uint>>> readNFA(uint N, uint M, vector<bool>& is_final) {
    vector<vector<vector<uint>>> v(N);

    // vstup je N radku
    // radek ma format  1 -  a 2 5  b     c 4
    // na radku je M hran
    for (uint i = 0; i < N; ++i) {
        auto vv = readLine(i, N, M, is_final);
        v[i] = vv;
    }
    return v;
}

vector<uint> BFS_step(vector<bool>& is_final, vector<vector<vector<uint>>>& nfa, vector<vs_pair>& found_seq, queue<uint>& q) {
    uint curr = q.front(); q.pop(); // kdyz je curr 0 tak nemame zadne pary
    vector<uint> final_states{};
    for (uint i = 0; i < nfa[curr].size(); ++i) {

        auto letter_edge = nfa[curr][i];
        if (letter_edge.empty()) continue;

        for (auto dest_state : letter_edge) { // a 1 2 3
            q.push(dest_state);
            if (is_final[dest_state]) final_states.push_back(dest_state);

            if(curr == 0) { // inicialni pruchod teprve "zaklada" pary
                vector<uint> state_seq {dest_state};
                string char_seq {from_int_char[i]};
                pair<vector<uint>,string> new_pair = {state_seq, char_seq};
                found_seq.push_back(new_pair);
            }

            // else: najdi par kterej ma sekvenci koncici curr TODO: hash-mapa?
            // pust z nej dalsi krok BFS a pridej dalsi pary
///            else {}  musime appendovat do vytvorenych paru
            else{
                for (uint j = 0; j < found_seq.size(); ++j) {
                    vs_pair pair = found_seq[j];
                    vector<uint> state_seq = pair.first;
                    uint last_state = state_seq[state_seq.size()-1];
                    if(last_state == curr) {
                        found_seq[i].first.push_back(dest_state);
                        found_seq[i].second.push_back(from_int_char[i]);
                    }
                }
            }
        }
    }
    return final_states;
}


int main() {
    uint N, M;   // N = pocet stavu, M = velikost abecedy
    cin >> N >> M;

    string e;getline(cin, e); /// read empty line

    vector<bool> is_final(N, false);
    vector<vector<vector<uint>>> nfa;
    nfa = readNFA(N, M, is_final);
    string S;
    getline(cin, S);

//    vector<pair<vector<uint>,string>> found_seq;
//    queue<uint> q;
//    q.push(0); // zaciname od stavu 0
//    // iteruj BFS step dokud nenajdes final stav
//    // po tom co najdes final stav, zkontroluj sekvenci hran KMPckem / Boyer Moorem
//    while(!q.empty()) {
//        /// chceme znat nalezene finalni stavy
//        /// sekvence stavu koncici finalnim stavem znaci moznost kontroly stringu KMPckem
//        vector<uint> found_final_states = BFS_step(is_final, nfa, found_seq, q);
//    }
    cout << "OK" << endl;

    return 0;
}

