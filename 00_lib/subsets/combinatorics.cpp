//
// Created by kuzdavoj@fel.cvut.cz on 12/20/2022.
//
#include <iostream>
#include <string>
#include <vector>

using namespace std;

///  ######################
///  ## KOMBINACNI CISLO ##
///  ######################

// uses: http://www.geeksforgeeks.org/dynamic-programming-set-9-binomial-coefficient/
int binomial_coef(int n, int k) {
    int C[n+1][k+1];
    int i, j;

    // Caculate value of Binomial Coefficient in bottom up manner
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= min(i, k); j++) {
            // Base Cases
            if (j == 0 || j == i) {
                C[i][j] = 1;
            }
                // Calculate value using previosly stored values
            else {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }
    }

    return C[n][k];
}


///  ####################################
///  ## PARTIAL PERMUTATIONS (variace) ##
///  ####################################

/// VARIACE bez opakovani  (trojciferne cislo z {1,2,3,4,5} prvku)
/// V(3,5) = 5*4*3 = 60
// src: https://stackoverflow.com/questions/34866921/partial-permutations
void partial_permutations(const string& sofar, string rest, int n) {
//    cout << "p_p('" << sofar << "', '" << rest << "', " << n << ")\n";
    string substring;
    if (n == 0)
        cout << sofar << '\n';
    else {
        for (size_t i = 0; i < rest.length(); i++) {
            substring = rest.substr(0, i) + rest.substr(i + 1, rest.length());
            partial_permutations(sofar + rest[i], substring, n - 1);
        }
    }
}

int _main() {
    partial_permutations("", "12345", 3);
//    123
//    124
//    125
//    132
//    ...
}








///  ###############
///  ## K-SUBSETS ##
///  ###############


// src: https://cp-algorithms.com/combinatorics/generating_combinations.html#generate-all-k-combinations-such-that-adjacent-combinations-differ-by-one-element
// src: C:\Users\vojte\Documents\01_FEL\1.sem\01_PAL\dl_sources\cpalgo-Kcombinations.html


// Given the natural numbers N and K,
// and considering a set of numbers from 1 to N.
// The task is to derive all subsets of size K.
bool next_K_subset(vector<int>& a, int n) {
//    First, generate in lex order.
//    First combination is {1, 2, ..., K}
//    Combination that immediately follows:
//      consider our current combination, and find the rightmost
//      element that has not yet reached its highest possible value.
//    Once found increment it 1, and assign the lowest valid value to all subsequent elements.
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

int _main1() {
    vector<int> a = {1,2};
    do {
        for (auto i: a) {
            cout << " " << i;
        }
        cout << endl;
    } while (next_K_subset(a, 6));

//    1 2 4
//    1 2 5
//    1 3 4
//    1 3 5
//    ...
    return 0;
}


///  ##########################
///  ## K-SUBSETS- Gray code ##
///  ##########################


vector<int> permutation = {0,1,2,3};
vector<int> chosen = {0,1,2,3};
int n = 5;


void search() {
    if (permutation.size() == n) {
// process permutation
    } else {
        for (int i = 0; i < n; i++) {
            if (chosen[i]) continue;
            chosen[i] = true;
            permutation.push_back(i);
            search();
            chosen[i] = false;
            permutation.pop_back();
        }
    }
}







int gray_code (int n) {
    return n ^ (n >> 1);
}

int count_bits (int n) {
    int res = 0;
    for (; n; n >>= 1)
        res += n & 1;
    return res;
}

void all_combinations (int n, int k) {
    for (int i = 0; i < (1 << n); i++) {
        int cur = gray_code (i);
        if (count_bits(cur) == k) {
            for (int j = 0; j < n; j++) {
                if (cur & (1 << j))
                    cout << j + 1;
            }
            cout << "\n";
        }
    }
}

void print_choice(vector<int> pick) {
    for (auto i: pick) {
        cout << " " << i;
    }
}

int main() {
//    all_combinations(8,4);
    search();
    print_choice(chosen);
    search();
    print_choice(chosen);

}


