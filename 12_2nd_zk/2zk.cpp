//
// Created by kuzdavoj@fel.cvut.cz on 1/16/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <climits>
#include <bits/stdc++.h>

using namespace std;


int main() {
    int N; // # servery
    int C; // # planovane spoje tzn B ma C+1, A ma C-1
    int F; // # fast servery

    cin >> N >> C >> F;
    vector<vector<int>> A_adj(N);
    vector<vector<int>> B_adj(N);

    vector<bool> A_is_fast(N, false);
    vector<int> A_fast_list;

    vector<bool> B_is_fast(N, false);
    vector<int> B_fast_list;

    for (int i = 0; i < F; ++i) {
        int f;
        cin >> f;
        A_is_fast[f] = true;
        A_fast_list.push_back(f);
    }

    for (int i = 0; i < C-1; ++i) {
        int src;
        int dst;
        cin >> src >> dst;
        A_adj[src].push_back(dst);
        A_adj[dst].push_back(src);
    }

    for (int i = 0; i < F; ++i) {
        int f;
        cin >> f;
        B_is_fast[f] = true;
        B_fast_list.push_back(f);
    }

    for (int i = 0; i < C+1; ++i) {
        int src;
        int dst;
        cin >> src >> dst;
        B_adj[src].push_back(dst);
        B_adj[dst].push_back(src);
    }

    cout << "done";
}