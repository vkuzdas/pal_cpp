//
// Created by kuzdavoj@fel.cvut.cz on 1/16/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

#include <climits>

struct Edge {
    int src;
    int dst;
    bool src_fast;
    bool dst_fast;

    Edge(int src, int dst, bool srcFast, bool dstFast) : src(src), dst(dst), src_fast(srcFast), dst_fast(dstFast) {}
};



struct result {
    int a;
    int b;
    int c;
    int d;

    bool operator<(const result &rhs) const {
        if (a < rhs.a)
            return true;
        if (rhs.a < a)
            return false;
        if (b < rhs.b)
            return true;
        if (rhs.b < b)
            return false;
        if (c < rhs.c)
            return true;
        if (rhs.c < c)
            return false;
        return d < rhs.d;
    }

    bool operator>(const result &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const result &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const result &rhs) const {
        return !(*this < rhs);
    }

    result(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {}
};

void print_choice(Edge &ins_to_A, Edge &del_from_B) {
    setbuf(stdout, nullptr);
    cout << "Del B:  "
         << del_from_B.src <<
         "(" << del_from_B.src_fast << ")" << "-" << del_from_B.dst <<
         "(" << del_from_B.dst_fast << ")   ";


    cout << "||  Ins A:    "
         << ins_to_A.src <<
         "(" << ins_to_A.src_fast << ")" << "-" << ins_to_A.dst <<
         "(" << ins_to_A.dst_fast << ")   \n";
//    for (auto i: pick) {
//        cout << " " << i;
//    }
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


//next_K_subset(helper2, N)


bool f_count_check(Edge del_from_B, Edge ins_to_A, const vector<bool>& A_is_fast, const vector<bool>& B_is_fast) {
    int B_f = 0;
    int A_f = 0;

    if (del_from_B.src_fast) B_f ++;
    if (del_from_B.dst_fast) B_f ++;
    if (ins_to_A.src_fast) A_f ++;
    if (ins_to_A.dst_fast) A_f ++;

    if(B_f == A_f) return true;
    return false;
}

map<int, vector<int>> get_d2(vector<vector<int>> &adj, vector<int> &d1) {
    map<int, vector<int>> node_d2;
    for (int src = 0; src < adj.size(); ++src) {

        vector<int> neigh_degs(adj.size(), 0);

        for (int dst = 0; dst < adj[src].size(); ++dst) {
            // pro kazdeho souseda
            // koukni jaky ma stupen
            int dst_node = adj[src][dst];
            int dst_deg = d1[dst_node];
            neigh_degs[dst_deg]++;
        }

        node_d2[src] = neigh_degs;
    }
    return node_d2;
}


// vrati pole sousedu
// index odpovidajici uzlu na sobe nese stupen
vector<int> get_d1(const vector<vector<int>>&  adj) {
    vector<int> d1(adj.size());
    for (int i = 0; i < adj.size(); ++i) {
        d1[i] = (int)adj[i].size();
    }
    return d1;
}


vector<int> adjust_A_D1(vector<int> &A_D1, Edge &ins_to_A) {
    vector<int> new_d1;
    for (auto i : A_D1) new_d1.push_back(i);
    new_d1[ins_to_A.dst] ++;
    new_d1[ins_to_A.src] ++;
    return new_d1;
}

vector<int> adjust_B_D1(vector<int> &B_D1, Edge &del_from_B) {
    vector<int> new_d1;
    for (auto i : B_D1) new_d1.push_back(i);
    new_d1[del_from_B.dst] --;
    new_d1[del_from_B.src] --;
    return new_d1;
}

bool e_exists(const vector<vector<int>>& adj, Edge &edge) {
    for (int i = 0; i < adj[edge.src].size(); ++i) {
        for(int dst : adj[edge.src]) {
            if (dst == edge.dst)
                return true;
        }
    }
    return false;
}


// node -> list kolik ma sousedu o stupni 1,2,3,4,5 ...
map<int, vector<int>> get_d2_A(vector<vector<int>> &adj, vector<int> &d1, Edge &ins_to_A) {
    map<int, vector<int>> node_d2;
    for (int src = 0; src < adj.size(); ++src) {

        vector<int> neigh_degs(adj.size(), 0);

        for (int dst = 0; dst < adj[src].size(); ++dst) {
            // pro kazdeho souseda
            // koukni jaky ma stupen
            int dst_node = adj[src][dst];
            int dst_deg = d1[dst_node];
            neigh_degs[dst_deg]++;
        }

        node_d2[src] = neigh_degs;
    }
    return node_d2;
}

// node -> list kolik ma sousedu o stupni 1,2,3,4,5 ...
map<int, vector<int>> get_d2_B(vector<vector<int>> &adj, vector<int> &d1, Edge &del_from_B) {
    map<int, vector<int>> node_d2;
    for (int src = 0; src < adj.size(); ++src) {

        vector<int> neigh_degs(adj.size(), 0);

        for (int dst = 0; dst < adj[src].size(); ++dst) {
            // pro kazdeho souseda
            // koukni jaky ma stupen
            int dst_node = adj[src][dst];
            int dst_deg = d1[dst_node];
            neigh_degs[dst_deg]++;
        }

        node_d2[src] = neigh_degs;
    }
    return node_d2;
}


bool sorted_d1_equal(vector<int> d1_A, vector<int> d1_B) {
    sort(d1_A.begin(), d1_A.end());
    sort(d1_B.begin(), d1_B.end());
    if(d1_A==d1_B) return true;
    return false;
}

//A_d2, adjusted_A_D1, ins_to_A
map<int, vector<int>> add_to_D2(const map<int, vector<int>>& A_d2, vector<int>& adjusted_A_D1, Edge ins_to_A, vector<vector<int>> &adj, vector<vector<int>> &new_adj_A) {
    new_adj_A[ins_to_A.src].push_back(ins_to_A.dst);
    new_adj_A[ins_to_A.dst].push_back(ins_to_A.src);
    return get_d2(new_adj_A, adjusted_A_D1);
}


//A_d2, adjusted_A_D1, ins_to_A
map<int, vector<int>> del_from_D2(const map<int, vector<int>>& A_d2, vector<int>& adjusted_A_D1, Edge del_from_B, vector<vector<int>> &adj, vector<vector<int>> &new_adj_B) {
    auto pos = find(
            new_adj_B[del_from_B.src].begin(), new_adj_B[del_from_B.src].end(),
            del_from_B.dst);
    new_adj_B[del_from_B.src].erase(pos);
    pos = find(
            new_adj_B[del_from_B.dst].begin(), new_adj_B[del_from_B.dst].end(),
            del_from_B.src);
    new_adj_B[del_from_B.dst].erase(pos);
    return get_d2(new_adj_B, adjusted_A_D1);
}



vector<int> gen_perms(vector<int> &factorials, vector<pair<vector<int>,vector<int>>> &o_n_mapping_pairs, vector<vector<int>> &old_g) {

    vector<vector<int>> perm(factorials.size());
    for (int i = 0; i < factorials.size(); ++i) {
        int rank = factorials[i];
        auto m = o_n_mapping_pairs[i].second;
        for (int j = 0; j < rank; ++j) {
            prev_permutation(m.begin(), m.end());
        }
        perm[i] = m;
    }
    vector<int> index_map(old_g.size());
    for (int p = 0; p < o_n_mapping_pairs.size(); ++p) {
        auto pair = o_n_mapping_pairs[p];
        for (int i = 0; i < pair.first.size(); ++i) {
            int old_node = pair.first[i];
            int new_node = perm[p][i];
            index_map[old_node] = new_node;
        }
    }
    return index_map;
}


int fact(int num) {
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

bool decrement(vector<int> &factorials, vector<int> &factorials_original) {
    bool carry = false;
    for (int i = (int)factorials.size(); i-- > 0; ) {
        int last = factorials[i];
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

/// kontroluje jestli mapovane hrany existuji v obou grafech
bool check_mapping(vector<int> &index_map, vector<vector<int>> &adj_A, vector<vector<int>> &adj_B) {
    for (int i = 0; i < adj_A.size(); ++i) {
        for (int j = 0; j < adj_A[i].size(); ++j) {
            int s = i;
            int d = adj_A[i][j];
//            pair<int, int> old_edge = {s, d};
            pair<int, int> new_edge = {index_map[s], index_map[d]};
            // does such new_edge exist in adj_B?
            bool edge_exists = false;
            for (int k = 0; k < adj_B[new_edge.first].size(); ++k) {
                int new_d = adj_B[new_edge.first][k];
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

bool test_mappings_edges_OK(vector<vector<int>> &possible_mappings,
                            vector<vector<int>> &adj_A, vector<vector<int>> &adj_B,
                            vector<bool> &A_is_fast, vector<bool> &B_is_fast) {

    vector<pair<vector<int>, vector<int>>> B_A_pairing; // ktere uzly jdou na ktere uzly

    for (int i = 0; i < possible_mappings.size(); ++i) {
        int mapping_src = i;
        vector<int> dst_list = possible_mappings[i];
        if(dst_list.size()==1) {
            B_A_pairing.push_back({{mapping_src}, dst_list });
        } else {
            int dst_mapping_index = INT_MAX;
            for (int j = 0; j < B_A_pairing.size(); ++j) {
                if(B_A_pairing[j].second == dst_list) {
                    dst_mapping_index = j;
                    break;
                }
            }
            if(dst_mapping_index != INT_MAX) { // dst_mapping je v B_A_pairingu
                B_A_pairing[dst_mapping_index].first.push_back(mapping_src);
            } else { // dst_mapping NENI v B_A_pairingu
                B_A_pairing.push_back({{mapping_src}, dst_list });
            }
        }
    }

    vector<int> factorials; // kolik faktorialu nutno provest pro kazdej pairing
    for (int i = 0; i < B_A_pairing.size(); ++i) {
        int s = (int)B_A_pairing[i].first.size();
        int f = fact(s);
        factorials.push_back(f);
    }

    auto factorials_original = factorials;
    vector<vector<int>> valid_mappings;
    do {
        vector<int> index_map = gen_perms(factorials, B_A_pairing, adj_A);//
        // check mapping = kontrola zda mapovane hrany existuji v obou grafech
        bool valid_mapping = check_mapping(index_map, adj_A, adj_B);
        if(valid_mapping) {
//            cout << "           [";  for (int n : index_map) cout << n << ", ";  cout << "]\n";
//            cout << "           valid mapping\n";
            valid_mappings.push_back(index_map);
            break;
        }
    } while (decrement(factorials, factorials_original));

    vector<int> invalid_mappings;
    for (int m_idx = 0; m_idx < valid_mappings.size(); ++m_idx) {
        for (int m_src = 0; m_src < valid_mappings[m_idx].size(); ++m_src) {
            int m_dst = valid_mappings[m_idx][m_src];
            if(A_is_fast[m_src] != B_is_fast[m_dst]) {
                invalid_mappings.push_back(m_idx);
                break;
            }
        }
    }

    if(valid_mappings.empty()) return false;
    if(invalid_mappings.size() == valid_mappings.size()) return false;
    return true;
}







int main() {
    int N; // # servery
    int C; // # planovane spoje tzn B ma C+1, A ma C-1
    int F; // # fast servery

    cin >> N >> C >> F;
    vector<vector<int>> A_adj(N);
    vector<vector<int>> B_adj(N);

    vector<bool> A_is_fast(N, false);
    vector<Edge> A_e_list;
    vector<int> A_fast_list;

    vector<bool> B_is_fast(N, false);
    vector<Edge> B_e_list;
    vector<int> B_fast_list;

    for (int i = 0; i < F; ++i) {
        int f;
        cin >> f;
        A_is_fast[f] = true;
        A_fast_list.push_back(f);
    }

    for (int i = 0; i < C-1; ++i) {
        int src;
        int dst;
        cin >> src >> dst;
        A_adj[src].push_back(dst);
        A_adj[dst].push_back(src);
        A_e_list.emplace_back(src, dst, A_is_fast[src], A_is_fast[dst]);
    }

    for (int i = 0; i < F; ++i) {
        int f;
        cin >> f;
        B_is_fast[f] = true;
        B_fast_list.push_back(f);
    }

    for (int i = 0; i < C+1; ++i) {
        int src;
        int dst;
        cin >> src >> dst;
        B_adj[src].push_back(dst);
        B_adj[dst].push_back(src);
        B_e_list.emplace_back(src, dst, B_is_fast[src], B_is_fast[dst]);
    }

    vector<int> A_d1 = get_d1(A_adj);
    map<int, vector<int>> A_d2 = get_d2(A_adj, A_d1);

    vector<int> B_d1 = get_d1(B_adj);
    map<int, vector<int>> B_d2 = get_d2(B_adj, B_d1);

    vector<result> results;



    for (int i = 0; i < B_e_list.size(); ++i) {
        Edge del_from_B = B_e_list[i];
        vector<int> adjusted_B_D1 = adjust_B_D1(B_d1, del_from_B);
        vector<vector<int>> new_adj_B(B_adj);
        map<int, vector<int>> adjusted_B_D2 = del_from_D2(B_d2, adjusted_B_D1, del_from_B, B_adj, new_adj_B);

        vector<int> pick = {0,1}; // k=2, n = pocet uzlu, tzn vybiram dva uzly z celeho grafu
        do {


            Edge ins_to_A = {pick[0], pick[1],A_is_fast[pick[0]], A_is_fast[pick[1]]};
//            if (del_from_B.src == 3 && del_from_B.dst == 0) {
//                cout << "bug\n";
//            }
            result r = {ins_to_A.src, ins_to_A.dst, min(del_from_B.src, del_from_B.dst), max(del_from_B.src, del_from_B.dst)};
            // INV-prep: existujici hrany v A zahazuju
            if(e_exists(A_adj, ins_to_A)) {
                continue;
            }

            // 1) Invariant: kolik je na hranach fastu?
//            bool f_count_ok = f_count_check(del_from_B, ins_to_A, A_is_fast, B_is_fast);
//            if(!f_count_ok)
//                continue;

            // 2) Invariant: D1 adjusted
            vector<int> adjusted_A_D1 = adjust_A_D1(A_d1, ins_to_A);


            if(!sorted_d1_equal(adjusted_A_D1, adjusted_B_D1))
            {


                continue;
            }



//            // 3) Invariant: D2 adjusted
//            0 1 0 3

            vector<vector<int>> new_adj_A(A_adj);
            map<int, vector<int>> adjusted_A_D2 = add_to_D2(A_d2, adjusted_A_D1, ins_to_A, A_adj, new_adj_A);


//            print_choice(ins_to_A, del_from_B);

            vector<vector<int>> mapping(adjusted_A_D2.size());
            vector<bool> mapped_A_nodes(adjusted_A_D2.size(), false);
            for (auto const& op : adjusted_A_D2) { // pro vsechny vektory D2 v A
                int A_node = op.first;
                vector<int> old_nei_degs = op.second;
                // najdi tento vektor uvnitr adjusted_B_D2
                // pokud tam neni, vrat prazdnej mapping

                for (auto const& np : adjusted_B_D2) { // pro vsechny vektory D2 v B

                    int B_node = np.first;
                    vector<int> new_nei_degs = np.second;
                    bool vectors_equal = true;

                    for (int nei_degree = 0; nei_degree < old_nei_degs.size(); ++nei_degree) { // prochazime oba vektory najednou
                        int old_deg = old_nei_degs[nei_degree];
                        int new_deg = new_nei_degs[nei_degree];
                        if(old_deg != new_deg) { // zaznamy se nerovnaji-> mame spatnej match adjusted_A_D2 a adjusted_B_D2
                            vectors_equal = false;
                            break;
                        }
                    }
                    if (vectors_equal) {
                        if ((A_is_fast[A_node] && B_is_fast[B_node]) || (!A_is_fast[A_node] && !B_is_fast[B_node])) {
                            if(adjusted_A_D1[A_node] == adjusted_B_D1[B_node]){
                                mapping[A_node].push_back(B_node);
                                mapped_A_nodes[A_node] = true;
                            }
                        }
                    }
                }
            }
            bool gen_new = false;
            for (int i = 0; i < mapped_A_nodes.size(); ++i) {
                if(!mapped_A_nodes[i]) {
                    gen_new = true;
                    break;
                }
            }
            if (gen_new) {
//                cout << "d2 wrong.....  ";
//                cout << ins_to_A.src << " " << ins_to_A.dst
//                     << " " << min(del_from_B.src, del_from_B.dst)
//                     << " " << max(del_from_B.src, del_from_B.dst) << "\n";
                continue;
            }
            if (!test_mappings_edges_OK(mapping, new_adj_A, new_adj_B, A_is_fast, B_is_fast)) continue;

            results.emplace_back(ins_to_A.src, ins_to_A.dst, min(del_from_B.src, del_from_B.dst), max(del_from_B.src, del_from_B.dst));

        } while (next_K_subset(pick, N));
    }


    sort(results.begin(), results.end());
    for (int i = 0; i < results.size(); ++i) {
        result r = results[i];
        cout << r.a << " " << r.b << " " << r.c << " " << r.d << endl;
    }
}




//int _main() {
//    vector<int> a = {1,3,3};
//    vector<int> b = {1,2,3};
//    if(a==b)
//        cout << "equal";
//
//}