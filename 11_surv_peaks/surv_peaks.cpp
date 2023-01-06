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


vector<unordered_set<int>> condense_graph(vector<vector<int>> adj,
                    int components_size,
                    vector<int> comp_of) {
    // kondenzace komponent:
    // pro kazdou komponentu vytvorim uzel v ADJ_CONDENSED
    // pro kazdej uzel v ADJ
            // 1) v jake je komponente
            // 2) do kterych uzlu vedou jejich hrany (do jakych komponent)
    vector<unordered_set<int>> adj_cond(components_size);
    for (int src = 0; src < adj.size(); ++src) {
        int src_component = comp_of[src];
        for (int d = 0; d < adj[src].size(); ++d) {
            int dest = adj[src][d];
            int dest_component = comp_of[dest];
            if(dest_component != src_component) {
                adj_cond[src_component].insert(dest_component);
            }
        }
    }
    return adj_cond;
}

vector<Path> PATHS;
int LONGEST_PATH = 0;


void rec_DFS_to_CP(int current, vector<unordered_set<int>>& adjacencyList, int CP_comp, Path curr_path) {
    // Process the current node here
    curr_path.visited[current] = true;
    if(current == CP_comp) {
        PATHS.push_back(curr_path);
        LONGEST_PATH = max(LONGEST_PATH, (int)curr_path.sequence.size());
        return;
    }
    curr_path.sequence.push_back(current);


    for (int neighbor : adjacencyList[current]) {
        if (!curr_path.visited[neighbor]) {
            rec_DFS_to_CP(neighbor, adjacencyList, CP_comp, curr_path);
        }
    }
}

vector<int> comp_w_most_parents;
vector<bool> visited_from_CP;
int MOST_PARENTS = 0;


void bfs_from_CP(int start, vector<unordered_set<int>>& adjacencyList, vector<vector<int>>& parents) {
    queue<int> Q;
    vector<bool> visited(adjacencyList.size(), false);

    Q.push(start);
    visited[start] = true;

    while (!Q.empty()) {
        int curr = Q.front(); Q.pop();

        // Process the curr node here

        for (int neighbor : adjacencyList[curr]) {
            if (!visited[neighbor]) {
                Q.push(neighbor);
                visited[neighbor] = true;
                visited_from_CP[neighbor] = true;
                for(auto par_of_curr : parents[curr]) {
                    parents[neighbor].push_back(par_of_curr);
                }
                int prev_max = MOST_PARENTS;
                MOST_PARENTS = max(MOST_PARENTS, (int)parents[neighbor].size());
                if(prev_max == MOST_PARENTS) {
                    comp_w_most_parents.push_back(neighbor);
                } else {
                    comp_w_most_parents.clear();
                    comp_w_most_parents.push_back(neighbor);
                }
            }
        }
    }
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

    vector<unordered_set<int>> adj_cond = condense_graph(adj, (int)components.size(), comp_of);
    // dfs ze vsech do vsech
    // v moment kdy jsem v end nodu
    // eviduju sekvenci a zda obsahuje CP
    int CP_comp = comp_of[C];

    vector<vector<int>> parents(components.size());
    for (int i = 0; i < components.size(); ++i) {
        parents[i].push_back(i);
    }
    vector<bool> vfcp(components.size(), false);
    visited_from_CP = vfcp;
    bfs_from_CP(CP_comp, adj_cond, parents);

    for (int i = 0; i < adj_cond.size(); i++) {
        /// recursive DFS uklada cesty do PATHS
        if(visited_from_CP[i]) continue;

        vector<bool> visited(adj_cond.size(), false);
        Path curr_path{true, {}, visited};

        rec_DFS_to_CP(i, adj_cond, CP_comp, curr_path);
    }


    // najdi nejdelsi pathsy ktere obsahuji CP

    sort(PATHS.begin(), PATHS.end());


    set<int> walked_comps;

    int ptr = 0;
    while (true) {
        // pro kazdou LONGEST cestu z ni vytahneme jeji komponenty
        Path curr = PATHS[ptr];
        if(curr.sequence.size() != LONGEST_PATH) break;
        for (int i = 0; i < curr.sequence.size(); ++i) {
            walked_comps.insert(curr.sequence[i]);
        }
        ptr++;
    }
    for (const auto& pars : parents) {
        if(pars.size() == MOST_PARENTS) {
            for(auto par : pars) {
                walked_comps.insert(par);
            }
        }
    }

    int walked_points = 0;
    for (int wc : walked_comps) {
        walked_points += (int)components[wc].size();
    }

    LONGEST_PATH += MOST_PARENTS;

    cout << LONGEST_PATH << " " << walked_points << endl;

}











