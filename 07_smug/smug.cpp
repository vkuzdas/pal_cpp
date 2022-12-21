//
// Created by kuzdavoj@fel.cvut.cz on 12/20/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <sstream>
#include <map>


using namespace std;


vector<vector<int>> read_adj(int N, int M) {
    vector<int> m;
    vector<vector<int>> adj(N,m);
    for (int i = 0; i < M; ++i) {
        int src, dst;
        cin >> src >> dst;
        /// neorientovany graf
        adj[src].push_back(dst);
        adj[dst].push_back(src);
    }
    return adj;
}


bool next_K_subset(vector<int>& a, int n) {
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

// udelej jedno BFS z nejakeho uzlu v subset
// do souseda vejdi jen pokud je v subset
bool is_subset_connected(vector<bool>& in_subset, vector<int> &subset, vector<vector<int>> &adj) {

    queue<int> Q;
    vector<bool> visited(in_subset.size(), false);
    Q.push(subset[0]);

    while (!Q.empty()) {
        int curr = Q.front(); Q.pop();
        visited[curr] = true;

        for(auto neigh : adj[curr]) {
            /// soused v subsetu a nenavstiveny -> do fronty
            if(in_subset[neigh] && !visited[neigh]) {
                Q.push(neigh);
            }
        }
    }

    /// pro kazdej nod v subsetu se mrkni jestli je visited
    for(auto node : subset) {
        if (!visited[node])
            return false;
    }
    return true;
}

void print_choice(vector<int> pick) {
    for (auto i: pick) {
        cout << " " << i;
    }
    cout << endl;
}

vector<int> adjust_choice(vector<int>& pick) {
    vector<int> p2;
    for (auto i: pick)
        p2.push_back(i-1);
    return p2;
}

bool edges_equal_C(vector<bool> &in_subset, vector<int> &subset, vector<vector<int>> &adj, int C) {
    // zkontroluj pocet hran
    set<pair<int, int>> edges;
    for(auto node : subset) {
        for(auto neig : adj[node]) {
            if(!in_subset[neig]) continue;
            pair<int, int> p = {min(node, neig), max(neig, node)};
            edges.insert(p);
            if(edges.size() > C) {
                return false;
            }
        }
    }
    return edges.size() == C;
}

bool subset_overlaps(vector<bool>& in_first_subset, vector<int>& second_subset) {
    for (auto i : second_subset) {
        if(in_first_subset[i])
            return true;
    }
    return false;
}


bool sofar_valid(int P, vector<bool>& in_first_subset, vector<vector<int>>& adj, int C, int N, vector<int>& second_subset, vector<bool>& in_second_subset) {
    if(!is_subset_connected(in_second_subset, second_subset, adj)) // over zda je choice spojeny
        return false;
    if(!edges_equal_C(in_second_subset, second_subset, adj, C)) // zkontroluj pocet hran
        return false;
    return true;
}


map<int, int> get_d1(vector<int>& curr_subset, vector<bool>& in_subset, vector<vector<int>>& adj) {
    vector<int> new_degrees;
    map<int,int> node_degree;
    for (int i = 0; i < curr_subset.size(); ++i) {
        int curr_node = curr_subset[i];
        auto neighbors = adj[curr_node];
        int degree = 0;
        for (int j = 0; j < neighbors.size(); ++j) {
            int neigh = neighbors[j];
            if (in_subset[neigh]) { // pocitej pouze uzly ktere jsou v subsetu
                degree++;
            }
        }
        node_degree[curr_node] = degree;
    }
    return node_degree;
}


map<int, vector<int>> get_d2(vector<vector<int>> &adj, vector<int> &curr_subset, vector<bool> &in_subset) {
    map<int, vector<int>> node_d2;
    for (int i = 0; i < curr_subset.size(); ++i) {
        vector<int> d2(curr_subset.size(), 0);
        int curr_node = curr_subset[i];
        auto neighbors = adj[curr_node];
        for (int j = 0; j < neighbors.size(); ++j) { // projed vsechny neigh ktere jsou v setu
            int neighbor = neighbors[j];
            if(!in_subset[neighbor]) continue;
            // od neigh degree se musi odecist ty ktere tam nejsou
            int nei_degree = 0;
            for (int k = 0; k < adj[neighbor].size(); ++k) { // koukni na neighbors' neighbors, jestli jsou v setu, inkrementuj degree
                if(in_subset[adj[neighbor][k]]) {
                    nei_degree++;
                }
            }
            d2[nei_degree]++;

        }
        node_d2[curr_node] = d2;
    }
    return node_d2;
}


bool d1_OK(map<int, int> first_d1, map<int, int> second_d1) {
    vector<int> first_d1_sorted;
    for (auto p : first_d1) first_d1_sorted.push_back(p.second);
    vector<int> second_d1_sorted;
    for (auto p : second_d1) second_d1_sorted.push_back(p.second);

    sort(first_d1_sorted.begin(), first_d1_sorted.end());
    sort(second_d1_sorted.begin(), second_d1_sorted.end());
    for (int i = 0; i < first_d1_sorted.size(); ++i) {
        if (first_d1_sorted[i] != second_d1_sorted[i]) {
            return false;
        }
    }
    return true;
}

vector<pair<int, int>> mapping_from_d2(
        map<int, vector<int>>& first_d2,
        map<int, vector<int>>& second_d2,
        vector<int>& first_subset,
        vector<int>& second_subset,
        vector<bool>& in_first_subset,
        vector<bool>& in_second_subset
        ) {

    vector<pair<int, int>> mapping;
    vector<bool> mapped_nodes_first(first_subset.size(), false);

    int i = 0;
    for (pair<int, vector<int>> op : first_d2) { // pro vsechny vektory D2 ve starem
        int curr_first_node = op.first;
        vector<int> first_nei_degs = op.second;
        // najdi tento vektor uvnitr d2_new
        // pokud tam neni, vrat prazdnej mapping


        for (auto const& np : second_d2) { // pro vsechny vektory D2 v novem
            int curr_second_node = np.first;
            if(mapped_nodes_first[i]) break;
            if(!in_second_subset[curr_second_node]) continue;
            vector<int> second_nei_degs = np.second;
            bool vectors_equal = true;

            for (int nei_degree = 0; nei_degree < first_nei_degs.size(); ++nei_degree) { // prochazime oba vektory najednou
                int old_deg = first_nei_degs[nei_degree];
                int new_deg = second_nei_degs[nei_degree];
                if(old_deg != new_deg) { // zaznamy se nerovnaji-> mame spatnej match D2_old a D2_new
                    vectors_equal = false;
                    break;
                }
            }
            if (vectors_equal) {
                mapped_nodes_first[i] = true;
                mapping.emplace_back(curr_first_node,curr_second_node);
            }
        }
        i++;
    }

    return mapping;
}


void print_res(vector<int>& first_subset, vector<int>& second_subset) {
    for (int i : first_subset)
        cout << i << " ";
    for (int i : second_subset) {
        cout << i << " ";
    }
}



bool are_isomorphic(
        vector<vector<int>> adj,
        vector<int> first_subset,
        vector<bool> in_first_subset,
        vector<int> second_subset,
        vector<bool> in_second_subset
        ) {

    /// nyni pro oba grafy spocteme sousedy
    auto first_d1 = get_d1(first_subset, in_first_subset, adj);
    auto second_d1 = get_d1(second_subset, in_second_subset, adj);
    if(!d1_OK(first_d1, second_d1))
        return false;


    /// spocteme sousedy sousedu
    vector<bool> mapped_nodes_first(first_subset.size(), false);
    auto first_d2 = get_d2(adj, first_subset, in_first_subset);
    auto second_d2 = get_d2(adj, second_subset, in_second_subset);

    vector<pair<int,int>> mapping = mapping_from_d2(
            first_d2, second_d2,
            first_subset, second_subset,
            in_first_subset, in_second_subset
    );

    /// MAPPING skryva mozna reseni
    if (mapping.size() != first_subset.size())
        return false;
    return true;
};

int main() {
    int N,M,P,C; // nody, hrany, smugs, spoje
    cin >> N >> M >> P >> C;
    vector<vector<int>> adj = read_adj(N, M);

    vector<int> helper1;
    for (int i = 1; i < P+1; ++i) {helper1.push_back(i);}

    set<pair<vector<int>, vector<int>>> already_generated;
    // vyber P osob z N osob
    do {
        vector<int> first_subset = adjust_choice(helper1); // neumim generovat s nulou :( musim odecist jednicku
        vector<bool> in_first_subset(adj.size());
        for(int i : first_subset)
            in_first_subset[i] = true;

        if(!is_subset_connected(in_first_subset, first_subset, adj) ||
            !edges_equal_C(in_first_subset, first_subset, adj, C)) {
            continue;
        }


        vector<int> helper2;
        for (int i = 1; i < P+1; ++i) {helper2.push_back(i);}
        do {
            vector<int> second_subset = adjust_choice(helper2);
            vector<bool> in_second_subset(adj.size());
            for(int i : second_subset) { in_second_subset[i] = true; }

            if(subset_overlaps(in_first_subset, second_subset) ||
                !is_subset_connected(in_second_subset, second_subset, adj) ||
                !edges_equal_C(in_second_subset, second_subset, adj, C))
                continue;

            /// sem se dostanu pouze tehdy, kdyz first_subset a second_subset
            /// 1) neprekryvaji se
            /// 2) maji M=C
            /// 3) jsou spojene

            bool solution_found = are_isomorphic(adj,first_subset, in_first_subset,second_subset, in_second_subset);
            if(!solution_found)
                continue;

            auto cpy = already_generated;
            cpy.insert({second_subset, first_subset});
            if(cpy.size() == already_generated.size()) {
                /// reseni uz jsme nasli
                continue;
            }
            already_generated.insert({first_subset, second_subset});
            print_res(first_subset, second_subset);
            cout << endl;
        } while (next_K_subset(helper2, N));
    } while (next_K_subset(helper1, N));
    return 0;
}


