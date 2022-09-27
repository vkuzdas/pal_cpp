//
// Created by kuzdavoj@fel.cvut.cz on 9/25/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cassert>
#include <algorithm>

using namespace std;
using wd_pair = pair<int, int>;

int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
void assign_districts(vector<vector<pair<int,int>>> &adj, vector<int> &districts, int D);
bool inSet(unordered_set<int> set, int v);

int main() {
    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(R); // src -> { {w1, dest1}, {w2, dest2} }
    getInput(R, adj);

    vector<int> districts(T+1);
    for (int i = 1; i <= D; ++i) { districts[i] = i; } // district belongs to itself
    assign_districts(adj, districts, D);

    // do prim in D subgraphs
    int total_cost = 0;
    for (int i = 1; i <= D; ++i) {
        int cost = prim_mst_per_district(adj, districts, i);
        total_cost = total_cost + cost;
    }
    // do prim in a supergraph

    return 0;
}

int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district) {
    int total_cost = 0;
    int to_visit = int(count(districts.begin(), districts.end(), district));
    unordered_set<int> seen;
    priority_queue<wd_pair, vector<wd_pair>, greater<>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, district});

    // traversal
    // TODO: stopping condition - district[district] count or
    while (seen.size() < to_visit) { // size-1 bcs of non-existent id(0) node
        cout << "size: " << seen.size();
        wd_pair curr = heap.top();
        if (inSet(seen, curr.second)) continue; // skip visited
        heap.pop();
        total_cost = total_cost + curr.first;
        seen.insert(curr.second);

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            if (districts[neighPair.second] != district) continue;// skip dest nodes that are not in this district
            if (inSet(seen, neighPair.second)) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
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

