//
// Created by kuzdavoj@fel.cvut.cz on 1/1/2023.
//


#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <queue>

using namespace std;

struct MHtype {
    int M;
    int H;

    MHtype(int m, int h) : M(m), H(h) {}


    bool operator<(const MHtype &rhs) const {
        if (M < rhs.M)
            return true;
        if (rhs.M < M)
            return false;
        return H > rhs.H;
    }

    bool operator>(const MHtype &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const MHtype &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const MHtype &rhs) const {
        return !(*this < rhs);
    }
};


// Function to calculate the minimum of three values
int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}

/// Function to calculate the Levenshtein distance between two strings
vector<int> last_row_levenDist(string shorter, string longer, bool print_table) {
    int len1 = (int) shorter.length();
    int len2 = (int) longer.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    for (int i = 1; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (int j = 1; j <= len2; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (shorter[i - 1] == longer[j - 1]) {
                /// chars are same, cost is zero
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                /// chars are different, cost  is + 1
                int del_cost = dp[i - 1][j];
                int ins_cost = dp[i][j - 1];
                int sub_cost = dp[i - 1][j - 1];
                dp[i][j] = 1 + minimum(del_cost, ins_cost, sub_cost);
            }
        }
    }

    if(print_table) {

        // print dlouheho stringu nahore
        cout << "    ";
        for(char c : longer) cout << c << " ";
        cout << endl;

        for (int i = 0; i < dp.size(); ++i) {
            if(i != 0) cout << shorter[i-1] << " ";
            if(i == 0) cout << "  ";
            for (int j = 0; j < dp[0].size(); ++j) {

                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<int> last_row = dp[dp.size()-1];
    return last_row;
}




vector<int> find_next_cuts(string P, string T, int D) {
    vector<int> res = last_row_levenDist(P, T, false);
    vector<int> found_idxs={};

    for (int i = 0; i < res.size(); ++i) {
        if (res[i] <= D) {
            found_idxs.push_back(i);
        }
    }

    return found_idxs;
}




MHtype rec_help(const string& P, const string& T, int D) {

    /// base case
    if(T.length() < 2) {
        if(T.empty())
            return MHtype{0,0};  /// prazdnej string -> nic
        return MHtype{0,1};   /// jedno pismeno -> H++
    }

    vector<int> found_idxs = find_next_cuts(P, T, D);

    if (!found_idxs.empty()) {
        priority_queue<MHtype> PQ;

        for (int cut_index : found_idxs) {
            string new_T = T.substr(cut_index, T.length());
            PQ.push(rec_help(P, new_T, D));
        }
        MHtype best = PQ.top();
        best.M += 1;
        return best;
    } else {
        string new_T = T.substr(1, T.length());
        MHtype solution = rec_help(P, new_T, D);
        solution.H += 1;
        return solution;
    }
}

int _main() {
    auto a = rec_help("abb", "aabbbcabab", 1);
    cout << a.M << " " << a.H << endl;
}



int main() {
    string T;
    string P;
    int D;
    cin >> T;
    cin >> P;
    cin >> D;

    auto a = rec_help(P, T, D);
    cout << a.M << " " << a.H << endl;
}