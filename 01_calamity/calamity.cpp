//
// Created by kuzdavoj@fel.cvut.cz on 9/25/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <stack>

using namespace std;
using wd_pair = pair<int, int>;

int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district);
int prim_mst(vector<vector<wd_pair>> &adj, int startV);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
void assign_districts(vector<vector<pair<int,int>>> &adj, vector<int> &districts, int D);
bool inSet(unordered_set<int> set, int v);
void condense_graph(vector<vector<wd_pair>> &adj, vector<int> &districts, int D);
vector<int> dfs(vector<vector<wd_pair>> &adj);

int main() {
    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(T+1); // src -> { {w1, dest1}, {w2, dest2} }
    getInput(R, adj);

    vector<int> districts(T+1);
    for (int i = 1; i <= D; ++i) { districts[i] = i; } // district belongs to itself
    dfs(adj);
//    assign_districts(adj, districts, D);
//
//    // do prim in D subgraphs
//    int total_cost = 0;
//    for (int i = 1; i <= D; ++i) {
//        int cost = prim_mst_per_district(adj, districts, i);
//        total_cost = total_cost + cost;
//    }
//
//    // do prim in the supergraph
//    condense_graph(adj, districts, D);
//    total_cost = total_cost + prim_mst(adj, 1);
//    cout << total_cost;
    return 0;
}

vector<int> dfs(vector<vector<wd_pair>> &adj) {
    vector<int> path;
    unordered_set<int> seen;

    for (int startV = 1; startV < adj.size(); ++startV) { // pro uzly v grafu
        if (inSet(seen, startV)) continue;
        stack<int> s({startV});
        while (!s.empty()) { // pro uzly na stacku
            int curr = s.top();
            s.pop();
            seen.insert(curr);
            path.push_back(curr);
            for (auto p : adj[curr]) {
                if (inSet(seen, p.second)) continue; // neighbor not seen yet (not found in seen)
                s.push(p.second);
            }
            cout << " curr: " << curr << " seen: " << seen.size() << " s: " << s.size() << "\n";
        }
    }
    return path;
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
        wd_pair curr = heap.top();
        if (inSet(seen, curr.second)) continue; // skip visited
        heap.pop();
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

