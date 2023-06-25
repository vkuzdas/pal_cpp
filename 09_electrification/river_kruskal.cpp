//
// Created by kuzdavoj@fel.cvut.cz on 1/15/2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <climits>
#include <bits/stdc++.h>

using namespace std;



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

int kruskal(vector<Edge> &edges, vector<Edge> rvr, vector<int> pick) {

    vector<int> parent;
    vector<int> rank;
    int n = edges.size();

    int cost = 0;
    vector<Edge> result;
    parent.resize(n);
    rank.resize(n);
    // U-F init
    for (int i = 0; i < n; i++)
        kruskal_make_set(parent, rank, i);

    /// picknute hrany nejdrive spojime do setu a updatneme cost
    for(auto i : pick) {
        Edge e = rvr[i];
        cost += e.cost;
        result.push_back(e);
        kruskal_union_sets(parent, rank, e.src, e.dest);
    }

    // hlavni iterace kruskala pres zbyle edges
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




/// Villages 0,..,D are located on the left bank
/// Villages D+1,..,N−1 are located on the right bank
bool is_rvr(int src, int dst, int D) {
    if(src <= D && dst <= D) // left bank both
        return false;
    else if(src > D && dst > D) // right bank both
        return false;
    return true;
}

bool next_combination(vector<int>& a, int n) {
    int k = (int)a.size();
    for (int i = k - 1; i >= 0; i--) {
        if (a[i] < n - k + i + 1) {
            a[i]++;
            for (int j = i + 1; j < k; j++)
                a[j] = a[j - 1] + 1;
            return true;
        }
    }
    return false;
}

void print_rvr_choice(const vector<int> &pick, const vector<struct Edge>& rvr) {
    cout << "\nPicking " << pick.size() << " from " << rvr.size() << " edges: " << endl;
    for (auto i: pick)
        cout << rvr[i].src << "-" << rvr[i].dest << "    ";
    cout << endl;
}

int main() {
    int M,N,D,B;
    cin >> M >> N >> D >> B;
    vector<Edge> rvr;
    vector<Edge> all;
    for (int i = 0; i < M; ++i) {
        int src, dst, wgh;
        cin >> src >> dst >> wgh;
        all.emplace_back(src, dst, wgh);
        if(is_rvr(src, dst, D))
            rvr.emplace_back(src, dst, wgh);
    }

    // ted musis iterovat vybery B hran z rvr
    vector<int> pick(B);
    std::iota(pick.begin(), pick.end(), 0);
//    print_rvr_choice(pick, rvr);
    int cost = INT32_MAX;
    do {
//        print_rvr_choice(pick, rvr);
        cost = min(cost, kruskal(all, rvr, pick));
    } while(next_combination(pick, (int)rvr.size() - 1));




    cout << cost;

}