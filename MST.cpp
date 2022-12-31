//
// Created by kuzdavoj@fel.cvut.cz on 12/31/2022.
//



#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <sstream>
#include <map>
#include <stack>


using namespace std;

///  #######################
///  ###  Jarnik / Prim  ###
///  #######################


using wd_pair = pair<int, int>;

int prim_mst(vector<vector<wd_pair>> &adj, int startV) {
    int total_cost = 0;
    size_t counter = 0;
    vector<bool> visited(adj.size(), false);
    priority_queue<wd_pair, vector<wd_pair>, greater<wd_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, startV});

    // traversal
    while (counter < adj.size()-1) { // size-1 bcs of non-existent id(0) node
        wd_pair curr = heap.top(); heap.pop();
        if (visited[curr.second]) continue; // skip visited
        total_cost = total_cost + curr.first;
        counter = counter + 1; visited[curr.second] = true;

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            if (visited[neighPair.second]) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
}








///  #################
///  ###  Kruskal  ###
///  #################



struct Edge {
    int src;
    int dest;
    int cost;

    Edge(int src, int dest, int cost) : src(src), dest(dest), cost(cost) {}

    bool operator < (Edge const& other) const  {
        return cost < other.cost;
    }

};

void kruskal_make_set(vector<int> &parent, vector<int> &rank, int v) {
    parent[v] = v;
    rank[v] = 0;
}

int kruskal_find_set(vector<int> &parent, vector<int> &rank, int v) {
    if (v == parent[v])
        return v;
    return parent[v] = kruskal_find_set(parent, rank, parent[v]);
}

void kruskal_union_sets(vector<int> &parent, vector<int> &rank, int a, int b) {
    a = kruskal_find_set(parent, rank, a);
    b = kruskal_find_set(parent, rank, b);
    if (a != b) {
        if (rank[a] < rank[b])
            swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b])
            rank[a]++;
    }
}

/// sort all the edges of the graph
/// put each vertex in its own tree (i.e. its set) via calls to the make_set function
/// iterate through all the edges
/// for each edge determine whether the ends belong to different trees (with two find_set calls)
/// Finally, perform the union of the two trees
/// for which the union_sets function will be called

int kruskal (vector<Edge> &edges) {

    vector<int> parent;
    vector<int> rank;
    int n = edges.size();

    int cost = 0;
    vector<Edge> result;
    parent.resize(n);
    rank.resize(n);
    for (int i = 0; i < n; i++)
        kruskal_make_set(parent, rank, i);

    sort(edges.begin(), edges.end());

    for (Edge e : edges) {

        int src_set = kruskal_find_set(parent, rank, e.src);
        int dest_set = kruskal_find_set(parent, rank, e.dest);

        if (src_set != dest_set) {
            cost += e.cost;
            result.push_back(e);
            kruskal_union_sets(parent, rank, e.src, e.dest);
        }
    }
    return cost;
}


///  ##############
///  ###  MAIN  ###
///  ##############

int main() {
    vector<Edge> es = {{0,1,3}, {1,2,1}, {0,3,5},{2,3,1}};
    cout << kruskal(es);
}
