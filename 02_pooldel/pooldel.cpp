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
vector<unsigned int> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N);
void delete_wcs(vector<unsigned int> &comp_of,
                vector<vector<unsigned int>> &adj
                );
void get_min_cost(
        vector<vector<unsigned int>> &comps,
        vector<vector<unsigned int>> &adj,
        vector<unsigned int> &var,
        vector<unsigned int> &cost
        );


void get_max_var(vector<unsigned int> &comp_of, vector<unsigned int> &var_indices,  unsigned int &var,  unsigned int N);

// 1) identify SCC's
// 2) detect WC's
// 3) DFS in MAX(var) to get min(cost)
int main() {
    unsigned int N, M;
    cin >> N >> M;

    vector<vector<unsigned int>> adj = getInput(N, M);
    vector<unsigned int> comp_of = tarjan_scc(adj, N);
    delete_wcs(comp_of, adj);

    unsigned int max_var = 0;
    vector<unsigned int> var_indices;
    get_max_var(comp_of, var_indices, max_var, N);


//    vector<unsigned int> cost(comp_of.size(), 0);
//    get_min_cost(comp_of, adj, var_indices, cost);

    cout << "comp_of size: " << comp_of.size() << "\n";
    cout << "var: " << max_var << "\n";
    return 0;
}


/**
 * comp_of obsahuje uzle a jejich komponenty
 * Pokud je uzel weak crossing, ma komponentu rovnou UINT_MAX
 *
 * Chceme zjistit nejvetsi var ze vsech komponent
 */
void get_max_var(vector<unsigned int> &comp_of, vector<unsigned int> &var_indices, unsigned int &var, unsigned int N) {

    /// POCITAS TO SPATNE!!!
    // todo: udelej si pocet u kazde komponenty
    vector<unsigned int> var_of_comps(N, 0); // TODO: asi prilis velke
    for (unsigned int curr = 0; curr < comp_of.size(); ++curr) {
        // pro kazdy uzel, zjisti komponentu, jeji vyskyt eviduj na indexu komponenty ve var_of_comps
        if(comp_of[curr] == UINT_MAX) continue;
        var_of_comps[comp_of[curr]] += 1;
    }

    var = 0; var_indices.clear();
    for (unsigned int i = 0; i < var_of_comps.size(); ++i) {
        if (var_of_comps[i] > var) {
            // novy max -> odstran predchozi, zaznamenej pozici, updatni max
            var_indices.clear();
            var_indices.push_back(i);
            var = var_of_comps[i]; // FIXME
        } else if (var_of_comps[i] == var) {
            // stejny max -> zaznamenej pozici
            var_indices.push_back(i);
        }
    }
    // spocitali jsme vsechny uzle, var je ve skutecnosti pocet uzlu do kterych se dostanes z jednoho uzlu
    var -= 1;
}


//1c1,2
//< 2 94 3165
//---
//> comp_of size: 5
//> var: 118        should be 94!
/// pub07 actin' weird

/**
 * Chceme smazat uzly ktere vedou pryc z komponenty
 * Oznaceni uzlu je rychlejsi nez mazani
 */
void delete_wcs(vector<unsigned int> &comp_of,
                vector<vector<unsigned int>> &adj) {
    vector<unsigned int> comp_indices;
    vector<unsigned int> var_of_comp;
    vector<unsigned int> tbd;

    for (unsigned int src = 0; src < comp_of.size(); ++src) {
        for (unsigned int d = 0; d < adj[src].size(); ++d) {
            auto dst = adj[src][d];
            if(comp_of[dst] != comp_of[src]) {

            }
            if (comp_of[dst] != comp_of[src]) {
                tbd.push_back(src);
            }
        }
    }
    for (auto index : tbd) {
        comp_of[index] = UINT_MAX;
    }
}

// TODO: docs
vector<unsigned int> tarjan_scc(vector<vector<unsigned int>> &adj, unsigned int N) { // consider const
    const unsigned int UNDEF = UINT_MAX;
    vector<unsigned int> index(N, UNDEF); // order of node in which it was visited
    vector<unsigned int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<unsigned int> stck;
    vector<unsigned int> comp_of(N, UINT_MAX);
    unsigned int comp_num = 0;
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
                while (true) {
                    auto next = stck.top();
                    stck.pop();
                    on_stack[next] = false;
                    comp_of[next] = comp_num;
                    if (next == node) {
                        comp_num++;
                        break;
                    }
                }
            }
        }
    }
    return comp_of;
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
