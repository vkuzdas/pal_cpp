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
void delete_wcs(vector<vector<unsigned int>> &comps,
                vector<vector<unsigned int>> &adj,
                vector<unsigned int> &var_indices,
                unsigned int &max_var
                );
void get_min_cost(
        vector<vector<unsigned int>> &comps,
        vector<vector<unsigned int>> &adj,
        vector<unsigned int> &var,
        vector<unsigned int> &cost
        );


// 1) identify SCC's
// 2) detect WC's
// 3) DFS in MAX(var) to get min(cost)
int main() {
    unsigned int N, M;
    cin >> N >> M;

    vector<vector<unsigned int>> adj = getInput(N, M);
    vector<vector<unsigned int>> comps = tarjan_scc(adj, N);

    unsigned int max_var = 0;
    vector<unsigned int> var_indices;
    delete_wcs(comps, adj, var_indices, max_var);


    vector<unsigned int> cost(comps.size(),0);
    get_min_cost(comps, adj, var_indices, cost);

    cout << "comps size: " << comps.size() << "\n";
    cout << "var: " << max_var << "\n";
    return 0;
}


//1c1,2
//< 2 94 3165
//---
//> comps size: 5
//> var: 118        should be 94!
/// pub07 actin' weird

/**
 * There are nodes that have an edge going out from SCC these edges are "weak crossings".
 * We mark them by replacing their node ID with UINT_MAX.
 *
 * For efficiency purposes we also find the maximum size of SCC without weak crossings (@param max_var).
 * Moreover, the position of these SCCs is marked within @param var_indices.
 */
void delete_wcs(vector<vector<unsigned int>> &comps,
                vector<vector<unsigned int>> &adj,
                vector<unsigned int> &var_indices,
                unsigned int &max_var) {
    vector<unsigned int> comp_indices;

    for (unsigned int c = 0; c < comps.size(); ++c) { /// for a COMP
        vector<unsigned int> SCC = comps[c];
        vector<unsigned int> weak_indices;
        for (unsigned int n = 0; n < SCC.size(); ++n) { /// for a NODE within comp
            unsigned int src = SCC[n];
            for (unsigned int d = 0; d < adj[src].size(); ++d) { /// for node's DEST
                unsigned int dst = adj[src][d];
                if(std::find(SCC.begin(), SCC.end(), dst) == SCC.end()) { // FIXME: might be slow
                    // destination of crossing is not within SCC -> should be deleted
                    printf("Del %d from comp[%d] (%d -> %d)\n", src, c, src, dst);
                    weak_indices.push_back(n);
                    break;
                }
            }
        }
        for (unsigned int index : weak_indices) {
            // weak crossing "deletion" (marking is faster)
            SCC[index] = UINT_MAX;
        }
        comps[c] = SCC;
        auto comp_size = (unsigned int)(SCC.size() - weak_indices.size());
        if (comp_size > max_var) {
            // found new max -> delete indices of lower var, push index of curr comp
            max_var = comp_size;
            var_indices.clear();
            var_indices.push_back(c);
        } else if (comp_size == max_var) {
            // var is still the same, mark its index
            var_indices.push_back(c);
        }
    }
    if (max_var != 0) max_var = max_var - 1; /// var is the num of nodes that each node can get to in a SCC
}

// TODO: docs
vector<vector<unsigned int>> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N) { // consider const
    const unsigned int UNDEF = UINT_MAX;
    vector<unsigned int> index(N, UNDEF); // order of node in which it was visited
    vector<unsigned int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<unsigned int> stck;
    vector<vector<unsigned int>> comps;
    stack<pair<unsigned int, unsigned int>> call_stack; // recursion argument stack
    unsigned int i = 0;

    for (unsigned int node = 0; node < N; ++node) {
        if (index[node] != UNDEF) continue;
        call_stack.push({node, 0});
        while (!call_stack.empty()) {
            node = call_stack.top().first;
            unsigned int ptr = call_stack.top().second;
            call_stack.pop();
            // uzel nenavstiven
            if (ptr == 0) {
                index[node] = i;
                lowlink[node] = i;
                i++;
                stck.push(node);
                on_stack[node] = true;
            }
            // "recursed" on something
            if (ptr > 0) {
                unsigned int prev = adj[node][ptr - 1];
                lowlink[node] = min(lowlink[node], lowlink[prev]);
            }
            // recurse on
            while (ptr < adj[node].size() && index[adj[node][ptr]] != UNDEF) {
                unsigned int next = adj[node][ptr];
                if (on_stack[next]) {
                    lowlink[node] = min(lowlink[node], index[next]);
                }
                ptr++;
            }
            // found vert without index
            if (ptr < adj[node].size()) {
                unsigned int next = adj[node][ptr];
                call_stack.push({node, ptr + 1});
                call_stack.push({next, 0});
                continue;
            }
            // vertex is root of SCC
            if (lowlink[node] == index[node]) {
                vector<unsigned int> scc;
                while (true) {
                    auto next = stck.top();
                    stck.pop();
                    on_stack[next] = false;
                    scc.push_back(next);
                    if (next == node) {
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
