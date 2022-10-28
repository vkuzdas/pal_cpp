//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//


#include <iostream>
#include <vector>
#include <map>
#include "../00_library/library.h"


using namespace std;

void get_fast_servers(unsigned int f, vector<bool> &f2s_bool, vector<unsigned int> &f2s_vec) {
    for (unsigned int i = 0; i < f; ++i) {
        unsigned int edge_label;
        cin >> edge_label;
        f2s_bool[edge_label] = true;
        f2s_vec[i] = edge_label;
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
    unsigned int old_v, old_e;
    cin >> old_v >> old_e;
    vector<vector<unsigned int>> old_g = get_undi_graph_from_input(old_e);

    // get new
    unsigned int new_v, new_e, f;
    cin >> new_v >> new_e >> f;
    vector<bool>fast_s_bool(30); // 1 ≤ F2 ≤ N2, 5 ≤ N2 ≤ 30
    vector<unsigned int>fast_s_verts(f, false);
    get_fast_servers(f, fast_s_bool, fast_s_verts);
    vector<vector<wd_pair>> new_g = get_undi_weighted_graph_from_input(new_v, new_e);

    new_g = remap_new_g(new_g, fast_s_bool, fast_s_verts);

    return 0;
}


