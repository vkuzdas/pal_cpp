//
// Created by kuzdavoj@fel.cvut.cz on 10/23/2022.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using uint = unsigned int;
using wd_pair = pair<uint, uint>;

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

vector<uint> get_old_degrees_sorted(vector<vector<uint>> &old_g) {
    vector<uint> old_degrees;
    for (auto const& src : old_g) {
        old_degrees.push_back((uint)src.size());
    }
    std::sort(old_degrees.begin(), old_degrees.end());
    return old_degrees;
}

bool degrees_ok(vector<bool> &in_subset, vector<uint> &curr_subset, vector<vector<wd_pair>> &new_g, vector<vector<uint>> &old_g, vector<uint> &old_degrees_sorted) {
    // projdi vsechny uzly ze subsetu
    // koukni jaky degree maji uvnitr new grafu
    // odecti od degree ty ktere nejsou v subsetu
    vector<uint> new_degrees;
    for (uint i = 0; i < curr_subset.size(); ++i) {
        uint curr_node = curr_subset[i];
        vector<wd_pair> neighbors = new_g[curr_node];
        uint degree = 0;
        for (uint j = 0; j < neighbors.size(); ++j) {
            uint neigh = neighbors[j].second;
            if (in_subset[neigh]) {
                degree++;
            }
        }
        new_degrees.push_back(degree);
    }
    std::sort(new_degrees.begin(), new_degrees.end());
    for (uint i = 0; i < old_degrees_sorted.size(); ++i) {
        if (old_degrees_sorted[i] != new_degrees[i]) {
            return false;
        }
    }
    return true;
}

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

bool invariants_ok(vector<uint> &curr_subset, vector<vector<wd_pair>> &new_g, vector<vector<uint>> &old_g, vector<uint> &old_degrees_sorted) {
    vector<bool> in_subset(30, false);
    for (auto node : curr_subset) in_subset[node] = true;

    // check degrees
    if(!degrees_ok(in_subset, curr_subset, new_g, old_g, old_degrees_sorted)) {
        cout << "bad degrees. \n";
        return false;
    } else {cout << "ok degrees, ";}
    // check for connectivity
    if(!is_connected(in_subset, curr_subset, new_g)) {
        cout << "disconnected.\n";
        return false;
    } else {cout << "connected. \n";}
    // check if it is a tree [connected and n-1 edges]
    // check if it is planar
    // match neighbour old_degrees
    return false;
}

//bool neighbors_ok(vector<uint> &curr_subset, vector<vector<wd_pair>> &new_g, vector<vector<uint>> &old_g) {
//     pravidlo: uzly ktere nesousedili v old nesmi sousedit v new
//     tohle se bude checkovat po vygenerovani mappingu
//    return false;
//}

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
    vector<uint> old_degrees_sorted = get_old_degrees_sorted(old_g);
    for (uint i = 0; i < last_rank; ++i) { // TODO: teoreticky by se dalo jit z obou koncu (prevratit mapping)
        vector<uint> candidate = unrank_subset(i, n2, n1);
        for (uint j = 0; j < candidate.size(); ++j) candidate[j] = candidate[j]-1; // TODO: linear time!

        cout << i << "th candidate:  [";
        for (uint j = 0; j < candidate.size(); ++j) cout << candidate[j] << ", ";
        cout << "]  ";

        invariants_ok(candidate, new_g, old_g, old_degrees_sorted);
    }

    return 0;
}
