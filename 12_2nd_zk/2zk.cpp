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
    vector<int> new_d1 = A_D1;
    new_d1[ins_to_A.dst] ++;
    new_d1[ins_to_A.src] ++;
    sort(new_d1.begin(), new_d1.end());
    return new_d1;
}

vector<int> adjust_B_D1(vector<int> &A_D1, Edge &del_from_B) {
    vector<int> new_d1 = A_D1;
    new_d1[del_from_B.dst] ++;
    new_d1[del_from_B.src] ++;
    sort(new_d1.begin(), new_d1.end());
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
    vector<int> B_d1 = get_d1(B_adj);





    for (int i = 0; i < B_e_list.size(); ++i) {
        Edge del_from_B = B_e_list[i];

        vector<int> pick = {0,1}; // k=2, n = pocet uzlu, tzn vybiram dva uzly z celeho grafu
        do {

            Edge ins_to_A = {pick[0], pick[1],A_is_fast[pick[0]], A_is_fast[pick[1]]};

            // INV-prep: existujici hrany v A zahazuju
            if(e_exists(A_adj, ins_to_A)) {
                continue;
            }
//            print_choice(ins_to_A, del_from_B);

            // 1) Invariant: kolik je na hranach fastu?
            bool f_count_ok = f_count_check(del_from_B, ins_to_A, A_is_fast, B_is_fast);
            if(!f_count_ok)
                continue;

            // 2) Invariant: D1 adjusted
            vector<int> adjusted_A_D1 = adjust_A_D1(A_d1, ins_to_A);
            vector<int> adjusted_B_D1 = adjust_B_D1(A_d1, ins_to_A);
            if(adjusted_A_D1 != adjusted_B_D1) continue;

//            // 3) Invariant: D2 adjusted



        } while (next_K_subset(pick, N));
    }



    cout << "done";
}




int _main() {
    vector<int> a = {1,3,3};
    vector<int> b = {1,2,3};
    if(a==b)
        cout << "equal";

}