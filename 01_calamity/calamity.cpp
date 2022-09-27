//
// Created by kuzdavoj@fel.cvut.cz on 9/25/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cassert>

using namespace std;

void getInput(int R, vector<vector<pair<int,int>>>& adj);
void assign_districts(vector<vector<pair<int,int>>> &adj, vector<int> &districts, int D);
bool inSet(unordered_set<int> set, int v);

int main() {
    vector<vector<pair<int,int>>> adj{
        {{}},
        {{5,3}, {4,2}},
        {{3,4}, {4,1}},
        {{5,5},{4,4}, {5,1}},
        {{3,6}, {3,2}, {4,3}},
        {{2,6}, {5,3}},
        {{3,4}, {2,5}}
    };
    int T = 6, D = 2, R = 7;
//    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
//    cin >> T >> D >> R;
//    vector<vector<pair<int,int>>> adj(R); // src -> { {w1, dest1}, {w2, dest2} }
//    getInput(R, adj);

    vector<int> districts(R);
    for (int i = 1; i <= D; ++i) { districts[i] = i; } // district belongs to itself
    assign_districts(adj, districts, D);
    vector<int> dexp = {0,1,2,1,2,1,2};
    assert(districts == dexp);

    cout << "  --hello\n";
    return 0;
}

void getInput(int R, vector<vector<pair<int,int>>>& adj) {
    for (int i = 0; i < R; ++i) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        if (src > dest) {
            int temp = src;
            src = dest;
            dest = temp;
        }
        adj[src].push_back({weight, dest});
    }
}

/**
 * modified BFS O(n+m)
 *   input: graph with "district" nodes
 *   output: nodes get assigned to given district nodes by their total edge count distance
 */
void assign_districts(vector<vector<pair<int,int>>> &adj, vector<int> &districts, int D) {
    unordered_set<int> seen;
    vector<int> traversals(adj.size(), INT32_MAX);

    for (int dTown = 1; dTown <= D; ++dTown) { // pro district uzly v grafu
        if (inSet(seen, dTown)) continue;

        vector<int> level(adj.size(),0);
        queue<int> q({dTown});
        while (!q.empty()) { // pro uzly ve fronte
            int curr = q.front();
            q.pop();
            seen.insert(curr);
            for (pair<int,int> p : adj[curr]) {
                int neighbor = p.second;
                if (inSet(seen, neighbor)) continue; // neighbor not seen yet (not found in seen)
                if (neighbor <= D) continue; // ignore other district towns
                level[neighbor] = level[curr] + 1;
                if (traversals[neighbor] > level[neighbor]) {
                    traversals[neighbor] = level[neighbor];
                    districts[neighbor] = dTown;
                }
                q.push(neighbor);
            }
        }
        seen.clear();
    }
    vector<int> texp = {INT32_MAX, INT32_MAX,INT32_MAX,1,1,2,2};
    assert(traversals == texp);
}

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}
