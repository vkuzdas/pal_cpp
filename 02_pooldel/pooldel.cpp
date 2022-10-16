//
// Created by kuzdavoj@fel.cvut.cz on 10/7/2022.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <climits>

using namespace std;


struct Triple {
    unsigned int node;
    unsigned int var;
    unsigned int cost;
};



/**
 * BFS inside SCC's
 * Get {node, var, cost} combinations
 * Per SCC: do N*BFS(node), fill destination matrix, count cost and var to each other node
 *
 * @param comps list of components  {c1 -> {v1,v2..}}, deleted nodes have id UINT_MAX
 */
vector<vector<unsigned int>> get_dist_matrix(vector<vector<unsigned int>> &comps, vector<vector<unsigned int>> &adj, vector<unsigned int> &comp_of) {
    vector<unsigned int> row(adj.size(), 0); // infinite distance
    vector<vector<unsigned int>> dist_matrix(adj.size(), row); // TODO: arr of pair {dist,cost}

    for (unsigned int scc = 0; scc < comps.size(); ++scc) { // per SCC

        for (auto startV : comps[scc]) {
            vector<bool> seen(adj.size(), false);
            queue<unsigned int> q;
            if(startV == UINT_MAX) continue;
            q.push(startV);

            while (!q.empty()) {
                auto curr = q.front(); q.pop();
                for (unsigned int n = 0; n < adj[curr].size(); ++n) {
                    auto nei = adj[curr][n];
                    if (nei == UINT_MAX) continue;      // chceme neoznacene uzly
                    if (comp_of[nei] != scc) continue;  // uvnitr SCC
                    if (seen[nei]) continue;            // ktere jsme jeste nevideli
                    if (dist_matrix[startV][nei] != 0) continue; // dist jsme uz pridali    TODO: mozna spatne, chci tam dat ten nejnizsi -> ten co tam prijde prvni
                    auto dist_to_curr = dist_matrix[startV][curr];
                    dist_matrix[startV][nei] = dist_to_curr + 1;
                    q.push(nei);
                    seen[curr] = true;
                }
            }
        }
    }
    return dist_matrix;
}

/**
 * Chceme smazat uzly ktere vedou pryc z komponenty
 * Oznaceni uzlu je rychlejsi nez mazani
 */
void delete_wcs(vector<unsigned int> &comp_of, vector<vector<unsigned int>> &adj) {
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

void delete_wcs_adj(vector<vector<unsigned int>> &comps, vector<unsigned int> &comp_of, vector<vector<unsigned int>> &adj) {
    // delete weak streets
    for (unsigned int c = 0; c < comps.size(); ++c) { // for comp
        vector<unsigned int> scc = comps[c];
        vector<unsigned int> index_to_delete;
        for (unsigned int s = 0; s < scc.size(); ++s) { // for src in comp
            unsigned int src = scc[s];
            for (unsigned int d = 0; d < adj[src].size(); ++d) { // if src destination
                unsigned int dst = adj[src][d];
                if(std::find(scc.begin(), scc.end(), dst) == scc.end()) { // not in component TODO slow
                    index_to_delete.push_back(s);
                    comp_of[src] = UINT_MAX;
                }
            }
        }
        for (unsigned int index : index_to_delete) {
            scc[index] = UINT_MAX;
        }
        comps[c] = scc;
    }
}

vector<vector<unsigned int>> tarjan_scc_adj(vector<vector<unsigned int>> &adj, vector<unsigned int> &comp_of, unsigned int N) { // consider const
    const unsigned int UNDEF = UINT_MAX;
    vector<unsigned int> index(N, UNDEF);
    vector<unsigned int> lowlink(N, UNDEF);
    vector<bool> on_stack(N, false);
    stack<unsigned int> stck;
    vector<vector<unsigned int>> comps;
    unsigned int comp_num = 0;
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

vector<Triple> get_nvc(vector<vector<unsigned int>> &dist_matrix, vector<vector<unsigned int>> &comps) {
    // pro kazdy uzel v scc spocitej var a cost z dist matrix
    vector<Triple> nvc;
    for (unsigned int scc_i = 0; scc_i < comps.size(); ++scc_i) {
        auto SCC = comps[scc_i];
        for (unsigned int s = 0; s < SCC.size(); ++s) {
            if (SCC[s]==UINT_MAX) continue;
            auto src = SCC[s];
            unsigned int cost_sum = 0;
            unsigned int var = 0;
            for (unsigned int d = 0; d < SCC.size(); ++d) {
                if (SCC[d] == UINT_MAX) continue;
                if (s==d) continue;
                auto dst = SCC[d];
                auto loaded_cost = dist_matrix[src][dst];
                auto unloaded_cost = dist_matrix[dst][src];
                if (loaded_cost == 0 || unloaded_cost == 0) continue;
                cost_sum += 2*loaded_cost + unloaded_cost;
                var++;
            }
            nvc.push_back({src, var, cost_sum});
        }
    }
    return nvc;
}

void print_result(vector<Triple> &nvc) {
    unsigned int max_var = 0;
    for (unsigned int i = 0; i < nvc.size(); ++i) {
        // find max from all
        if (nvc[i].var > max_var) max_var = nvc[i].var;
    }
    unsigned int min_cost = UINT_MAX;
    unsigned int count = 0;
    for (unsigned int i = 0; i < nvc.size(); ++i) {
        // find min, check only those that have max_var
        auto n = nvc[i].node;
        auto v = nvc[i].var;
        auto c = nvc[i].cost;
        if (v != max_var) continue;
        if(c < min_cost) {
            // found new min
            min_cost = c;
            count = 1;
        }
        else if (nvc[i].cost == min_cost) {
            count ++;
        }
    }
    printf("%d %d %d", count, max_var, min_cost);
}

int main() {
    unsigned int N, M;
    cin >> N >> M;

    vector<vector<unsigned int>> adj = getInput(N, M);
    vector<unsigned int> comp_of(N, UINT_MAX);
    vector<vector<unsigned int>> comps = tarjan_scc_adj(adj, comp_of, N);
    delete_wcs_adj(comps, comp_of, adj);

    vector<vector<unsigned int>> dist_matrix = get_dist_matrix(comps, adj, comp_of);
    vector<Triple> nvc = get_nvc(dist_matrix, comps);

    print_result(nvc);
//    cout << "comp_of size: " << comps.size() << "\n";
//    cout << "var: " << max_var << "\n";
    return 0;
}

