//
// Created by kuzdavoj@fel.cvut.cz on 1/1/2023.
//


#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;


// Function to calculate the minimum of three values
int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}

/// Function to calculate the Levenshtein distance between two strings
vector<int> editor_ld(string shorter, string longer, bool print_table) {
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
    vector<int> res = editor_ld(P, T, false);
    vector<int> found_idxs={};

    for (int i = 0; i < res.size(); ++i) {
        if (res[i] <= D) {
            found_idxs.push_back(i);
        }
    }

    return found_idxs;
}


int main() {
    string T;
    string P;
    int D;
    cin >> T;
    cin >> P;
    cin >> D;

    int M = 0;
    int H = 0;

    while(!T.empty()){
        vector<int> found_idxs = find_next_cuts(P, T, D);

        if (!found_idxs.empty()) {
            int idx = found_idxs[0];
            T = T.substr(idx, T.length());
            M ++;
            // pokud jsme nasli vice vyhovujicich, tak chceme pro kazdy zjistit kolikrat
        } else {
            H++;
            T = T.substr(1, T.length());
            // pokud jsme nasli pouze jeden vyhovujici
        }
    }

    cout << M << " " << H << endl;




}