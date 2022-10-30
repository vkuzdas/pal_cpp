//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;
using uint = unsigned int;
using wd_pair = pair<uint, uint>;

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
    // reads graph from input and represents it as mirrored directed graph in adj list
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
void get_fast_servers(uint f, vector<bool> &is_fast, vector<uint> &f_servers) {
    for (uint i = 0; i < f; ++i) {
        uint edge_label;
        cin >> edge_label;
        is_fast[edge_label] = true;
        f_servers[i] = edge_label;
    }
}

// Chceme fast servery co nejvice na levo
// vytvorime mapping kde jako prvni hodime fasty, potom ostatni servery
// new_g premapujeme dle tohoto mappingu
vector<vector<wd_pair>> remap_new_g(vector<vector<wd_pair>> &new_g, vector<bool> &fast_s_bool, vector<uint> &fast_nodes) {
    vector<uint> map(new_g.size());
    for (uint i = 0; i < fast_nodes.size(); ++i) {
        auto fast_node = fast_nodes[i];
        map[fast_node] = i;
    }
    auto itr = (uint)fast_nodes.size();
    for (uint i = 0; i < new_g.size(); ++i) {
        if (fast_s_bool[i]) continue;
        auto slow_node = i;
        map[slow_node] = itr;
        itr++;
    }

    vector<vector<wd_pair>> remapped(new_g.size());
    for (uint i = 0; i < new_g.size(); ++i) {
        for (uint j = 0; j < new_g[i].size(); ++j) {
            uint s, w, d, rem_s, rem_d;
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
    uint block_sizes = 0;
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
        vector<uint> d2(10, 0); // 10 nodu, zadne degrees
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

vector<vector<uint>> find_mappings(
        vector<bool> &in_subset,
        vector<uint> &curr_subset,
        vector<vector<wd_pair>> &new_g,
        vector<vector<uint>> &old_g,
        map<uint,uint> &d1_old_map,
        map<uint,vector<uint>> &d2_old
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
            cout << "d1 mapping NOT ok... \n";
            return {{}};
        }
    }
    cout << "d1 mapping ok... ";

    vector<vector<uint>> mapping(10);
    vector<bool> mapped_nodes_old(old_g.size(), false);
    vector<bool> mapped_nodes_new(30, false);
    map<uint, vector<uint>> d2_new = get_new_d2(new_g, curr_subset, in_subset);
    for (auto const& op : d2_old) { // pro vsechny vektory D2 ve starem
        uint curr_old_node = op.first;
        vector<uint> old_nei_degs = op.second;
        // najdi tento vektor uvnitr d2_new
        // pokud tam neni, vrat prazdnej mapping
        for (auto np : d2_new) { // pro vsechny vektory D2 v novem
            uint curr_new_node = np.first;
            vector<uint> new_nei_degs = np.second;
            for (uint k = 0; k < old_nei_degs.size(); ++k) { // pro kazdy zaznam D2 vektoru ve starem
                if(old_nei_degs[k] != new_nei_degs[k]) { // zaznamy se nerovnaji-> mame spatnej match D2_old a D2_new
                    continue;
                } else { // pokud se rovnaji, zaznamename o ktere uzly se jedna
                    mapping[curr_old_node].push_back(curr_new_node);
                    mapped_nodes_old[curr_old_node] = true;
                    mapped_nodes_new[curr_new_node] = true;
                }
            }
        }
    }
    for(auto node : mapped_nodes_old) {
        if(!mapped_nodes_old[node]) {
            cout << "    D2 mapping NOT ok. \n";
            return {};
        }
    }

    cout << "    D2 mapping ok. \n";
    // porovnej vectory v mapach, najdi matchujici


    // rozdel uzly do skupin dle degrees
    // skupiny z old a new porovnej
    // meli by byt shodne
    // rozedl uzly do skupin dle neighbor degrees
    // prmutace uvnitr skupin daji vhodne mappings
    // pro kazdy mapping:
    // zkontroluj kazdou hranu v mappingu zda existuje v novem i starem grafu

    return mapping;
}


int main() {
    // get old
    uint n1, m1;
    cin >> n1 >> m1;
    vector<vector<uint>> old_g = get_old_net(n1, m1);

    // get new
    uint n2, m2, f;
    cin >> n2 >> m2 >> f;
    vector<bool>is_fast(30); // 1 ≤ F2 ≤ N2, 5 ≤ N2 ≤ 30
    vector<uint>f_servers(f, false);
    get_fast_servers(f, is_fast, f_servers);
    vector<vector<wd_pair>> new_g = get_new_net(n2, m2);

    // remapuj aby fast servery byly na prvnich mistech -> generovat budeme nejdrive MAX(F)
    new_g = remap_new_g(new_g, is_fast, f_servers);
    // kombinacni cislo: n1-servrové kombinace z n2 serveru [napr 5 serveru z 10]
    uint last_rank = bin_coeff(n2, n1);
    map<uint,uint> old_d1 = get_old_d1(old_g);
    map<uint,vector<uint>> old_d2 = get_old_d2(old_g);
    for (uint i = 0; i < last_rank; ++i) { // TODO: teoreticky by se dalo jit z obou koncu (prevratit mapping)

        vector<uint> candidate = unrank_subset(i, n2, n1);
        for (uint j = 0; j < candidate.size(); ++j) candidate[j] = candidate[j]-1; // TODO: linear time!

        cout << i << " / " << last_rank << " candidate:  [";
        for (uint j = 0; j < candidate.size(); ++j) cout << candidate[j] << ", ";
        cout << "]  ";

        vector<bool> in_subset(30, false);
        for (auto node : candidate) in_subset[node] = true;
        if(!is_connected(in_subset, candidate, new_g)) {
            setbuf(stdout, nullptr);
            cout << "disconnected.\n";
            continue;
        } else {
            setbuf(stdout, nullptr);
            cout << "connected, ";
        }

        vector<vector<uint>> mappings = find_mappings(
                in_subset, candidate,new_g,
                old_g, old_d1, old_d2
            );
    }

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
