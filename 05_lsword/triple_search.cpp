//
// Created by kuzdavoj@fel.cvut.cz on 12/8/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <climits>
#include <sstream>

using namespace std;
using uint = unsigned int;
using ld_pair = pair<char, uint>; // <letter & destination>


struct State {
    uint id;
    string path_from_start;
    string path_to_end;
    vector<uint> sequence{};
};


string final_res(6000, 'f');

bool is_abcde(string s) {
    if(s[0]=='a' || s[0]=='b' || s[0]=='c' || s[0]=='d' || s[0]=='e')
        return true;
    return false;
}

vector<ld_pair> readLine(vector<bool>& is_final, vector<uint>& finals) {
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
    if(final_str[0] == 'F') {
        is_final[uint(curr_state)] = true;
        finals.push_back(uint(curr_state));
    }

    vector<ld_pair> src_edges;

    uint dest;
    char letter;

    // alespon M-krat precti
    string token;
    bool got_dest = false;
    while(getline(iss, token, ' ')) {
        if (token.empty()) continue;
        if (is_abcde(token)) {
            letter = token[0];
            got_dest = false;
        } else {
            dest = uint(stoi(token));
            got_dest = true;
        }
        if(got_dest) {
            ld_pair p = {letter, dest};
            src_edges.push_back(p);
        }
    }
    return src_edges;
}

vector<vector<ld_pair>> readNFA(uint N, vector<bool>& is_final, vector<uint>& finals) {
    vector<vector<ld_pair>> nfa;
    for (uint src = 0; src < N; ++src) {
        vector<ld_pair> dest_edges = readLine(is_final, finals);
        nfa.push_back(dest_edges);
    }
    return nfa;
}

/**
 * jestli jsme nasli lepsi string nez ma neighbour momentalne
 */
bool lex_cmp(string n, string c) {
    // vraci true kdyz n je less
    if(c.size() < n.size()) return true;
    if(n.size() < c.size()) return false;
    return lexicographical_compare(c.begin(),c.end(), n.begin(),n.end());
}


vector<State> init_states(size_t size) {
    vector<State> states;
    for (uint i = 0; i < size; ++i) {
        State s;
        s.path_from_start="";
        s.path_to_end="";
        s.id=i;
        states.push_back(s);
    }
    return states;
}

/**
 * ze start stavu do vsech ostatnich stavu
 * zaroven evidovat nejlepsi string spolu se sekvenci
 */
void BFS_from_start(vector<vector<ld_pair>> nfa, vector<struct State>& states) {

    vector<bool> visited(nfa.size(), false);
    queue<uint> Q;

    Q.push(0);
    visited[0] = true;

    while(!Q.empty()) {
        uint curr = Q.front(); Q.pop();
        State curr_state = states[curr];
        for (auto pair : nfa[curr]) {
            char edge = pair.first;
            State neig_state = states[pair.second];

            bool found_better = lex_cmp(neig_state.path_from_start, curr_state.path_from_start + edge);
            if(neig_state.path_from_start.empty() || found_better) {
                neig_state.path_from_start = curr_state.path_from_start + edge;
                neig_state.sequence = curr_state.sequence;
                neig_state.sequence.push_back(curr_state.id);
            }
            /// else { uz tam nejlepsi cesta je }

            states[neig_state.id] = neig_state;
            if (visited[neig_state.id]) continue;
            Q.push(neig_state.id);
            visited[neig_state.id] = true;
        }
    }
    states[0].path_from_start = "";
}

/**
 * final stavu do vsech ostatnich stavu
 * zaroven evidovat nejlepsi string spolu se sekvenci
 */
vector<State>
BFS_from_end(vector<vector<ld_pair>> r_nfa, vector<State>& states, vector<uint> &finals, vector<bool> &is_final) {

    queue<uint> Q;
    for(auto node : finals)
        Q.push(node);

    vector<bool> visited(r_nfa.size(), false);
    while(!Q.empty()) {
        uint curr = Q.front(); Q.pop();
        if (visited[curr]) continue;
        visited[curr] = true;

        State curr_state = states[curr];

        for (auto pair : r_nfa[curr]) {

            char edge = pair.first;
            State neig_state = states[pair.second];

            if(is_final[neig_state.id]) continue; /// nejblizsi cesta z finalu do finalu je sam do sebe

            bool found_better = lex_cmp(neig_state.path_to_end, edge + curr_state.path_to_end);
            if(neig_state.path_to_end.empty() || found_better) {
                neig_state.path_to_end = edge + curr_state.path_to_end;
                neig_state.sequence = curr_state.sequence;
                neig_state.sequence.push_back(curr_state.id);
            }
            /// else { uz tam nejlepsi cesta je }

            states[neig_state.id] = neig_state;
            if (visited[neig_state.id]) continue;
            Q.push(neig_state.id);
        }
    }
    return states;
}

