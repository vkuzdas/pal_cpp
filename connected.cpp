//
// Created by kuzdavoj@fel.cvut.cz on 12/21/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>


using namespace std;


/// umi cist 07_smug/datapub/*
vector<vector<int>> read_adj(int N, int M) {
    vector<int> m;
    vector<vector<int>> adj(N,m);
    for (int i = 0; i < M; ++i) {
        int src, dst;
        cin >> src >> dst;
        /// neorientovany graf
        adj[src].push_back(dst);
        adj[dst].push_back(src);
    }
    return adj;
}


bool is_subset_connected(vector<bool>& in_subset, vector<int> &subset, vector<vector<int>> &adj) {
    
    queue<int> Q;
    vector<bool> visited(in_subset.size(), false);
    Q.push(subset[0]);

    while (!Q.empty()) {
        int curr = Q.front(); Q.pop();
        visited[curr] = true;

        for(auto neigh : adj[curr]) {
            /// soused v subsetu a nenavstiveny -> do fronty
            if(in_subset[neigh] && !visited[neigh]) {
                Q.push(neigh);
            }
        }
    }

    /// pro kazdej nod v subsetu se mrkni jestli je visited
    for(auto node : subset) {
        if (!visited[node])
            return false;
    }
    return true;
}


void print_choice(vector<int> pick) {
    for (auto i: pick) {
        cout << " " << i;
    }
}

bool next_K_subset(vector<int>& a, int n) {
    int k = (int)a.size();
    for (int i = k - 1; i >= 0; i--) {
        if (a[i] < n - k + i + 1) {
            a[i]++;
            for (int j = i + 1; j < k; j++)
                a[j] = a[j - 1] + 1;
            return true;
        }
    }
    return false;
}

// subset je generovanej
vector<int> adjust_subset(vector<int>& pick) {
    vector<int> p2;
    for (auto i: pick)
        p2.push_back(i-1);
    return p2;
}

int main() {
    /// umi cist 07_smug/datapub/*
    int N,M,P,C; // nody, hrany, smugs, spoje
    cin >> N >> M >> P >> C;
    vector<vector<int>> adj = read_adj(N, M);

    vector<int> helper1;
    for (int i = 1; i < P+1; ++i) {helper1.push_back(i);}

    do {
        vector<int> subset = adjust_subset(helper1);
        
        vector<bool> in_subset(adj.size());
        for(int i : subset) { in_subset[i] = true; }

        if (is_subset_connected(in_subset, subset, adj)) {
            print_choice(subset);
            cout << "    connected" << endl;
        }
    } while (next_K_subset(helper1, N));




    return 0;
}