//
// Created by kuzdavoj@fel.cvut.cz on 10/7/2022.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;

vector<vector<unsigned int>> getInput(unsigned int N, unsigned int M);
vector<vector<unsigned int>> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N);
void delete_wcs(vector<vector<unsigned int>> &comps, vector<vector<unsigned int>> &adj);
void count_var_cost(
        vector<vector<unsigned int>> &comps,
        vector<vector<unsigned int>> &adj,
        vector<unsigned int> &var,
        vector<unsigned int> &cost
        );

int main() {
    // 1) identify SCC's
    // 2) detect WC's
    // 3) DFS in MAX(var) to get min(cost)
    unsigned int N, M;
    cin >> N >> M;

    vector<vector<unsigned int>> adj = getInput(N, M);
    vector<vector<unsigned int>> comps = tarjan_scc(adj, N);
    delete_wcs(comps, adj);
    vector<unsigned int> var(comps.size(),0), cost(comps.size(),0);
    count_var_cost(comps, adj, var, cost);

    cout << comps.size();
    return 0;
}

void count_var_cost(
        vector<vector<unsigned int>> &comps,
        vector<vector<unsigned int>> &adj,
        vector<unsigned int> &var,
        vector<unsigned int> &cost
               ) {
    for (unsigned int i = 0; i < comps.size(); ++i) {
        // var = all nodes that are not UINT_MAX
        vector<unsigned int> scc = comps[i];
        unsigned int min_cost =
        // dfs to all nodes, track only the minimal

    }
}


void delete_wcs(vector<vector<unsigned int>> &comps, vector<vector<unsigned int>> &adj) {
    for (unsigned int i = 0; i < comps.size(); ++i) {
        vector<unsigned int> scc = comps[i];
        vector<unsigned int> index_to_delete;
        for (unsigned int j = 0; j < scc.size(); ++j) {
            unsigned int src = scc[j];
            for (unsigned int k = 0; k < adj[src].size(); ++k) {
                unsigned int dst = adj[src][k];
                if(std::find(scc.begin(), scc.end(), dst) == scc.end()) {
                    index_to_delete.push_back(j);
                }
            }
        }
        for (unsigned int index : index_to_delete) {
            scc[index] = UINT_MAX;
        }
        comps[i] = scc;
    }
}

// TODO: docs
vector<vector<unsigned int>> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N) { // consider const
    const unsigned int UNDEF = UINT_MAX;
    vector<unsigned int> index(N, UNDEF);
    vector<unsigned int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<unsigned int> stck;
    vector<vector<unsigned int>> comps;
    stack<pair<unsigned int, unsigned int>> call_stack; // recursion argument stack
    vector<unsigned int> scc_of(N, UNDEF);
    unsigned int i = 0;

    for (unsigned int v = 0; v < N; ++v) {
        if (index[v] != UNDEF) continue;
        call_stack.push({v, 0});
        while (!call_stack.empty()) {
            v = call_stack.top().first;
            unsigned int pi = call_stack.top().second;
            call_stack.pop();
            // uzel nenavstiven
            if (pi == 0) {
                index[v] = i;
                lowlink[v] = i;
                i++;
                stck.push(v);
                on_stack[v] = true;
            }
            // "recursed" on something
            if (pi > 0) {
                unsigned int prev = adj[v][pi - 1];
                lowlink[v] = min(lowlink[v], lowlink[prev]);
            }
            // recurse on
            while (pi < adj[v].size() && index[adj[v][pi]] != UNDEF) {
                unsigned int next = adj[v][pi];
                if (on_stack[next]) {
                    lowlink[v] = min(lowlink[v], index[next]);
                }
                pi++;
            }
            // found vert without index
            if (pi < adj[v].size()) {
                unsigned int next = adj[v][pi];
                call_stack.push({v, pi + 1});
                call_stack.push({next, 0});
                continue;
            }
            // vertex is root of SCC
            if (lowlink[v] == index[v]) {
                vector<unsigned int> scc;
                while (true) {
                    auto next = stck.top();
                    stck.pop();
                    on_stack[next] = false;
                    scc.push_back(next);
                    if (next == v) {
                        break;
                    }
                }
                comps.push_back(scc);
            }
        }
    }
    return comps;
}



vector<vector<unsigned int>> getInput(unsigned int N, unsigned int M) {
    vector<vector<unsigned int>> adj(N);
    // reads graph from input and represents it as directed graph in adj list
    for (unsigned int i = 0; i < M; ++i) {
        unsigned int src, dest;
        cin >> src >> dest;
        adj[src].push_back(dest);
    }
    return adj;
}
