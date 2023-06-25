//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <map>

using namespace std;
using uint = unsigned int;
using wd_pair = pair<uint, uint>;


class Subseter {
public:
    Subseter()=default;
    vector<vector<uint>> subsets;


    void k_subsets3a(const vector<uint>& set, vector<uint>& res,
                     const uint start, const uint end,
                     const uint depth, const uint& k) {
        if(depth >= k) {
            subsets.push_back(res);
            return;
        }
        for (uint i = start; i <= end && end - i + 1 >= k - depth; i++) {
            res[depth] = set[i];
            k_subsets3a(set, res, i + 1, end, depth + 1, k);
        }
    }

    void subset(const vector<uint>& set, const uint& k){
        subsets.clear();
        vector<uint> results(k);
        k_subsets3a(set, results, 0, (uint)set.size() - 1, 0, k);
    }

};


// input
vector<vector<uint>> get_old_net(uint n, uint m) {
    // reads graph from input and represents it as mirrored directed graph in adj list
    vector<vector<uint>> adj(n);
    for (uint i = 0; i < m; ++i) {
        uint src, dest;
        cin >> src >> dest;
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }
    return adj;
}

// input
vector<vector<wd_pair>> get_new_net(uint vert_count, uint edge_count) {
    // vezme graf z inputu
    // k tomu zaznamena vytvori seznam pomalych serveru
    vector<vector<wd_pair>> adj(vert_count);
    for (uint i = 0; i < edge_count; ++i) {
        uint src, dest, weight;
        cin >> src >> dest >> weight;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
    return adj;
}

// input
vector<uint> get_all_f(uint f, vector<bool> &is_f) {
    // z inputu vezme rychle servery
    // k tomu vyplni seznam vsech uzlu zda je nebo neni konkretni uzel rychly
    vector<uint> all_f(f);
    for (uint i = 0; i < f; ++i) {
        uint node;
        cin >> node;
        is_f[node] = true;
        all_f[i] = node;
    }
    return all_f;
}


uint bin_coeff(uint n, uint k) {
    double res = 1;
    for (uint i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return (uint)(res + 0.01);
}

// fce  rank->subset
vector<uint> unrank_subset(uint rank, uint n, uint k) {
    if (k == 1) {
        return {rank+1};
    }
//    uint block_sizes = 0;
    uint n1 = n-1;
    uint subset0 = 1;
    while (true) {
        uint bsize = bin_coeff(n1, k-1);
        if(bsize <= rank) {
            rank -= bsize;
            subset0 += 1;
            n1 -= 1;
        } else break;
    }
    vector<uint> subset_rec = unrank_subset(rank, n1, k-1);
    for (uint i = 0; i < subset_rec.size(); ++i) {
        subset_rec[i] += subset0;
    }
    subset_rec.insert(subset_rec.begin(), subset0);
    for (uint i = 0; i < subset_rec.size(); ++i) {
    }
    return subset_rec;
}

// node-id: stupen[D1 je stupen uzlu]
map<uint,uint> get_old_d1(vector<vector<uint>> &old_g) {
    map<uint,uint> node_degree;
    vector<uint> old_degrees;
    for (uint i = 0; i < old_g.size(); ++i) {
        uint src = i;
        uint degree = (uint)old_g[i].size();
        node_degree[src] = degree;
    }
    return node_degree;
}

// node-id: stupen [D1 je stupen uzlu]
map<uint,uint> get_new_d1(vector<vector<wd_pair>> &new_g, vector<bool> &in_subset, vector<uint> &curr_subset) {
    vector<uint> new_degrees;
    map<uint,uint> node_degree;
    for (uint i = 0; i < curr_subset.size(); ++i) {
        uint curr_node = curr_subset[i];
        vector<wd_pair> neighbors = new_g[curr_node];
        uint degree = 0;
        for (uint j = 0; j < neighbors.size(); ++j) {
            uint neigh = neighbors[j].second;
            if (in_subset[neigh]) { // pocitej pouze uzly ktere jsou v subsetu
                degree++;
            }
        }
        node_degree[curr_node] = degree;
    }
    return node_degree;
}

// node-id: vector stupnu
// vector stupnu:  pozice = stupen, hodnota = pocet uzlu s takovym stupnem
map<uint, vector<uint>> get_old_d2(vector<vector<uint>> &old_g) {
    map<uint, vector<uint>> node_d2;
    for (uint i = 0; i < old_g.size(); ++i) { // pro node
        vector<uint> d2(old_g.size(), 0); // 10 nodu, zadne degrees
        vector<uint> neighbors = old_g[i];
        for (uint j = 0; j < neighbors.size(); ++j) { // pro jeho sousedy
            uint neighbor = neighbors[j];
            auto neighbor_degree = old_g[neighbor].size(); // neighbor ma treba d1=3 -> zvedneme pocet 3-deg uzlu
            d2[neighbor_degree]++;
        }
        node_d2[i] = d2;
    }
    return node_d2;
}

// node-id: vector stupnu
// vector stupnu:  pozice = stupen, hodnota = pocet uzlu s takovym stupnem
map<uint, vector<uint>> get_new_d2(vector<vector<wd_pair>> &new_g, vector<uint> &curr_subset, vector<bool> &in_subset) {
    map<uint, vector<uint>> node_d2;
    for (uint i = 0; i < curr_subset.size(); ++i) {
        vector<uint> d2(10, 0);
        uint curr_node = curr_subset[i];
        auto neighbors = new_g[curr_node];
        for (uint j = 0; j < neighbors.size(); ++j) { // projed vsechny neigh ktere jsou v setu
            uint neighbor = neighbors[j].second;
            if(!in_subset[neighbor]) continue;
            // od neigh degree se musi odecist ty ktere tam nejsou
            uint nei_degree = 0;
            for (uint k = 0; k < new_g[neighbor].size(); ++k) { // koukni na neighbors' neighbors, jestli jsou v setu, inkrementuj degree
                if(in_subset[new_g[neighbor][k].second]) {
                    nei_degree++;
                }
            }
            d2[nei_degree]++;

        }
        node_d2[curr_node] = d2;
    }
    return node_d2;
}

// BFS grafem, pokud se dostaneme do vsech nodu, vraci true
// TODO: skipuj, je to validovane v D2!!!
bool is_connected(vector<bool> &in_subset, vector<uint> &curr_subset, vector<vector<wd_pair>> &new_g) {
    // udelej jedno BFS z nejakeho uzlu v curr_subset
    // do souseda vejdi jen pokud je v curr_subset
    queue<uint> q;
    vector<bool> seen(30, false);
    q.push(curr_subset[0]); // vloz prvni subset node
    while (!q.empty()) {
        uint curr = q.front(); q.pop(); // vyndej node z fronty
        seen[curr] = true; // node navstiveny jakmile zpracujeme jeho uzly
        for(wd_pair wd_nei : new_g[curr]) { // koukni na nodovo sousedy
            uint neighbor = wd_nei.second;
            if(in_subset[neighbor] && !seen[neighbor]) { // pokud je soused v subsetu, dej ho do fronty
                q.push(neighbor);
            }
        }
    }

    for (uint i = 0; i < seen.size(); ++i) {
        bool node_seen = seen[i];
        bool node_in_subset = in_subset[i];
        if(node_in_subset) { // nodes in subset should all be visited
            if(!node_seen) {
                return false;
            }
        }
    }
    return true;
}

bool d1_matches(        vector<bool> &in_subset,
                        vector<uint> &curr_subset,
                        vector<vector<wd_pair>> &new_g,
                        map<uint,uint> &d1_old_map
) {
    // zkontroluj ze d1 stupne uzlu jsou stejne
    vector<uint> d1_old_sorted;
    for (auto p : d1_old_map) d1_old_sorted.push_back(p.second);

    map<uint,uint> d1_new_map = get_new_d1(new_g, in_subset, curr_subset);
    vector<uint> d1_new_sorted;
    for (auto p : d1_new_map) d1_new_sorted.push_back(p.second);

    sort(d1_old_sorted.begin(), d1_old_sorted.end());
    sort(d1_new_sorted.begin(), d1_new_sorted.end());
    for (uint i = 0; i < d1_old_sorted.size(); ++i) {
        if (d1_old_sorted[i] != d1_new_sorted[i]) {
//            cout << "d1 mapping NOT ok... \n";
//            return {{}};
            return false;
        }
    }
//    cout << "d1 mapping ok... ";
    return true;
}


/// najde mozne mappingy jednoho src na nekolik dst
vector<vector<uint>> find_possible_mappings(
        vector<bool> &in_subset,
        vector<uint> &curr_subset,
        vector<vector<wd_pair>> &new_g,
        vector<vector<uint>> &old_g,
        map<uint,uint> &d1_old_map,
        map<uint,vector<uint>> &d2_old
    ) {

    // kontrola stupnu obou grafu
    if(!d1_matches(in_subset, curr_subset, new_g, d1_old_map)) {
        return {};
    }

    vector<vector<uint>> mapping(old_g.size());
    vector<bool> mapped_nodes_old(old_g.size(), false);
    map<uint, vector<uint>> d2_new = get_new_d2(new_g, curr_subset, in_subset);

    for (auto const& op : d2_old) { // pro vsechny vektory D2 ve starem
        uint curr_old_node = op.first;
        vector<uint> old_nei_degs = op.second;
        // najdi tento vektor uvnitr d2_new
        // pokud tam neni, vrat prazdnej mapping


        for (auto const& np : d2_new) { // pro vsechny vektory D2 v novem
            uint curr_new_node = np.first;
            if(!in_subset[curr_new_node]) continue;
            vector<uint> new_nei_degs = np.second;
            bool vectors_equal = true;

            for (uint nei_degree = 0; nei_degree < old_nei_degs.size(); ++nei_degree) { // prochazime oba vektory najednou
                uint old_deg = old_nei_degs[nei_degree];
                uint new_deg = new_nei_degs[nei_degree];
                if(old_deg != new_deg) { // zaznamy se nerovnaji-> mame spatnej match D2_old a D2_new
                    vectors_equal = false;
                    break;
                }
            }
            if (vectors_equal) {
                mapping[curr_old_node].push_back(curr_new_node);
                mapped_nodes_old[curr_old_node] = true;
            }

        }
    }
    for (uint i = 0; i < mapped_nodes_old.size(); ++i) {
        if(!mapped_nodes_old[i]) {
//            cout << "    D2 mapping NOT ok. \n";
            return {};
        }
    }

//    cout << "    D2 mapping ok. \n";
    // porovnej vectory v mapach, najdi matchujici

    // prmutace uvnitr skupin daji vhodne mappings
    // pro kazdy mapping:
    // zkontroluj kazdou hranu v mappingu zda existuje v novem i starem grafu

    return mapping;
}

uint fact(uint num) {
    if (num == 1) return 1;
    if (num == 2) return 2;
    if (num == 3) return 9;
    if (num == 4) return 24;
    if (num == 5) return 120;
    if (num == 6) return 720;
    if (num == 7) return 5040;
    if (num == 8) return 40320;
    if (num == 9) return 362880;
    if (num == 10) return 3628800;
    return 0;
}

bool decrement(vector<uint> &factorials, vector<uint> &factorials_original) {
    bool carry = false;
    for (uint i = (uint)factorials.size(); i-- > 0; ) {
        uint last = factorials[i];
        if(last == 1) {
            if(i != 0) {
                factorials[i] = factorials_original[i];
                carry = true;
            }
            else if(i==0) return false;
        }
        else { // if (last != 1)
            if(carry) {
                factorials[i] = factorials[i]-1;
                return true;
            }
            else { //if(!carry) {
                factorials[i] = factorials[i]-1;
                return true;
            }
        }
    }
    return false;
}

vector<uint> gen_perms(vector<uint> &factorials, vector<pair<vector<uint>,vector<uint>>> &o_n_mapping_pairs, vector<vector<uint>> &old_g) {

    vector<vector<uint>> perm(factorials.size());
    for (uint i = 0; i < factorials.size(); ++i) {
        uint rank = factorials[i];
        auto m = o_n_mapping_pairs[i].second;
        for (uint j = 0; j < rank; ++j) {
            prev_permutation(m.begin(), m.end());
        }
        perm[i] = m;
    }
    vector<uint> index_map(old_g.size());
    for (uint p = 0; p < o_n_mapping_pairs.size(); ++p) {
        auto pair = o_n_mapping_pairs[p];
        for (uint i = 0; i < pair.first.size(); ++i) {
            uint old_node = pair.first[i];
            uint new_node = perm[p][i];
            index_map[old_node] = new_node;
        }
    }
    return index_map;
}

bool check_mapping(vector<uint> &index_map, vector<vector<uint>> &old_g, vector<vector<wd_pair>> &new_g) {
    for (uint i = 0; i < old_g.size(); ++i) {
        for (uint j = 0; j < old_g[i].size(); ++j) {
            uint s = i;
            uint d = old_g[i][j];
//            pair<uint, uint> old_edge = {s, d};
            pair<uint, uint> new_edge = {index_map[s], index_map[d]};
            // does such new_edge exist in new_g?
            bool edge_exists = false;
            for (uint k = 0; k < new_g[new_edge.first].size(); ++k) {
                uint new_d = new_g[new_edge.first][k].second;
                if(new_d == new_edge.second) {
                    edge_exists = true;
                    break;
                }
            }
            if (!edge_exists) {
                return false;
            }
        }
    }
    return true;
}

uint test_mappings_edges(vector<vector<uint>> &possible_mappings, vector<vector<uint>> &old_g, vector<vector<wd_pair>> &new_g, vector<bool> &in_subset) {

    vector<pair<vector<uint>, vector<uint>>> o_n_pairing; /// old-new pairing - ktere uzly na ktere lze mapovat

    for (uint i = 0; i < possible_mappings.size(); ++i) {
        uint mapping_src = i;
        vector<uint> dst_list = possible_mappings[i];
        if(dst_list.size()==1) {
            o_n_pairing.push_back({{mapping_src}, dst_list });
        } else {
            uint dst_mapping_index = UINT_MAX;
            for (uint j = 0; j < o_n_pairing.size(); ++j) {
                if(o_n_pairing[j].second == dst_list) {
                    dst_mapping_index = j;
                    break;
                }
            }
            if(dst_mapping_index != UINT_MAX) { // dst_mapping je v o_n_pairingu
                o_n_pairing[dst_mapping_index].first.push_back(mapping_src);
            } else { // dst_mapping NENI v o_n_pairingu
                o_n_pairing.push_back({{mapping_src}, dst_list });
            }
        }
    }

    vector<uint> factorials;
    for (uint i = 0; i < o_n_pairing.size(); ++i) {
        uint s = (uint)o_n_pairing[i].first.size();
        uint f = fact(s);
        factorials.push_back(f);
    }

    auto factorials_original = factorials;
    vector<vector<uint>> valid_mappings;
    do {
        // generujeme permutace moznych mapovani na uzly
        vector<uint> index_map = gen_perms(factorials, o_n_pairing, old_g);
        // kontrolujeme zda se mapovane hrany nachazi v grafu
        bool valid_mapping = check_mapping(index_map, old_g, new_g);
        if(valid_mapping) {
//            cout << "           [";  for (uint n : index_map) cout << n << ", ";  cout << "]\n";
//            cout << "           valid mapping\n";
            valid_mappings.push_back(index_map);
            break;
        }
    } while (decrement(factorials, factorials_original));

    uint min_cost = UINT_MAX;
    for (auto vm : valid_mappings) {
        vector<uint> visited(new_g.size(), false);
        uint c_sum = 0;
        for (uint src : vm) {
            for(wd_pair nei : new_g[src]) {
                uint w = nei.first;
                uint d = nei.second;
                if(!in_subset[d]) continue;
                if(visited[d]) continue;
                c_sum += w; // wd_pair -> {weight, destination}
            }
            visited[src] = true;
        }
        min_cost = min(min_cost,c_sum);
    }

    return min_cost;
}


void k_subsets3a(vector<uint> &set, uint k, uint i_start,
                 vector<uint> &result, uint depth, vector<vector<uint>> &collector) {
    if(depth == k) {
        collector.push_back(result);
        return;
    }

    uint i_lastStart = (uint) set.size() - (k - depth);
    for (uint i = i_start; i < i_lastStart+1; ++i) {
        result[depth] = set[i];
        k_subsets3a(set, k, i + 1, result, depth + 1, collector);
    }
}




int main() {
    // get old/small network
    uint n1, m1;
    cin >> n1 >> m1;
    vector<vector<uint>> old_g = get_old_net(n1, m1);

    // get new/big network
    uint n2, m2, F;
    cin >> n2 >> m2 >> F;
    vector<bool>is_f(n2,false);
    vector<uint>all_f = get_all_f(F, is_f);

    vector<uint>all_s;
    for (uint i = 0; i < n2; ++i) if(!is_f[i]) all_s.push_back(i);

    vector<vector<wd_pair>> new_g = get_new_net(n2, m2);

    // predpocitej degrees uzlu a degrees sousedu
    map<uint,uint> old_d1 = get_old_d1(old_g);
    map<uint,vector<uint>> old_d2 = get_old_d2(old_g);

    /// postupuj od nejvyssiho poctu rychlych serveru F
    bool decrease_f = true;
    uint overall_min = UINT_MAX;
    uint output_f = F;
    for(auto curr_f = F; curr_f > 0 ; curr_f--) {

        if (!decrease_f) break;


        uint curr_s;
        if(curr_f > n1) {
            curr_s = 0;
            curr_f = n1;
        } else{
            curr_s = n1 - curr_f;
        }

        vector<vector<uint>> f_subsets{};
        vector<vector<uint>> s_subsets{};
        vector<uint> empty_f(curr_f, 0);
        vector<uint> empty_s(curr_s, 0);
        Subseter f = Subseter();
        Subseter s = Subseter();
        f.subset(all_f, curr_f);
        s.subset(all_s, curr_s);

        /// generuj vsechny mozne vybery rychlych a pomalych serveru
//        k_subsets3a(all_f, curr_f, 0, empty_f, 0, f_subsets);
//        k_subsets3a(all_s, curr_s, 0, empty_s, 0, s_subsets);

        for (auto f_sub : f.subsets) {
            for (auto s_sub : s.subsets) {
                vector<uint> the_subset;
                the_subset.insert(the_subset.end(), f_sub.begin(), f_sub.end());
                the_subset.insert(the_subset.end(), s_sub.begin(), s_sub.end());

                vector<bool> in_subset(n2, false);
                for (auto node : the_subset) in_subset[node] = true;

//                cout <<" the_subset: (f=" << curr_f <<")  [";
//                for (auto node : the_subset) cout << node << ", ";
//                cout << "]  \n";

                if(!is_connected(in_subset, the_subset, new_g)) {
//                    cout << "               disconnected\n";
                    continue;
                }

                vector<vector<uint>> possible_mappings = find_possible_mappings(
                        in_subset, the_subset, new_g,
                        old_g, old_d1, old_d2
                );
                if(possible_mappings.empty()) continue;

                /// nyni lze opt najit s F = curr_f !!
                decrease_f = false; // breakuje vrchni iteraci z MAX(F),
                output_f = curr_f;
                uint min_cost = test_mappings_edges(possible_mappings, old_g, new_g, in_subset);
                overall_min = min(overall_min, min_cost);
            }
        }
    }
    cout << output_f << " " << overall_min;

    return 0;
}


//  Priblizny postup:
//   ✅ Premapujeme vsechny uzly v New tak aby FAST byly na zacatku
//   ✅ generujeme k-subsety (protoze jsme premapovali tak prvni subsety jsou s nejvice FAST)
//   ✅ pro kazdy subset (bincoef(S,k)-krat) zkontrolujeme Invarianty
//        [shoda degrees, pravidlo sousedu ze zadani, planarita, spojitost, ...]
//  ->  Pokud se v subsetu shoduji invarianty zjisti zda existuje mapping
//      Pokud existuje mapping, zapamatuj si pocet jeho F
//      Dale generuj pouze s timto poctem F
//      nakonec pro kazdy mapping spocti cenu
