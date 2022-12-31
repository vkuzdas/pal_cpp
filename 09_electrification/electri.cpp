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
    int val;
    int mark;
    vector<int> neib;
};


int find(vector<Node> &UF, int vert) {
    if (UF[vert].parrent == vert) {
        return vert;
    } else {
        return UF[vert].parrent = find(UF, UF[vert].parrent);
    }

}


void unite(vector<Node> &UF, int &counter, int v1, int v2, int eVal, bool force) {
    // not same commponent
    int t1 = find(UF, v1);
    int t2 = find(UF, v2);
    if (t1 != t2) {
        if (t1 < t2) {
            UF[t2].parrent = t1;
            UF[t1].capacity += UF[t2].capacity;
            UF[t1].val = UF[t1].val + eVal + UF[t2].val;
        } else {
            UF[t1].parrent = t2;
            UF[t2].capacity += UF[t1].capacity;
            UF[t2].val = UF[t2].val + eVal + UF[t1].val;
        }
        --counter;
    }
    if (t1 == t2 && force) {
        UF[t1].val += eVal;
        --counter;
    }
}


void dfs(vector<Node> &UF, int v, int mark) {
    UF[v].mark = mark;
    for (auto e: UF[v].neib) {
        if (UF[e].mark == -1) dfs(UF, e, mark);
    }
}


int find_b(vector<Node> &UFB, int vert) {
    if (UFB[vert].parrent == vert) {
        return vert;
    } else {
        return UFB[vert].parrent = find_b(UFB, UFB[vert].parrent);
    }
}


bool unite_B(vector<Node> &UFB, int v1, int v2, int eVal) {
    // not same commponent
    int t1 = find_b(UFB, v1);
    int t2 = find_b(UFB, v2);
    if (t1 != t2) {
        if (t1 < t2) {
            UFB[t2].parrent = t1;
            UFB[t1].capacity += UFB[t2].capacity;
            UFB[t1].val = UFB[t1].val + eVal + UFB[t2].val;
        } else {
            UFB[t1].parrent = t2;
            UFB[t2].capacity += UFB[t1].capacity;
            UFB[t2].val = UFB[t2].val + eVal + UFB[t1].val;
        }
        return true;
    }
    return false;
}




int counter;
int comp = 0;

int main() {
    priority_queue<Edge, vector<Edge>, greater<>> river_edges_PQ0;
    priority_queue<Edge, vector<Edge>, greater<>> river_edges_PQ1;
    priority_queue<Edge, vector<Edge>, greater<>> other_edges_PQ;

    int M, N, D, B;
    cin >> M >> N >> D >> B;
    counter = N;


    Node tmp;
    tmp.neib = vector<int>();
    tmp.val = 0;
    tmp.mark = -1;
    tmp.capacity = 1;
    vector<Node> nodes = vector<Node>(N, tmp);

    for (int i = 0; i < M; i++) {
        int src, dest, cost;
        cin >> src >> dest >> cost;

        Edge tmp;
        tmp.src = src;
        tmp.dest = dest;
        tmp.cost = cost;

        // init Union find struct
        nodes[src].parrent = src;
        nodes[dest].parrent = dest;

        // check if is the river edge
        if (src <= D && dest > D || src > D && dest <= D) {
            // add to river queue
            river_edges_PQ0.push(tmp);

        } else {
            // add to other queue
            other_edges_PQ.push(tmp);
            nodes[src].neib.push_back(dest);
            nodes[dest].neib.push_back(src);
        }
    }

    vector<Node> UFB = vector<Node>();
    // find Components
    for (int i = 0; i < N; i++) {
        if (nodes[i].mark == -1) {

            dfs(nodes, i, comp);
            Node ut;

            ut.parrent = comp;
            ut.capacity = 1;
            UFB.push_back(ut);
            comp += 1;

        }
    }


    while (!river_edges_PQ0.empty()) {
        Edge tmp = river_edges_PQ0.top();
        river_edges_PQ0.pop();

        // if i can add it
        if (unite_B(UFB, nodes[tmp.src].mark, nodes[tmp.dest].mark, tmp.cost)) {

            // if i can add it
            unite(nodes, counter, tmp.src, tmp.dest, tmp.cost, true);
        } else {
            river_edges_PQ1.push(tmp);
        }
    }

    // Kruscal's alg cross the river
    while (counter > N - B) {
        Edge tmp = river_edges_PQ1.top();
        river_edges_PQ1.pop();

        // if i can add it
        unite(nodes, counter, tmp.src, tmp.dest, tmp.cost, true);
    }

    // Kruscal's alg to other
    while (nodes[find(nodes, 0)].capacity != N) {
        Edge tmp = other_edges_PQ.top();
        other_edges_PQ.pop();

        // if i can add it
        unite(nodes, counter, tmp.src, tmp.dest, tmp.cost, false);
    }

    cout << nodes[find(nodes, 0)].val << endl;
}


