//
// Created by kuzdavoj@fel.cvut.cz on 9/25/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
using wd_pair = pair<int, int>;

int prim_mst_per_district(vector<vector<wd_pair>> &adj, vector<int> &districts, int district);
int prim_mst(vector<vector<wd_pair>> &adj, int startV);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
void condense_graph(vector<vector<wd_pair>> &adj, vector<int> &districts, int D);
vector<int> assign_districts(vector<vector<pair<int, int>>> &adj, int D);


int main() {
    int T, D, R;
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(T+1); // src -> { {w1, dest1} }
    getInput(R, adj);

    // BFS district assign
    vector<int> districts = assign_districts(adj,  D);

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
    for (size_t src = 1; src < adj.size(); ++src) {
        for (auto &nei_pair : adj[src]) {
            nei_pair.second = districts[nei_pair.second];
        }
    }
    // odstranit edges ktere vedou d1 -> d1
    for (size_t src = 1; src < adj.size(); ++src) {
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
    for (size_t src = adj.size()-1; src > size_t(D); --src) { // from last src to last D
        for (auto nei_pair : adj[src]) {
            int src_dist = districts[src];
            adj[src_dist].push_back(nei_pair);
        }
        adj.erase(adj.begin() + int(src));
    }
}

/**
 * Jarnik/Prim algorithm with Min-Heap running in O(m log n)
 */
int prim_mst(vector<vector<wd_pair>> &adj, int startV) {
    int total_cost = 0;
    size_t counter = 0;
    vector<bool> seenBool(adj.size(), false);
    priority_queue<wd_pair, vector<wd_pair>, greater<wd_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, startV});

    // traversal
    while (counter < adj.size()-1) { // size-1 bcs of non-existent id(0) node
        wd_pair curr = heap.top(); heap.pop();
        if (seenBool[curr.second]) continue; // skip visited
        total_cost = total_cost + curr.first;
        counter = counter + 1; seenBool[curr.second] = true;

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            if (seenBool[neighPair.second]) continue; // skip visited
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
    size_t counter = 0;
    size_t to_visit = count(districts.begin(), districts.end(), district);
    vector<bool> seenBool(adj.size(), false);
    priority_queue<wd_pair, vector<wd_pair>, greater<wd_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, district});

    // traversal
    while (counter != to_visit) {
        wd_pair curr = heap.top();
        heap.pop();
        if (seenBool[curr.second]) continue; // skip visited
        total_cost = total_cost + curr.first;
        counter = counter + 1; seenBool[curr.second] = true;

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            if (districts[neighPair.second] != district) continue;// skip dest nodes that are not in this district
            if (seenBool[neighPair.second]) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
}

/**
 * Assigns district to every city based on city's proximity to district city
 *      (or based on which district has lower id in case proximity is same)
 */
vector<int> assign_districts(vector<vector<pair<int, int>>> &adj, int D) {
    // setup
    vector<int> districts(adj.size());
    vector<int> traversals(adj.size());
    queue<int> q;
    // add all districts to the queue
    // lowest city is guaranteed to get to the closest first
    for (int i = 1; i <= D; ++i) {
        traversals[i] = 0;
        districts[i] = i;
        q.push(i);
    }

    // BFS through graph from all district nodes
    //   assign depth (from district node) to each node
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        for(auto nei : adj[curr]) {
                if(districts[nei.second] == 0) {
                    traversals[nei.second] = traversals[curr] + 1;
                    districts[nei.second] = districts[curr];
                    q.push(nei.second);
                }
        }
    }
    return districts;
}

void getInput(int R, vector<vector<pair<int,int>>>& adj) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    for (int i = 0; i < R; ++i) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
}

