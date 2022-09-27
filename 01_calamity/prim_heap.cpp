//
// Created by kuzdavoj@fel.cvut.cz on 9/26/2022.
//

#include <queue>
#include <unordered_set>
#include <iostream>
#include <cassert>

using namespace std;
using wd_pair = pair<int, int>;

int prim_mst(vector<vector<wd_pair>> &adj, int startV);
bool inSet(unordered_set<int> set, int v);

int main() {
    vector<vector<wd_pair>> adj{
        {},
        {{1,2}, {1,3}, {1,4}},
        {{1,1}, {6,5}},
        {{1,1}, {2,5}},
        {{1,1}, {5,6}},
        {{2,3}, {1,7}},
        {{1,7}, {5,4}},
        {{1,5}, {1,6}}
    };
    int r = prim_mst(adj, 1);
    assert(r == 7);
    cout << r;
}
/**
 * input: adjList graph in the form of
 *          src -> {{weight, destination}, {..}}
 * output: integer value of MST
 */
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

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}





