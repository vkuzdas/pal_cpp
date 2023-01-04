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

vector<int> find_next_cuts(string P, string T, int D, string &cut_string) {
    vector<int> row = last_row_levenDist(P, T, true);
    vector<int> found_idxs = {};

    for (int i = 0; i < row.size(); ++i) {
        // arr je doprava stale stoupajici -> staci najit posledni row[i] <= D
        if (row[i] <= D) {
            found_idxs.push_back(i);
        }

        else if (row[i] > D && !found_idxs.empty()) {
            // ex: [2,1,1,2,..], D=1 -> podmince vyhovuje jen 1,1
            //   v tu chvili found_idxs neni empty
            //   dal hledat teda uz nemusime
            //   zaroven vime, ze minule i byl konec stringu ktery nas zajima
            //   proto naplnime cut_string
            cut_string = T.substr(0, i-1);
            break;
        }
    }

    return found_idxs;
}

/// input:   aabb       output: nejlepsi M&H
///            11
pair<int,int> cut_decide(const vector<int>& cuts, string &cut_string) {
    vector<pair<int, int>> mh_pairs = {};

    // ve vectoru i stringu pojedeme od zadu naraz
    // to zajisti shodu charu a intu
    int cut_from_end = 0;
    for (int each : cuts) {

        // TODO: co kdyz new_string ma delku 1???
        string new_string = cut_string.substr(0,cut_string.length() - cut_from_end);

        if(new_string.length() == cut_string.length()) {
            mh_pairs.push_back({1,0});
        }
        else if(new_string.length() - cut_string.length() == 1) {
            // cutnuli jsme string a na konci zbyl jeden char
            mh_pairs.push_back({1, 1});
        }
        else if(new_string.length() - cut_string.length() > 1) {
            // vyriznuli jsme string a jeste jeden zbyl
            mh_pairs.push_back({2, 0});
        }


        cut_from_end++;
    }
    pair<int, int>

}

int main() {
    string T_orig;
    string P;
    int D;
    cin >> T_orig;
    cin >> P;
    cin >> D;

    int s_cut = 0;
    int c_del = 0;

    int pos = 0;
    while(true) {

        string T_sub = T_orig.substr(pos, P.length()+D);
        string cut_string = "";
        vector<int> cuts = find_next_cuts(P, T_orig, D, cut_string);

        if(cuts.empty()) {
            c_del++;
        } else if (cuts.size() == 1) {
            s_cut++;
        } else {
            pair<int, int> mh = cut_decide(cuts, cut_string);
            s_cut += mh.first;
            c_del += mh.second;
        }

    }


}