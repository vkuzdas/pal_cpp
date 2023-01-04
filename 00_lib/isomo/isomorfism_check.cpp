//
// Created by kuzdavoj@fel.cvut.cz on 1/3/2023.
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

// (d1 btw two graphs are equivalent)
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

// (possible mappings of G1 -> G2)
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
}