vector<vector<ld_pair>> reverse_nfa(vector<vector<ld_pair>> nfa) {
    vector<vector<ld_pair>> r_nfa(nfa.size());
    vector<ld_pair> src_edges;
    for (uint src = 0; src < nfa.size(); ++src) {
        for (uint d = 0; d < nfa[src].size(); ++d) {
            ld_pair d_pair = nfa[src][d];
            char letter = d_pair.first;
            uint destination = d_pair.second;
            ld_pair new_pair = {letter, src};
            r_nfa[destination].push_back(new_pair);
        }
    }
    return r_nfa;
}


//void DFS_substring_from(vector<vector<ld_pair>>& nfa, uint start,
//                        string& S, vector<vector<uint>>& collector) {
//
//    vector<vector<uint>> sequences;
//    vector<State> states = init_states(nfa.size());
//    queue<uint> Q;
//    Q.push(start);
//
//    while(!Q.empty()) {
//        uint curr = Q.front(); Q.pop();
//        State curr_state = states[curr];
//        if (curr_state.path_from_start.size() == S.size())  {
//            curr_state.sequence.push_back(curr_state.id);
//            sequences.push_back(curr_state.sequence);
//            continue;
//        }
//
//        for (ld_pair pair : nfa[curr]) {
//            char edge = pair.first;
//            State neig_state = states[pair.second];
//
//            // pokud curr.path ma size 2, potrebujeme checknout edge = S[2]
//            char expected = S[curr_state.path_from_start.size()];
//            if(edge != expected) continue; /// neshoda -> nebudeme pushovat
//            else { // edge == expected
//                neig_state.path_from_start = curr_state.path_from_start + edge;
//                neig_state.sequence = curr_state.sequence;
//                neig_state.sequence.push_back(curr_state.id);
//            }
//            /// else { uz tam nejlepsi cesta je }
//            states[neig_state.id] = neig_state;
//            Q.push(neig_state.id);
//        }
//    }
//
//    for (const auto& seq : sequences) {
//        collector.push_back(seq);
//    }
//}


vector<uint> init_start_char_nodes(vector<vector<ld_pair>> nfa, char c) {
    vector<uint> start_char_nodes;
    for (uint s = 0; s < nfa.size(); ++s) {
        for (uint n = 0; n < nfa[s].size(); ++n) {
            ld_pair p = nfa[s][n];
            if(p.first == c) {
                start_char_nodes.push_back(s);
                break; // staci jedna hrana
            }
        }
    }
    return start_char_nodes;
}

/**
 * Overall, the function performs a depth-first search on the input data,
 * looking for the lexicographically smallest string that can be constructed
 * by concatenating the path_from_start field of a State object, the word string,
 * and the path_to_end field of another State object. It updates the final_res string
 * with the smallest such string that it finds.
 */
void DFS(vector<vector<ld_pair>>& nfa, vector<State>& states, ld_pair d_pair , string word, uint start, uint pos) {

    uint last_pos = (uint) word.size() - 1;
    if (pos == last_pos) {

        string curr_res;
        string pref = states[start].path_from_start;
        string suff = states[d_pair.second].path_to_end;
        curr_res = pref + word + suff;

        if (lex_cmp(final_res, curr_res)) {
            final_res = curr_res;
        }
        return;
    }

    for (ld_pair neigh : nfa[d_pair.second]) { // sousede vstupniho nodu
        State s = states[neigh.second];

        char expected = word[pos + 1];
        if (neigh.first == expected) {
            size_t m = max(s.path_to_end.size(), word.size() - (pos+2));
            size_t candidate = states[start].path_from_start.size() + pos+1 + m;
            if (candidate <= final_res.size()) {
                DFS(nfa, states, neigh, word, start, pos + 1);
            }
        }
    }
}



int main() {
    uint N, M;   // N = pocet stavu, M = velikost abecedy
    cin >> N >> M;
    string empty;
    getline(cin, empty);

    vector<bool> is_final(N, false);
    vector<uint> finals;
    vector<vector<ld_pair>> nfa = readNFA(N, is_final, finals);

    string S;
    getline(cin, S);

    vector<State> states = init_states(N);
    BFS_from_start(nfa, states); /// modifikuje states
    vector<vector<ld_pair>> r_nfa = reverse_nfa(nfa);
    BFS_from_end(r_nfa, states, finals, is_final); /// modifikuje states

    vector<uint> start_char_nodes = init_start_char_nodes(nfa, S[0]);


    for(uint src = 0; src < nfa.size(); src++) {
        for(ld_pair d_pair : nfa[src]) {
            if (d_pair.first == S[0]) { // pro kazdou hranu na ktere je start char

                size_t b_len = states[src].path_from_start.size();
                size_t e_len = states[src].path_to_end.size();
                size_t m = max(e_len, S.size());

                /// DFS ze sourcu pokud B+max(E/S) <= current
                if ( (b_len + m) <= final_res.size() ) {
                    DFS(nfa, states, d_pair, S, src, 0);
                }

            }
        }
    }
    cout << final_res << endl;
    return 0;
}

