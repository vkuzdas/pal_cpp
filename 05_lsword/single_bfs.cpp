//
// Created by kuzdavoj@fel.cvut.cz on 12/6/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <climits>
#include <sstream>
#include <stack>

using namespace std;
using uint = unsigned int;
using ulli = unsigned long long int;
using NFA = vector<vector<vector<uint>>>;

struct State {
    uint id;
    string path;
    uint S_match;
};

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

bool is_greater_or_shorter(string neigh, string curr) {
    if(neigh.size() < curr.size()) return true;
    // jinak jsou stejne dlouhe (kvuli BFS)
    for (uint i = 0; i < neigh.size(); ++i) {
        if(neigh[i] > curr[i]) {
            return true;
        }
    }
    return false;
}

int main() {
    uint N, M;   // N = pocet stavu, M = velikost abecedy
    cin >> N >> M;
    /// read empty line
    string empty;
    getline(cin, empty);

    vector<bool> is_final(N, false);
    NFA nfa = readNFA(N, M, is_final);

    string S;
    getline(cin, S);
    cout << "ok";

    // inituj vsechny nody
    vector<State> states;
    for (uint i = 0; i < N; ++i) {
        State s;
        s.path="";
        s.S_match=0;
        s.id=i;
        states.push_back(s);
    }

    vector<bool> in_Q(N,false);
    queue<uint> Q;
    Q.push(0);
    in_Q[0] = true;

    while(!Q.empty()) {
        State curr = states[Q.front()];
        Q.pop();
        in_Q[curr.id] = false;
        /// pro kazdej edge
        for (uint e = 0; e < nfa[curr.id].size(); ++e) {
            char edge = from_int_char[e];
            vector<uint> neighbor_ids = nfa[curr.id][e];
            /// pro kazdeho souseda na edgi
            for(uint id : neighbor_ids) {
                State neigh = states[id];
                if(neigh.path.empty() || is_greater_or_shorter(neigh.path, curr.path+edge)) {
                    // IF    neigh je vetsi -> updatujeme na current
                    // ELSE  jsme uz do neigh nasli nejkratsi cestu
                    neigh.path = curr.path + edge;

                    // pokud curr.S_match != 0 kontrolujeme jen edge
                    if(curr.S_match == 0) { /// zadnej match
                        if(S[0] == edge)
                            neigh.S_match = 1;
                        else
                            neigh.S_match = 0;
                    }

                    else if(curr.S_match == S.size()){ /// plnej match
                        neigh.S_match = curr.S_match;
                    }

                    else { /// castecnej match
                        char expected = S[curr.S_match];
                        if(edge == expected) {
                            neigh.S_match = curr.S_match + 1;
                        }
                        else {
                            if(S[0] == edge)
                                neigh.S_match = 1;
                            else
                                neigh.S_match = 0;
                        }
                    }
                }
                states[id] = neigh;
                /// pokud je cesta mensi nebo rovna
                /// uz jsme uzel zpracovali nejlepsi cestou
                if(!in_Q[neigh.id]) {
                    Q.push(neigh.id);
                    in_Q[neigh.id] = true;
                }
            }
        }
    }

}
