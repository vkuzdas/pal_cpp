//
// Created by kuzdavoj@fel.cvut.cz on 12/30/2022.
//

#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

struct Edge {
    int src;
    int dest;
    int cost;

    Edge(int src, int dest, int cost) : src(src), dest(dest), cost(cost) {}

    bool operator < (const Edge &e) const {
        return cost < e.cost;
    }

    bool operator > (const Edge &e) const {
        return cost > e.cost;
    }
};

struct Node {
    int parrent;
    int capacity;
    int comp;
    vector<int> neib;
};


int find(vector<Node> &nodes, int vert) {
    if (nodes[vert].parrent == vert) {
        return vert;
    }
    return nodes[vert].parrent = find(nodes, nodes[vert].parrent);
}

void unite_all(vector<Node> &nodes, int &counter, Edge e, bool force, int &total_cost) {
    // not same commponent
    int cost = e.cost;
    int src = e.src;
    int dest = e.dest;
    int src_prt = find(nodes, src);
    int dest_prt = find(nodes, dest);

    if (src_prt != dest_prt) {
        if (src_prt < dest_prt) {
            nodes[dest_prt].parrent = src_prt;
            nodes[src_prt].capacity += nodes[dest_prt].capacity;
        } else {
            nodes[src_prt].parrent = dest_prt;
            nodes[dest_prt].capacity += nodes[src_prt].capacity;
        }
        total_cost += cost;
        --counter;
    }
    if (src_prt == dest_prt && force) {
        total_cost += cost;
        --counter;
    }
}

void dfs(vector<Node> &nodes, int curr, int comp) {
    nodes[curr].comp = comp;
    for (auto neigh: nodes[curr].neib)
        if (nodes[neigh].comp == -1)
            dfs(nodes, neigh, comp);
}
/// true pokud odlisne komponenty
bool unite_comp(vector<Node> &comp_nodes, int src_comp, int dest_comp) {
    // not same commponent
    int src_prt = find(comp_nodes, src_comp);
    int dest_prt = find(comp_nodes, dest_comp);

    if (src_prt != dest_prt) {
        if (src_prt < dest_prt) {
            comp_nodes[dest_prt].parrent = src_prt;
            comp_nodes[src_prt].capacity += comp_nodes[dest_prt].capacity;
        } else {
            comp_nodes[src_prt].parrent = dest_prt;
            comp_nodes[dest_prt].capacity += comp_nodes[src_prt].capacity;
        }
        return true;
    }
    return false;
}

int main() {

    int M, N, D, B;
    cin >> M >> N >> D >> B;

    int counter;
    int comp = 0;
    counter = N;


    Node tmp;
    tmp.neib = vector<int>();
    tmp.comp = -1;
    tmp.capacity = 1;
    vector<Node> all_nodes = vector<Node>(N, tmp);


/// INIT
    priority_queue<Edge, vector<Edge>, greater<>> river_edges_PQ;
    priority_queue<Edge, vector<Edge>, greater<>> other_edges_PQ;
    for (int i = 0; i < M; i++) {
        int src, dest, cost;
        cin >> src >> dest >> cost;

        // init Union find struct
        all_nodes[src].parrent = src;
        all_nodes[dest].parrent = dest;

        // check if is the river edge
        Edge tmp(src, dest, cost);

        bool on_river = src <= D && dest > D || src > D && dest <= D;
        if (on_river) {
            river_edges_PQ.push(tmp);
        } else {
            /// řeka tvoří řez grafem
            /// řez může vytvořit několik souvislých podgrafů
            other_edges_PQ.push(tmp);
            all_nodes[src].neib.push_back(dest);
            all_nodes[dest].neib.push_back(src);
        }
    }

    /// DFSkem najdeme a spocteme vsechny podgrafy / komponenty

    vector<Node> comp_nodes = vector<Node>();
    // find Components
    for (int curr = 0; curr < N; curr++) {
        // z kazdeho nenavstiveneho uzle udelej DFS
        if (all_nodes[curr].comp == -1) {
            dfs(all_nodes, curr, comp);

            Node node;
            node.parrent = comp;
            node.capacity = 1;
            comp_nodes.push_back(node);
            comp += 1;
        }
    }


    int total_cost = 0;

    /// beru river edges od nejmensiho
    /// pokud je edge v jine komponente, pridam
    priority_queue<Edge, vector<Edge>, greater<>> river_edges_PQ1;
    while (!river_edges_PQ.empty()) {

        Edge e = river_edges_PQ.top();
        river_edges_PQ.pop();
        int src_comp = all_nodes[e.src].comp;
        int dest_comp = all_nodes[e.dest].comp;

        if (unite_comp(comp_nodes, src_comp, dest_comp)) {
            unite_all(all_nodes, counter, e, true, total_cost);
        } else {
            river_edges_PQ1.push(e);
        }

    }

    // Kruskal across the river
    while (counter > N - B) {
        Edge tmp = river_edges_PQ1.top();
        river_edges_PQ1.pop();
        // if i can add it
        unite_all(all_nodes, counter, tmp, true, total_cost);
    }

    // Kruskal to other
    while (true) {
        Edge tmp = other_edges_PQ.top();
        other_edges_PQ.pop();

        unite_all(all_nodes, counter, tmp, false, total_cost);

        int capacity = all_nodes[0].capacity;
        if (capacity == N) break;
    }


    cout << total_cost << endl;
}


