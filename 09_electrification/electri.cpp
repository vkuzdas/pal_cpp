//
// Created by kuzdavoj@fel.cvut.cz on 12/30/2022.
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


using wd_pair = pair<int, int>;

// nutno postavit least cost electrification
// mezi vesnicema na okraji dovu rek

// dostanes pocet spoju kterej musi vest pres reku
// dostanes hrany mezi vesnicema
// postav nejlevnejsi MST s danym poctem hran pres reku

struct River_edge {
    int src;
    int dest;
    int cost;
    bool operator < (const River_edge& edge) const { // edge je lehci
        if(cost < edge.cost) {
            return true;
        }
        return false;
    }
};


bool common_bank_edge(int src, int dest, int D) {
    return (src <= D && dest <= D) || (src > D && dest > D);
}




int prim_left_bank(vector<vector<wd_pair>> &adj, set<int> &lb_nodes, vector<bool> &visited, int D) {
    int total_cost = 0;

    size_t counter = 0;
    priority_queue<wd_pair, vector<wd_pair>, greater<wd_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, 0});

    // traversal
    while (counter < lb_nodes.size()) {
        wd_pair curr = heap.top(); heap.pop();
        if (visited[curr.second]) continue; // skip visited
        total_cost = total_cost + curr.first;
        counter = counter + 1;
        visited[curr.second] = true;

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            int src = curr.second;
            int dest = neighPair.second;
            if (visited[neighPair.second] || !common_bank_edge(src, dest, D)) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
}

int prim_right_bank(vector<vector<wd_pair>> &adj, set<int> &rb_nodes, vector<bool> &visited, int D) {
    int total_cost = 0;

    size_t counter = 0;
    priority_queue<wd_pair, vector<wd_pair>, greater<wd_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({0, D+1});


    // traversal
    while (counter < rb_nodes.size()) {
        wd_pair curr = heap.top(); heap.pop();
        if (visited[curr.second]) continue; // skip visited
        total_cost = total_cost + curr.first;
        counter = counter + 1;
        visited[curr.second] = true;

        // neighbors
        for (wd_pair neighPair : adj[curr.second]) {
            int src = curr.second;
            int dest = neighPair.second;
            if (visited[neighPair.second] || !common_bank_edge(src, dest, D)) continue; // skip visited
            heap.push(neighPair);
        }
    }
    return total_cost;
}

// M = # direct connections
// N = # vilages
//  0..D = left bank villages
//  D+1..N-1 = right bank villages
// B = # of connections across river
int main () {
    int M,N,D,B;
    cin >> M >> N >> D >> B;


    set<int> unvisited_lb_nodes;
    set<int> unvisited_rb_nodes;
    for (int i = 0; i < N; ++i) {
        if(i <= D) {
            unvisited_lb_nodes.insert(i);
        } else {
            unvisited_rb_nodes.insert(i);
        }
    }

    vector<River_edge> river_edges;
    vector<vector<wd_pair>> adj(N);
    for (int i = 0; i < M; ++i) {
        int src, dest, cost;
        cin >> src >> dest >> cost;
        if ( !common_bank_edge(src, dest, D) ) {
            river_edges.push_back({src, dest, cost});
            if(src <= D) {
                unvisited_lb_nodes.erase(src);
                unvisited_rb_nodes.erase(dest);
            } else {
                unvisited_lb_nodes.erase(dest);
                unvisited_rb_nodes.erase(src);
            }
        }
        adj[src].push_back({cost, dest});
        adj[dest].push_back({cost, src});
    }


    sort(river_edges.begin(), river_edges.end());
    vector<bool> visited(N, false);
    int cost = 0;
    for (int i = 0; i < B; ++i) {
        River_edge e = river_edges[i];
        cost += e.cost;
        visited[e.src] = true;
        visited[e.dest] = true;
    }


    // pro kazdej node na leve strane
    cost += prim_left_bank(adj, unvisited_lb_nodes, visited, D);
    // pro kazdej node na prave strane
    cost += prim_right_bank(adj, unvisited_rb_nodes, visited, D);


//    cost += prim_mst(adj, visited, D);

    cout << cost << endl;
    /// ve funkci detekuj kterej edge je pres reku, tenhle vyrad z river edges
    /// potom se mrkni do river edges a pridavej nejkratsi dokud nenaplnis B edges



}