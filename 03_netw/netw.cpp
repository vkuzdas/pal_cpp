//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//


#include <iostream>
#include <vector>

using namespace std;
using wd_pair = pair<unsigned int, unsigned int>;

vector<vector<unsigned int>> get_old_net(unsigned int n, unsigned int m) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    vector<vector<unsigned int>> adj(n);
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int src, dest;
        cin >> src >> dest;
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }
    return adj;
}

vector<vector<wd_pair>> get_new_net(unsigned int vert_count, unsigned int edge_count) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    vector<vector<wd_pair>> adj(vert_count);
    for (unsigned int i = 0; i < edge_count; ++i) {
        unsigned int src, dest, weight;
        cin >> src >> dest >> weight;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
    return adj;
}


void get_fast_servers(unsigned int f, vector<bool> &is_fast, vector<unsigned int> &f_servers) {
    for (unsigned int i = 0; i < f; ++i) {
        unsigned int edge_label;
        cin >> edge_label;
        is_fast[edge_label] = true;
        f_servers[i] = edge_label;
    }
}

// Chceme fast servery co nejvice na levo
// vytvorime mapping kde jako prvni hodime fasty, potom ostatni servery
// new_g premapujeme dle tohoto mappingu
vector<vector<wd_pair>> remap_new_g(vector<vector<wd_pair>> &new_g, vector<bool> &fast_s_bool, vector<unsigned int> &fast_nodes) {
    vector<unsigned int> map(new_g.size());
    for (unsigned int i = 0; i < fast_nodes.size(); ++i) {
        auto fast_node = fast_nodes[i];
        map[fast_node] = i;
    }
    auto itr = (unsigned int)fast_nodes.size();
    for (unsigned int i = 0; i < new_g.size(); ++i) {
        if (fast_s_bool[i]) continue;
        auto slow_node = i;
        map[slow_node] = itr;
        itr++;
    }

    vector<vector<wd_pair>> remapped(new_g.size());
    for (unsigned int i = 0; i < new_g.size(); ++i) {
        for (unsigned int j = 0; j < new_g[i].size(); ++j) {
            unsigned int s, w, d, rem_s, rem_d;
            s = i;
            w = new_g[i][j].first;
            d = new_g[i][j].second;
            rem_s = map[s];
            rem_d = map[d];
            remapped[rem_s].push_back({w, rem_d});
        }
    }

    return remapped;
}

unsigned int bin_coeff(unsigned int S, unsigned int k) {
    double res = 1;
    for (unsigned int i = 1; i <= k; ++i)
        res = res * (S - k + i) / i;
    return (unsigned int)(res + 0.01);
}


//Priblizny postup:
// ✅ Premapujeme vsechny uzly v New tak aby FAST byly na zacatku
//    generujeme k-subsety (protoze jsme premapovali tak prvni subsety jsou s nejvice FAST)
//    pro kazdy subset (bincoef(S,k)-krat) zkontrolujeme Invarianty
//      [shoda degrees, pravidlo sousedu ze zadani, planarita, spojitost, ...]
//    Pokud se v subsetu shoduji invarianty zjisti zda existuje mapping
//    Pokud existuje mapping, zapamatuj si pocet jeho F
//    Dale generuj pouze s timto poctem F
//    nakonec pro kazdy mapping spocti cenu

int main() {
    // get old
    unsigned int n1, m1;
    cin >> n1 >> m1;
    vector<vector<unsigned int>> old_g = get_old_net(n1, m1);

    // get new
    unsigned int n2, m2, f;
    cin >> n2 >> m2 >> f;
    vector<bool>is_fast(30); // 1 ≤ F2 ≤ N2, 5 ≤ N2 ≤ 30
    vector<unsigned int>f_servers(f, false);
    get_fast_servers(f, is_fast, f_servers);
    vector<vector<wd_pair>> new_g = get_new_net(n2, m2);

    new_g = remap_new_g(new_g, is_fast, f_servers);

    unsigned int subsets = bin_coeff((unsigned int)new_g.size(), (unsigned int)old_g.size());
    cout << new_g.size();
    cout << "\n";
    cout << old_g.size();
    cout << "\n";
    cout << subsets;

    return 0;
}


