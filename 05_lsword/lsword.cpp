//
// Created by kuzdavoj@fel.cvut.cz on 11/19/2022.
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

const vector<char> from_int_char{'a','b','c','d','e'};


// TODO: nemam tyhle informace nahodou uz nekde ulozeny?
struct State_FSD{
    vector<uint> from;  // parent uzlu
    vector<char> str;   //
    vector<uint> dist;
    bool was_in_q;
} ;


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

// projdi graf z candidate nodu zpet do start nodu
// pismena na hrane davej do stacku
// pokud ma hrana dve pismena, duplikuj stack
// pokud ma uzel dva predky, duplikuj stack
// nakonec stack popni, a zkontroluj vyskyt subtringu
vector<string> backtrack_strings(vector<uint>& candidates, NFA& nfa, vector<State_FSD>& fsd_states, uint last_depth) {
    vector<string> found;
    // pro kazdy final node
    for(uint c : candidates) {
        uint curr_depth = last_depth;
        vector<stack<uint>> stacks;
        pair<uint, uint> node_stack;
        queue<uint> q;
        q.push(c);
        while(!q.empty()) {
            State_FSD curr = fsd_states[q.front()]; q.pop();
            vector<uint> neighbors;
            for (uint i = curr.dist.size(); i >= 0; --i) {
                if(curr.dist[i] == curr_depth)
                    neighbors.push_back(curr.from[i]);
                else
                    break; // vice uzlu s curr_depth tam neni
            }

        }
    }
}


int main() {
    uint N, M;   // N = pocet stavu, M = velikost abecedy
    cin >> N >> M;
    /// read empty line
    string e; getline(cin, e);

    vector<bool> is_final(N, false);
    NFA nfa = readNFA(N, M, is_final);

    string S;
    getline(cin, S);



    // NFA reprezentovane grafem projdeme BFS
    // s kazdou 'vlnou' ulozime do uzlu informace FSD

    /// init vsech FSD nodu
    State_FSD sf;
    sf.dist={};
    sf.str={};
    sf.from={};
    vector<State_FSD> fsd_states(N, sf);

    /// init start stavu
    fsd_states[0].dist.push_back(0);
    fsd_states[0].from.push_back(UINT_MAX);
    fsd_states[0].str.push_back('N');
    fsd_states[0].was_in_q = false;

    // zaciname od start stavu
    queue<uint> q; q.push(0);

    vector<uint> candidates;
    uint last_depth = 0;
    while(!q.empty()) {
        uint curr = q.front(); q.pop();
        uint curr_depth = fsd_states[curr].dist[0]; // BFS garantuje ze prvni dist je nejblizsi ke startu


        /// v pripade ze prejdu z hloubky X na hloubku X+1
        /// chci zkontrolovat zda v hloubce X neexistuji final stavy
        /// pokud existovali, chci z techto stavu zkusit matchnout hledany substring
        if(curr_depth != last_depth && !candidates.empty()) {
            cout << "KMP check";
            vector<string> c = backtrack_strings(candidates, nfa, fsd_states, last_depth);
        }

        /// kazda hrana
        for (uint edge = 0; edge < nfa[curr].size(); ++edge) { //
            char letter = from_int_char[edge];
            vector<uint> dest_states = nfa[curr][edge];
            /// kazdy soused
            for (uint state : dest_states) {
                fsd_states[state].from.push_back(curr);
                fsd_states[state].str.push_back(letter);
                fsd_states[state].dist.push_back(curr_depth + 1);
                if(!fsd_states[state].was_in_q)
                    q.push(state);
            }
        }

        if (is_final[curr]) {
            candidates.push_back(curr);
        }
        last_depth = curr_depth;
    }

    cout << "OK" << endl;

    return 0;
}

