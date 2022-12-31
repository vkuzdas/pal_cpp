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




struct Edge {
    int src;
    int dest;
    int cost;
    bool on_river;

    bool operator < (Edge const& other) const  {
        return cost < other.cost;
    }

};


bool common_bank_edge(int src, int dest, int D) {
    return (src <= D && dest <= D) || (src > D && dest > D);
}



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


bool same_component(vector<int> parents) {
    for (int i = 1; i < parents.size(); ++i) {
        if (parents[i] != parents[0]) return false;
    }
    return true;
}

int main () {
    int M,N,D,B;
    cin >> M >> N >> D >> B;
    vector<Edge> edges;
    for (int i = 0; i < M; ++i) {
        Edge e;
        e.on_river = false;
        cin >> e.src >> e.dest >> e.cost;
        if ( !common_bank_edge(e.src, e.dest, D) ) {
            e.on_river = true;
        }
        edges.push_back(e);
    }

    vector<Edge> result;

    sort(edges.begin(), edges.end());

    vector<int> parent(N);
    vector<int> rank(N);
    for (int i = 0; i < N; i++)
        kruskal_make_set(parent, rank, i);

    int total_cost = 0;
    int b = 0;
    for (Edge edge : edges) {
        if(!edge.on_river) continue;

        total_cost += edge.cost;
        result.push_back(edge);

        int src_set = kruskal_find_set(parent, rank, edge.src);
        int dest_set = kruskal_find_set(parent, rank, edge.dest);

        if (src_set != dest_set) {
            kruskal_union_sets(parent, rank, edge.src, edge.dest);
        }
        b++;
        if (b == B) break;
    }

    // dale potrebujeme pickovat nejmensi ceny dokud vsechny uzly nejsou v jedne komponente
    int m = 0;
    for(Edge edge : edges) {
        if(edge.on_river) continue;

        int src_set = kruskal_find_set(parent, rank, edge.src);
        int dest_set = kruskal_find_set(parent, rank, edge.dest);

        if (src_set != dest_set) {
            result.push_back(edge);
            total_cost += edge.cost;
            m++;
            kruskal_union_sets(parent, rank, edge.src, edge.dest);
        }
        if (m == M-1) break;
    }



    cout << total_cost << endl;



}