//
// Created by kuzdavoj@fel.cvut.cz on 9/25/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;
using wd_pair = pair<int, int>;

int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district);
int prim_mst(vector<vector<wd_pair>> &adj, int startV);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
bool inSet(unordered_set<int> set, int v);
void condense_graph(vector<vector<wd_pair>> &adj, vector<int> &districts, int D);
vector<int> fast_assign(vector<vector<pair<int, int>>> &adj, int D);

int main() {
    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(T+1); // src -> { {w1, dest1}, {w2, dest2} }
    getInput(R, adj);

    vector<int> districts = fast_assign(adj,  D);

    // do prim in D subgraphs
    int total_cost = 0;
    for (int i = 1; i <= D; ++i) {
        int cost = prim_mst_per_district(adj, districts, i);
        total_cost = total_cost + cost;
    }

    // do prim in the supergraph
    condense_graph(adj, districts, D);
    total_cost = total_cost + prim_mst(adj, 1);
    cout << total_cost;
    return 0;
}


/**
 * Graph condensation
 *      merges all nodes from a district into a single node denoted by district ID
 *      edges within the district are erased
 *      d1 -> d2 edges are preserved
 */
void condense_graph(vector<vector<wd_pair>> &adj, vector<int> &districts, int D) {
    // kondenzace grafu:
    // nahradit destination za district (dest nalezi distu)
    for (auto src = 1; src < adj.size(); ++src) {
        for (auto &nei_pair : adj[src]) {
            nei_pair.second = districts[nei_pair.second];
        }
    }
    // odstranit edges ktere vedou d1 -> d1
    for (auto src = 1; src < adj.size(); ++src) {
        vector<wd_pair> replacement;
        for (wd_pair p : adj[src]) {
            int dest = p.second;
            int dest_dist = districts[dest];
            int src_dist = districts[src];
            if (dest_dist != src_dist) {
                replacement.push_back(p);
            }
        }
        adj[src] = replacement;
    }
    // move edge from node belonging to dist to original dist node
    for (auto src = adj.size()-1; src > D; --src) { // from last src to last D
        for (auto nei_pair : adj[src]) {
            int src_dist = districts[src];
            adj[src_dist].push_back(nei_pair);
        }
        adj.erase(adj.begin() + src);
    }
}

int prim_mst(vector<vector<wd_pair>> &adj, int startV) {
    int total_cost = 0;
    unordered_set<int> seen;
    priority_queue<wd_pair, vector<wd_pair>, greater<>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, startV});

    // traversal
    while (seen.size() < adj.size()-1) { // size-1 bcs of non-existent id(0) node
        wd_pair curr = heap.top(); heap.pop();
        if (inSet(seen, curr.second)) continue; // skip visited
        total_cost = total_cost + curr.first;
        seen.insert(curr.second);

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            if (inSet(seen, neighPair.second)) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
}

/**
 * Runs Prim with heap within a district O((V+E)logV)
 */
int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district) {
    int total_cost = 0;
    auto to_visit = int(count(districts.begin(), districts.end(), district));
    unordered_set<int> seen;
    priority_queue<wd_pair, vector<wd_pair>, greater<>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, district});

    // traversal
    while (seen.size() != to_visit) {
        wd_pair curr = heap.top();
        heap.pop();
        if (inSet(seen, curr.second)) continue; // skip visited
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

vector<int> fast_assign(vector<vector<pair<int, int>>> &adj, int D) {
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
                }
                q.push(nei.second);
                seen.insert(nei.second);
            }
        }
    }
    return districts;
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

