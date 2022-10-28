//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "library.h"

using namespace std;
using wd_pair = pair<unsigned int, unsigned int>;

vector<vector<unsigned int>> get_undi_graph_from_input(unsigned int edge_count) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    vector<vector<unsigned int>> adj(edge_count);
    for (unsigned int i = 0; i < edge_count; ++i) {
        unsigned int src, dest;
        cin >> src >> dest;
        adj[src].push_back({dest});
        adj[dest].push_back({src});
    }
    return adj;
}
vector<vector<wd_pair>> get_undi_weighted_graph_from_input(unsigned int vert_count, unsigned int edge_count) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    vector<vector<wd_pair>> adj(vert_count);
    for (unsigned int i = 0; i < edge_count; ++i) {
        unsigned int src, dest, weight;
        cin >> src >> dest >> weight;
        cout << src;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
    return adj;
}


/**
 * Jarnik/Prim algorithm with Min-Heap running in O(m log n)
 */
unsigned int prim_mst(vector<vector<wd_pair>> &adj, unsigned int startV) {
    unsigned int total_cost = 0;
    unsigned int counter = 0;
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
 * The source code closely follows the one on wikipedia except it implements its own Stack mem allocation on heap
 * https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
index := 0
S := empty stack
for each v in V do
    if v.index is undefined then
        strongconnect(v)
function strongconnect(v)
    v.index := index
    v.lowlink := index
    index := index + 1
    S.push(v)
    v.onStack := true
    for each (v, w) in E do
        if w.index is undefined then
            strongconnect(w)
            v.lowlink := min(v.lowlink, w.lowlink)
        else if w.onStack then
            v.lowlink := min(v.lowlink, w.index)
    if v.lowlink = v.index then
        start a new SCC
        repeat
            w := S.pop()
            w.onStack := false
            add w to current SCC
        while w ≠ v
        output the current SCC
 */
vector<unsigned int> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N) {
    unsigned int CURR_UNVISITED = 0;
    const unsigned int UNDEF = UINT_MAX;
    vector<unsigned int> index_arr(N, UNDEF); // order of node in which it was visited
    vector<unsigned int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<unsigned int> S;
    vector<unsigned int> comp_of(N, UINT_MAX);
    unsigned int comp_num = 0;
    stack<pair<unsigned int, unsigned int>> CALL_S; // recursion argument stack
    unsigned int index = 0;

    /// for each v in V
    for (unsigned int v = 0; v < N; ++v) {
        if (index_arr[v] != UNDEF) continue;
        CALL_S.push({v, CURR_UNVISITED});

        while (!CALL_S.empty()) {
            v = CALL_S.top().first;
            auto act = CALL_S.top().second;
            CALL_S.pop();

            // Pokud v vidíme poprvé
            if (act == CURR_UNVISITED) {
                // Set the depth index for v to the smallest unused index
                index_arr[v] = index;
                lowlink[v] = index;
                index++;
                S.push(v);
                on_stack[v] = true;
            }

            /// Consider successors of v
            unsigned int w;

            // Pokud jsme zrovna na něco vyrekurzovali (uzel ktery jsme si nechali na "potom"
            // Vracime se v DFS zpet a updatujeme lowlinky predchozim uzlum
            if (act > 0) {
                // Successor w has not yet been visited; recurse on it
                // strongconnect(w)    <- point of recursion  call
                w = adj[v][act - 1];
                lowlink[v] = min(lowlink[v], lowlink[w]);
            }

            // Najdeme další věc na kterou rekurzovat
            // V DFS stromu se vracime jeste dale, zde nas zajima zda i INDEX predchoziho neni nizsi nez LL curr uzlu
            while (act < adj[v].size() && index_arr[adj[v][act]] != UNDEF) {
                w = adj[v][act];
                if (on_stack[w]) {
                    // Successor w is in stack S and hence in the current SCC
                    // If w is not on stack, then (v, w) is an edge pointing to an SCC already found and must be ignored
                    lowlink[v] = min(lowlink[v], index_arr[w]);
                }
                act++;
            }

            // Pokud jsme našli něco co nemá index
            if (act < adj[v].size()) {
                w = adj[v][act];
                CALL_S.push({v, act + 1});          // Uzel nechavame na "potom" v call stacku
                CALL_S.push({w, CURR_UNVISITED});   // CALL
                continue;
            }

            // If v is a root node, pop the stack and generate an SCC
            if (lowlink[v] == index_arr[v]) {
                while (true) {
                    w = S.top();
                    S.pop();
                    on_stack[w] = false;
                    comp_of[w] = comp_num;
                    if (w == v) {
                        comp_num++;
                        break;
                    }
                }
            }
        }
    }
    return comp_of;
}