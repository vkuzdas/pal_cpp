//
// Created by kuzdavoj@fel.cvut.cz on 1/26/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <queue>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;
using dw_pair = pair<int, int>;


struct Node {
    int id;
    int depth;
    bool prim_counted;

    Node(int id, int depth, bool primCounted) : id(id), depth(depth), prim_counted(primCounted) {}
};



void condense_graph(vector<vector<dw_pair>> &adj, vector<int> &districts, int D) {
    // kondenzace grafu:
    // nahradit destination za district (dest nalezi distu)
    for (size_t src = 1; src < adj.size(); ++src) {
        for (auto &nei_pair : adj[src]) {
            nei_pair.second = districts[nei_pair.second];
        }
    }

    // odstranit edges ktere vedou d1 -> d1
    for (size_t src = 1; src < adj.size(); ++src) {
        vector<dw_pair> replacement;
        for (dw_pair p : adj[src]) {
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
 * Priradi district do kazdeho mesta na zaklade vzdalenosti k district mestu
 *      (nebo dle toho kterej district ma nizsi ID pokud vzdalenost je stejna)
 */
pair<int, vector<int>> BFS_distance(vector<vector<pair<int, int>>> &adj, int from) {
    // setup
    int max_depth = 0;
    vector<vector<int>> node_of_depth;
    vector<bool> visited(adj.size(), false);
    vector<int> depths(adj.size(), INT32_MAX);

    queue<int> Q;
    Q.push(from);
    depths[from] = 0;
    visited[from] = true;


    while (!Q.empty()) {
        int curr = Q.front(); Q.pop();
        for(auto neighbor_wd : adj[curr]) {
            int neigh_id = neighbor_wd.first;
            if (!visited[neigh_id]) {
                depths[neigh_id] = depths[curr] + 1;
                max_depth = max(depths[curr] + 1, max_depth);
                Q.push(neigh_id);
                visited[neigh_id] = true;
            }
        }
    }
    return {max_depth, depths};
}


pair<int, vector<int>> prim_mst_per_depth(vector<vector<dw_pair>> &adj, vector<Node> &nodes, Node &from, int curr_depth, vector<int> &depths, vector<bool> &prim_visited) {
    int total_cost = 0;
    vector<int> visited_in_iteration;
    vector<bool> seenBool(adj.size(), false);
    priority_queue<dw_pair, vector<dw_pair>, greater<dw_pair>> heap; /// MIN-heap (PQ is max heap by default)
    heap.push({from.id, 0});

    // traversal
    while (!heap.empty()) {
        dw_pair curr = heap.top(); heap.pop();

        if (seenBool[curr.first]) continue; // skip visited
        total_cost = total_cost + curr.second;
        visited_in_iteration.push_back(curr.first);
        prim_visited[curr.first] = true;
        seenBool[curr.first] = true;

        // sousede
        for (dw_pair neighPair : adj[curr.first]) {
            if (seenBool[neighPair.first]) continue; // skip visited

            // pokud soused ma stejnou hloubku od centra
            if (depths[neighPair.first] == curr_depth) {
                heap.push(neighPair);
            }
        }
    }
    return {total_cost, visited_in_iteration};
}


vector<vector<dw_pair>>
condense_comps(vector<vector<dw_pair>> &adj, vector<vector<int>> &list_of_comps, vector<int> &depths, int N) {
    vector<int> comp_of(N+1, INT32_MAX);

    vector<vector<dw_pair>> condensed(list_of_comps.size());
    for (int comp_num = 0; comp_num < list_of_comps.size(); ++comp_num) {
        for (int j = 0; j < list_of_comps[comp_num].size(); ++j) {
            int node_id = list_of_comps[comp_num][j];
            comp_of[node_id] = comp_num;

//            auto neighs = adj[node_id];
//            for(auto n : neighs) {
//                condensed[comp_num].push_back({comp_of[n.first], n.second});
//            }
        }
    }

    for (int comp_num = 0; comp_num < list_of_comps.size(); ++comp_num) {
        for (int j = 0; j < list_of_comps[comp_num].size(); ++j) {
            int node_id = list_of_comps[comp_num][j];
            auto neighs = adj[node_id];
            for(auto n : neighs) {
                condensed[comp_num].push_back({comp_of[n.first], n.second});
            }
        }
    }

    return condensed;
}



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




int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<dw_pair>> adj(N+1);

    for (int i = 1; i < M+1; ++i) {
        int src, dst, wght;
        cin >> src >> dst >> wght;
        adj[src].push_back({dst, wght});
        adj[dst].push_back({src, wght});
    }




    // pro kazdej node udelam BFS a zjistim kdy je nejlepsi ocislovani komponent
    // ve smyslu ze chci redukovat vzdalenost od from uzlu
    int depth_from_center = INT32_MAX;
    vector<int> depths;
    int center = -1;
    for (int from = 1; from < N + 1; ++from) {

        pair<int, vector<int>> pair = BFS_distance(adj, from);

        int curr_max_depth = pair.first;
        vector<int> curr_max_depths = pair.second;

        if (curr_max_depth < depth_from_center) {
            center = from; /// NASLI JSME CENTRUM
            depth_from_center = curr_max_depth;
            depths = curr_max_depths;
        }
    }



    /// ROZDELIME NODY PODLE HLOUBKY
    vector<vector<Node>> node_of_depth(depth_from_center+1);
    for (int i = 1; i < depths.size(); ++i) {
        int id = i;
        int depth = depths[i];
        node_of_depth[depth].push_back({id, depth, false});
    }








    int cost = 0;
    vector<bool> prim_visited(N, false);
    vector<vector<int>> list_of_comps;
    list_of_comps.push_back({center});
    for (int curr_depth = 1; curr_depth < depth_from_center + 1; ++curr_depth) {
        vector<Node> nodes = node_of_depth[curr_depth];
        for (Node n : nodes) {
            if(!prim_visited[n.id]) {
                auto p = prim_mst_per_depth(adj, nodes, n, curr_depth, depths, prim_visited);
                list_of_comps.push_back(p.second);
                cost += p.first;
            }
        }
    }

    vector<vector<dw_pair>> condensed = condense_comps(adj, list_of_comps, depths, N);

    // nyni prim pro spojeni



    cout << "center is: " << center << endl;
    cout << "cost is: " << cost;





}
