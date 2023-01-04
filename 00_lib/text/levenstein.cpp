//
// Created by kuzdavoj@fel.cvut.cz on 12/18/2022.
//


#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <stack>

using namespace std;

// Function to calculate the minimum of three values
int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}


void ld_dbg(string shorter, string longer, vector<vector<int>> dp) {
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

int cost(char c1, char c2) {
    if (c1 == c2) {
        return 0;
    }
    else {
        return 1;
    }
}


void traceback_ite(vector<vector<int>> D, string shorter, string longer, int row, int col) {
    stack<pair<int, int>> st;
    st.push(make_pair(row, col));

    while (!st.empty()) {
        pair<int, int> p = st.top();
        st.pop();

        row = p.first;
        col = p.second;

        int curr_cell = D[row][col];
        int del_cell = D[row][col - 1];   // segfaults
        int ins_cell = D[row - 1][col];   // segfaults
        int rep_cell = D[row - 1][col - 1];   // segfaults

        if (row > 0 && col > 0) {
            if (curr_cell == rep_cell + cost(shorter[row], longer[col])) {
                st.push({row - 1, col - 1});
                printf("%c", longer[min(row-1, col-1)]);
            }
            else if (curr_cell == ins_cell + 1) {
                st.push({row - 1, col});
                printf("%c", longer[min(row-1, col-1)]);
            }
            else if (curr_cell == del_cell + 1) {
                st.push({row, col - 1});
                printf("%c", longer[min(row-1, col-1)]);
            }
        }
    }
}

void traceback_rec(vector<vector<int>> D, string shorter, string longer, int row, int col) {
    if (row > 0 && col > 0) {
        int curr_cell = D[row][col];
        int del_cell = D[row][col - 1];
        int ins_cell = D[row - 1][col];
        int rep_cell = D[row - 1][col - 1];

        if (curr_cell == rep_cell + cost(shorter[row], longer[col])) {
            traceback_rec(D, shorter, longer, row - 1, col - 1);
            printf("%c %c", shorter[row-1], longer[col-1]);
        }
        if (curr_cell == ins_cell + 1) {
            traceback_rec(D, shorter, longer, row - 1, col);
            printf("%c -", shorter[row-1]);
        }
        if (curr_cell == del_cell + 1) {
            traceback_rec(D, shorter, longer, row, col - 1);
            printf("- %c", longer[col-1]);
        }
    }
}


vector<vector<int>> ld_apx_search(string shorter, string longer, bool print_table) {
    int len1 = (int) shorter.length();
    int len2 = (int) longer.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    /// init prvniho sloupce
    for (int i = 1; i <= len1; i++) {
        dp[i][0] = i;
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
        ld_dbg(shorter, longer, dp);
    }
//    cout << dp[len1][len2]
    return dp;
}




/// Function to calculate the Levenshtein distance between two strings
int ld_vanilla(string shorter, string longer, bool print_table) {
    int len1 = (int) shorter.length();
    int len2 = (int) longer.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));

    /// init prvniho sloupce
    for (int i = 1; i <= len1; i++) {
        dp[i][0] = i;
    }
    /// init prvniho radku
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
        ld_dbg(shorter, longer, dp);
    }

    return dp[len1][len2];
}



//int main() {
//    string str1 = "benyam";
//    string str2 = "ephrem";
//
//    cout << "The Levenshtein distance between " << str1 << " and " << str2 << " is: " << ld(str1, str2) << endl;
//
//    return 0;
//}

int main() {

    auto dp = ld_apx_search("abb", "aabbbcabab", true);
    traceback_ite(dp, "abb", "aabbbcabab", 2, 2);
    cout << endl;
    ld_apx_search("abb", "bbcabab", true);
    cout << endl;
    ld_apx_search("abb", "cabab", true);




//    cout << ld_apx_search("ncecj", "gjececcgkmjeccjjchjecfkcje", true);



//    cout << ld_vanilla("ncecj", "gjececcgkmjeccjjchjecfkcje", true) << endl; // M++   nejdelsi
//    cout << ld_vanilla("ncecj", "ccgkmjeccjjchjecfkcje", true) << endl; // M++ nejdelsi
//    cout << ld_vanilla("ncecj", "kmjeccjjchjecfkcje", true) << endl; /// H++
//    cout << ld_vanilla("ncecj", "mjeccjjchjecfkcje", true) << endl; // M++ nejdelsi
//    cout << ld_vanilla("ncecj", "cjchjecfkcje", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "chjecfkcje", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "ecfkcje", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "kcje", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "cj", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "ch", true) << endl; // M++ jedinej moznej
//    cout << ld_vanilla("ncecj", "je", true) << endl; // M++ jedinej moznej

}













