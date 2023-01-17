//
// Created by kuzdavoj@fel.cvut.cz on 1/16/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;



struct Edge {
    int src;
    int dst;
    bool src_fast;
    bool dst_fast;

    Edge(int src, int dst, bool srcFast, bool dstFast) : src(src), dst(dst), src_fast(srcFast), dst_fast(dstFast) {}
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
            if(src == ins_to_A.src && dst == ins_to_A.dst) {
                neigh_degs[dst_deg]++;
                neigh_degs[dst_deg]++;
            } else {
                neigh_degs[dst_deg]++;
            }
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
            if(src == del_from_B.src && dst == del_from_B.dst) {
//                neigh_degs[dst_deg]++;
//                neigh_degs[dst_deg]++;
            } else {
                neigh_degs[dst_deg]++;
            }
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
//    map<int, vector<int>> A_d2 = get_d2(A_adj, A_d1);

    vector<int> B_d1 = get_d1(B_adj);
//    map<int, vector<int>> B_d2 = get_d2(B_adj, B_d1);





    for (int i = 0; i < B_e_list.size(); ++i) {
        Edge del_from_B = B_e_list[i];

        vector<int> pick = {0,1}; // k=2, n = pocet uzlu, tzn vybiram dva uzly z celeho grafu
        do {

            Edge ins_to_A = {pick[0], pick[1],A_is_fast[pick[0]], A_is_fast[pick[1]]};

            // INV-prep: existujici hrany v A zahazuju
            if(e_exists(A_adj, ins_to_A)) {
                continue;
            }

            // 1) Invariant: kolik je na hranach fastu?
            bool f_count_ok = f_count_check(del_from_B, ins_to_A, A_is_fast, B_is_fast);
            if(!f_count_ok)
                continue;

            // 2) Invariant: D1 adjusted
            vector<int> a = {0,3};
            if(pick == a)
                cout << "bug";
            vector<int> adjusted_A_D1 = adjust_A_D1(A_d1, ins_to_A);
            vector<int> adjusted_B_D1 = adjust_B_D1(B_d1, del_from_B);

            if(!sorted_d1_equal(adjusted_A_D1, adjusted_B_D1))
                continue;



//            // 3) Invariant: D2 adjusted
//            map<int, vector<int>> adjusted_A_D2 = get_d2_A(A_adj, adjusted_A_D1, ins_to_A);
//            map<int, vector<int>> adjusted_B_D2 = get_d2_B(B_adj, adjusted_B_D1, del_from_B);

            print_choice(ins_to_A, del_from_B);

            vector<vector<int>> mapping(adjusted_A_D2.size());
            vector<bool> mapped_A_nodes(adjusted_A_D2.size(), false);
            for (auto const& op : adjusted_A_D2) { // pro vsechny vektory D2 v A
                int A_node = op.first;
                vector<int > old_nei_degs = op.second;
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
                        mapping[A_node].push_back(B_node);
                        mapped_A_nodes[A_node] = true;
                    }
                }
            }
//            if(!mapping.empty()) {
//                cout << "m not empty";
//            }
            bool gen_new = false;
            for (int i = 0; i < mapped_A_nodes.size(); ++i) {
                if(!mapped_A_nodes[i]) {
                    gen_new = true;
                    break;
//                    cout << "    D2 mapping NOT ok. \n";
//                    return {};
                }
            }
            if (gen_new) continue;
            print_choice(ins_to_A, del_from_B);
            cout << "    D2 mapping ok. \n";



        } while (next_K_subset(pick, N));
    }



    cout << "done";
}




//int _main() {
//    vector<int> a = {1,3,3};
//    vector<int> b = {1,2,3};
//    if(a==b)
//        cout << "equal";
//
//}