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
//    word2 = " "+word2;
//    cout <<endl <<endl <<"  "+ word1 << endl;
//    for (int i = 0; i < dp.size(); ++i) {
//        cout << word2[i];
//        for (int j = 0; j < dp[0].size(); ++j) {
//            cout << dp[i][j];
//        }
//        cout << endl;
//    }

    cout << endl << word1 << endl;
    cout << word2 << " c = ";

    return dp[len1][len2];
}



//int main() {
//    string str1 = "benyam";
//    string str2 = "ephrem";
//
//    cout << "The Levenshtein distance between " << str1 << " and " << str2 << " is: " << levenshteinDistance(str1, str2) << endl;
//
//    return 0;
//}

int main() {
    string s = "abdabaacdacaabdabaacdaca";
    cout << levenshteinDistance(s.substr(0,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(1,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(2,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(3,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(4,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(5,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(6,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(7,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(8,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(9,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(10,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(11,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(12,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(13,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(14,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(15,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(16,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(17,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(18,5), "bcadb") << endl;
    cout << levenshteinDistance(s.substr(19,5), "bcadb") << endl;

}