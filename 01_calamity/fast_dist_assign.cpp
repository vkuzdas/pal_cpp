//
// Created by kuzdavoj@fel.cvut.cz on 9/29/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;
using wd_pair = pair<int, int>;

bool inSet(unordered_set<int> set, int v);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
void fast_assign(vector<vector<pair<int, int>>> &adj, int D);

int main() {
    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(T+1); // src -> { {w1, dest1}, {w2, dest2} }
    getInput(R, adj);
    fast_assign(adj, D);
}

void fast_assign(vector<vector<pair<int, int>>> &adj, int D) {
    vector<int> districts(adj.size());
    vector<int> traversals(adj.size());
    unordered_set<int> seen;
    queue<int> q;
    for (int i = 1; i <= D; ++i) {
        traversals[i] = 0;
        districts[i] = i;
        q.push(i);
    }
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        for(auto nei : adj[curr]) {
            if(!inSet(seen, nei.second)) {
                if(districts[nei.second] == 0) {
                    traversals[nei.second] = traversals[curr] + 1;
                    districts[nei.second] = districts[curr];
    setbuf(stdout, 0); cout << curr; cout << "    " << nei.second; cout << " " << traversals[nei.second];                    cout << "|" << districts[nei.second] <<"\n";
                }
                q.push(nei.second);
                seen.insert(nei.second);
            }
        }
    }
}

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}

void getInput(int R, vector<vector<pair<int,int>>>& adj) {
    for (int i = 0; i < R; ++i) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
}
