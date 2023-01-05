//
// Created by kuzdavoj@fel.cvut.cz on 12/18/2022.
//


#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;


struct Cell {
    bool ins;
    bool rep;
    bool del;

    Cell() {}

    Cell(bool ins, bool rep, bool del) : ins(ins), rep(rep), del(del) {}
};



// Function to calculate the minimum of three values
int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}


void ld_dbg(string pattern, string text, vector<vector<int>> dp) {
    // print dlouheho stringu nahore
    cout << "    ";
    for(char c : text) cout << c << " ";
    cout << endl;

    for (int i = 0; i < dp.size(); ++i) {
        if(i != 0) cout << pattern[i - 1] << " ";
        if(i == 0) cout << "  ";
        for (int j = 0; j < dp[0].size(); ++j) {

            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
}

void op_dbg(string pattern, string text, vector<vector<Cell>> op) {
    // print dlouheho stringu nahore
    cout << "\n      ";
    for(char c : text) cout << "[ " <<c  << " ]";
    cout << endl;

    for (int r = 0; r < op.size(); ++r) {
        if(r != 0) cout << pattern[r - 1] << " ";
        if(r == 0) cout << "  ";
        for (int c = 0; c < op[0].size(); ++c) {
            if (op[r][c].ins) cout << "i"; else cout << " ";
            if (op[r][c].del) cout << "d"; else cout << " ";
            if (op[r][c].rep) cout << "r"; else cout << " ";
            cout << "][";
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


vector<pair<int,int>> START_ENDS{};

void traceback_helper(const vector<vector<int>>& dp,
                      const vector<vector<Cell>>& op,
                      const string& pattern,
                      const string& text,
                      pair<int,int> start_end,
                      int row, int col) {

    // base case rekurze
    if(row == 0 || col == 0) {
        start_end.first = col;
        START_ENDS.push_back(start_end);
        return;
    }

    Cell c = op[row][col];
    if(c.rep) traceback_helper(dp, op, pattern, text, start_end, row-1, col-1);
    if(c.ins) traceback_helper(dp, op, pattern, text, start_end, row-1, col);
    if(c.del) traceback_helper(dp, op, pattern, text, start_end, row, col-1);

}


/// kolik existuje podretezcu TEXTu aby LD = k?
// projdeme spodni radek tabulky, zde identifikujeme bunky kde hodnota <= k
void ld_traceback(vector<vector<int>> dp,
                  const vector<vector<Cell>>& op,
                  const string& pattern,
                  const string& text, int k) {
    int row = pattern.length();
    vector<int> last_row = dp[row];

    for (int col = 0; col < last_row.size(); ++col) {
        int v = last_row[col];

        /// pro kazdy prvek v poslednim radku ktery <= k
        if(v <= k) {
            Cell c = op[pattern.length()][col];
            pair<int, int> start_end = {INT32_MAX, col-1};
            if(c.rep) {
                traceback_helper(dp, op, pattern, text, start_end, row-1, col-1);
            }
            if(c.ins) {
                traceback_helper(dp, op, pattern, text, start_end, row-1, col);
            }
            if(c.del) {
                traceback_helper(dp, op, pattern, text, start_end, row, col-1);
            }
        }
    }
}



/// levensteinova vzdálenost optimálního podřetězce končící na této pozici
pair<vector<vector<int>>,vector<vector<Cell>>>  ld_apx_search(string pattern, string text, bool print_table) {
    int p_len = (int) pattern.length();
    int t_len = (int) text.length();

    vector<vector<int>> dp(p_len + 1, vector<int>(t_len + 1, 0));
    vector<vector<Cell>> op(p_len + 1, vector<Cell>(t_len + 1, {false, false, false}));

    /// init prvniho sloupce
    for (int i = 1; i <= p_len; i++) {
        dp[i][0] = i;
        op[i][0] = {true, false, false};
    }

    for (int r = 1; r <= p_len; r++) {
        for (int c = 1; c <= t_len; c++) {
            if (pattern[r - 1] == text[c - 1]) {
                /// chars are same, cost is zero
                dp[r][c] = dp[r - 1][c - 1];
                op[r][c] = {false, true, false};
            }
            else {
                /// chars are different, cost  is + 1
                int i_cost = dp[r - 1][c]; // radek nahoru, stejny sloupec
                int d_cost = dp[r][c - 1];
                int r_cost = dp[r - 1][c - 1];
                // r,r,d = min => delam vsechny operace

                Cell cell{false, false, false};
                int min = minimum(d_cost, i_cost, r_cost);
                if(min == d_cost)
                    cell.del = true;
                if(min == i_cost)
                    cell.ins = true;
                if(min == r_cost)
                    cell.rep = true;

                dp[r][c] = 1 + min;
                op[r][c] = cell;
            }
        }
    }

    if(print_table) {
        ld_dbg(pattern, text, dp);
        op_dbg(pattern, text, op);
    }
//    cout << dp[p_len][t_len]
    return {dp, op};
}




/// vzdalenost prefixu P a prefixu T
int ld_vanilla(string shorter, string longer, bool print_table) {
    int p_len = (int) shorter.length();
    int t_len = (int) longer.length();

    vector<vector<int>> dp(p_len + 1, vector<int>(t_len + 1, 0));

    /// init prvniho sloupce
    for (int i = 1; i <= p_len; i++) {
        dp[i][0] = i;
    }
    /// init prvniho radku
    for (int j = 1; j <= t_len; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= p_len; i++) {
        for (int j = 1; j <= t_len; j++) {
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

    return dp[p_len][t_len];
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

    string text = "coldcolt";
    string pattern = "old";
    auto a = ld_apx_search("old", "coldcolt", true);
    ld_traceback(a.first, a.second, "old", "coldcolt", 1);
    for (auto p : START_ENDS) {
        cout << text.substr(p.first, p.second-p.first+1) << endl;
    }
    cout << "done";



//    traceback_ite(dp, "old", "coldcolt", 2, 2);
//    cout << endl;
//    ld_apx_search("abb", "bbcabab", true);
//    cout << endl;
//    ld_apx_search("abb", "cabab", true);

}













