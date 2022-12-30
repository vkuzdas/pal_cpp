//
// Created by kuzdavoj@fel.cvut.cz on 12/30/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <sstream>
#include <map>


using namespace std;



//S, A, F, P, N, L
//  4 2 2 2 2 9
//  0 1 3
//  1 1 3
//  2 2 0
//  3 2 0
//  aaaaaaaba
//  abaaaabbb
//  aaaaaabbb
//  aaaaabbaa


///  (S)  =  # states of the automaton
///  (A)  =  size of  alphabet
///  (F)  =  # of final states
///  (P)  =  # of positive examples
///  (N)  =  # of negative examples
///  (L)  =  length

int simulate_atmt(int start, const string& text, const vector<vector<pair<char, int>>>& adj) {
    int curr_state = start;
    for(char c : text) {

        // char to index conversion
        int index = c - 'a';

        vector<pair<char, int>> neighs = adj[curr_state];
        pair<char, int> nxt = neighs[index];
        int nxt_state = nxt.second;
        curr_state = nxt_state;
    }

    return curr_state;
}

bool DBG = true;

int main () {
    int S,A,F,P,N,L;
    cin >> S >> A >> F >> P >> N >> L;
    vector<vector<pair<char,int>>> adj; // src -> {'a',dest}
    for (int i = 0; i < S; ++i) {
        int src; // mozna neni potreba
        cin >> src;
        vector<pair<char, int>> neighs;
        for (int j = 0; j < A; ++j) {
            int dest;
            cin >> dest;
            char c = 'a' + j;
            neighs.push_back({c, dest});
        }
        adj.push_back(neighs);
    }
    vector<string> all_strings;
    vector<string> pos;
    for (int i = 0; i < P; ++i) {
        string s;
        cin >> s;
        pos.push_back(s); all_strings.push_back(s);
    }
    vector<string> neg;
    for (int i = 0; i < N; ++i) {
        string s;
        cin >> s;
        neg.push_back(s); all_strings.push_back(s);
    }


    // pro node, simuluj jeho respective string
    // outputni kde konci
    set<int> neg_ends;
    for (int i = 0; i < S; ++i) neg_ends.insert(i);
    set<int> pos_ends;


    for (int start = 0; start < all_strings.size(); ++start) {
        string text = all_strings[start];
        bool pos = start < P;
        int end = simulate_atmt(start, text, adj);
        if(pos) {
            pos_ends.insert(end);
            neg_ends.erase(end);
        }
//        if (!pos) {
//            // negative case
//            cout << start << " ";
//            for (int i = 0; i < S; ++i) {
//                if(i == start) continue;
//                if(i == end) continue;
//                cout << i << " ";
//            }
//        }
//        cout << endl;

        if(DBG) {
            string should = pos ? " should" : " shouldnt";
            cout << text << " from " << start << should + " end in " << end << endl;
        }
    }
    for (int i = 0; i < S; ++i) {
        cout << i << " ";
        if(i < P ) {
            // positive cases
            for(int st : pos_ends) {
                cout << st << " ";
            }
        } else {
            // negative cases
            for(int st : neg_ends) {
                cout << st << " ";
            }
        }
        cout << endl;
    }
}














