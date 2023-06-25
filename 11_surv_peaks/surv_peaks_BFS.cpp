//
// Created by kuzdavoj@fel.cvut.cz on 1/21/2023.
//


//
// Created by kuzdavoj@fel.cvut.cz on 1/6/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <climits>
#include <bits/stdc++.h>

using namespace std;

struct Path {
    bool contains_CP;
    vector<int> sequence;
    vector<bool> visited;

    bool operator<(const Path &rhs) const {
        return sequence.size() > rhs.sequence.size();
    }

    Path(bool containsCp, const vector<int> &sequence, const vector<bool> &visited) : contains_CP(containsCp),
                                                                                      sequence(sequence),
                                                                                      visited(visited) {}

};


vector<vector<int>> tarjan_scc_adj(vector<vector<int>> &adj, vector<int> &comp_of, int N) { // consider const
    const int UNDEF = INT32_MAX;
    vector<int> index(N, UNDEF);
    vector<int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<int> stck;
    vector<vector<int>> comps;
    int comp_num = 0;
    stack<pair<int, int>> call_stack; // recursion argument stack
    vector<int> scc_of(N, UNDEF);
    int i = 0;

    for (int v = 0; v < N; ++v) {
        if (index[v] != UNDEF) continue;
        call_stack.push({v, 0});
        while (!call_stack.empty()) {
            v = call_stack.top().first;
            int pi = call_stack.top().second;
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
                int prev = adj[v][pi - 1];
                lowlink[v] = min(lowlink[v], lowlink[prev]);
            }
            // recurse on
            while (pi < adj[v].size() && index[adj[v][pi]] != UNDEF) {
                int next = adj[v][pi];
                if (on_stack[next]) {
                    lowlink[v] = min(lowlink[v], index[next]);
                }
                pi++;
            }
            // found vert without index
            if (pi < adj[v].size()) {
                int next = adj[v][pi];
                call_stack.push({v, pi + 1});
                call_stack.push({next, 0});
                continue;
            }
            // vertex is root of SCC
            if (lowlink[v] == index[v]) {
                vector<int> scc;
                while (true) {
                    auto next = stck.top();
                    stck.pop();
                    on_stack[next] = false;
                    scc.push_back(next);
                    comp_of[next] = comp_num;
                    if (next == v) {
                        comp_num++;
                        break;
                    }
                }
                comps.push_back(scc);
            }
        }
    }
    return comps;
}


vector<vector<int>> condense_graph(vector<vector<int>> adj,
                                int components_size,
                                vector<int> comp_of) {
    // kondenzace komponent:
    // pro kazdou komponentu vytvorim uzel v ADJ_CONDENSED
    // pro kazdej uzel v ADJ
    // 1) v jake je komponente
    // 2) do kterych uzlu vedou jejich hrany (do jakych komponent)
    vector<vector<int>> adj_cond(components_size);
    for (int src = 0; src < adj.size(); ++src) {
        int src_component = comp_of[src];
        for (int d = 0; d < adj[src].size(); ++d) {
            int dest = adj[src][d];
            int dest_component = comp_of[dest];
            if(dest_component != src_component) {
                adj_cond[src_component].push_back(dest_component);
            }
        }
    }
    return adj_cond;
}



/**
 * Assigns district to every city based on city's proximity to district city
 *      (or based on which district has lower id in case proximity is same)
 */
pair<vector<int>, vector<set<int>>> BFS_from_CP(vector<vector<int>> &adj, int CP) {

    vector<int> depth(adj.size(),0);
    vector<set<int>> node_predecessors(adj.size()); // pro kazdej node ulozi jeho predecessors
    queue<int> q;

    depth[CP] = 0;
    node_predecessors[CP] = {};
    q.push(CP);

    // kazdemu uzlu priradime hloubku od CP
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        for(auto nei : adj[curr]) {

            if (depth[curr] + 1 > depth[nei]) {
                // preferujeme delsi cestu
                node_predecessors[nei] = node_predecessors[curr];
                node_predecessors[nei].insert(curr);
            }

            else if (depth[curr] + 1 == depth[nei]) {
                // pokud jsou cesty stejne dlouhe, delame sjednoceni
                for(auto curr_predecessor : node_predecessors[curr])
                    node_predecessors[nei].insert(curr_predecessor);
                node_predecessors[nei].insert(curr);
            }

            // pokud je cesta z currentu kratsi, ignorujeme
            else if (depth[curr] + 1 < depth[nei]) {}

            depth[nei] = max(depth[curr] + 1, depth[nei]);
            q.push(nei);
        }
    }
    return {depth, node_predecessors};
    
    /// jsem v currentu, koukam na dalsi
}

int _main() {
    vector<vector<int>> adj = {{1,2,4},{3},{1},{},{1}};
    BFS_from_CP(adj, 0);
    return 0;
}


vector<vector<int>> reverse_edges(vector<vector<int>> adj) {
    vector<vector<int>> reversed;
    for (vector<int> vec : adj) {
        reversed.emplace_back();
    }
    for (int srcVert = 0; srcVert < adj.size(); ++srcVert) {
        for (int i = 0; i < adj[srcVert].size(); ++i) {
            int destVert = adj[srcVert][i];
            reversed[destVert].push_back(srcVert);
        }
    }
    return reversed;
}

int main() {
    int N; // # bodu v parku
    int M; // # tracku v parku
    int C; // # oznaceni peaku

    cin >> N >> M >> C;
    vector<vector<int>> adj(N);

    for (int i = 0; i < M; ++i) {
        int src, dest;
        cin >> src >> dest;
        adj[src].push_back(dest);
    }

    vector<int> comp_of(N, INT32_MAX);
    vector<vector<int>> components = tarjan_scc_adj(adj, comp_of, N);

    vector<vector<int>> adj_cond = condense_graph(adj, (int)components.size(), comp_of);
    int CP_comp = comp_of[C];


    /// nejdelsi cesta z CP pomoci BFS
    // zacni z CP
    pair<vector<int>, vector<set<int>>> depth_preds = BFS_from_CP(adj_cond, CP_comp);
    vector<vector<int>> adj_cond_REV = reverse_edges(adj_cond);
    pair<vector<int>, vector<set<int>>> depth_preds_REV = BFS_from_CP(adj_cond, CP_comp);

    return 0;
}











