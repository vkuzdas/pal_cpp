//
// Created by kuzdavoj@fel.cvut.cz on 12/18/2022.
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
int levenshteinDistance(string word1, string word2) {
    int len1 = (int) word1.length();
    int len2 = (int) word2.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    for (int i = 1; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (int j = 1; j <= len2; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (word1[i - 1] == word2[j - 1]) {
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
    // Return  final cost
    return dp[len1][len2];
}



int main() {
    string str1 = "benyam";
    string str2 = "ephrem";

    cout << "The Levenshtein distance between " << str1 << " and " << str2 << " is: " << levenshteinDistance(str1, str2) << endl;

    return 0;
}