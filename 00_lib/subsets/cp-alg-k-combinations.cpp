//
// Created by kuzdavoj@fel.cvut.cz on 12/21/2022.
//


#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>


using namespace std;


void print_choice(const vector<int>& pick) {
    for (auto i: pick)
        cout << " " << i;
    cout << endl;
}


// Generate next lexicographical K-combination
// First we will generate them in lexicographical order. The algorithm for this is simple.
// The first combination will be {1, 2, ..., K}. Now let's see how to find the combination that
// immediately follows this, lexicographically. To do so, we consider our current combination, and
// find the rightmost element that has not yet reached its highest possible value. Once finding this
// element, we increment it by 1, and assign the lowest valid value to all subsequent elements.

bool next_combination(vector<int>& a, int n) {
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

///  #####################
///  ###  MAIN_NEXT_C  ###
///  #####################

int main_nc() {
    /// umi od nuly
    vector<int> a = {0,1,2,3};
    print_choice(a);
    while(next_combination(a, 6)) {
        print_choice(a);
    }
    return 0;
}












// This time we want to generate all K-combinations in such an order, that
// adjacent combinations differ exactly by one element.
// The following is a naive implementation working by generating
// all 2^n possible subsets, and finding subsets of size K.

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
                    cout << j + 1 << " ";
            }
            cout << "\n";
        }
    }
}

///  ####################
///  ###  MAIN_ALL_C  ###
///  ####################

int main_ac() {
    /// neumi od nuly
    vector<int> a = {99,99,99,99};
    all_combinations(6, 4);
    return 0;
}















// More efficient implementation exists that only resorts to building
// valid combinations and thus works in O(N*comb_num(N,K))
// however it is recursive in nature and for smaller values of N it probably
// has a larger constant than the previous solution.

vector<int> ans;

void gen(int n, int k, int idx, bool rev) {
    if (k > n || k < 0)
        return;

    if (!n) {
        for (int i = 0; i < idx; ++i) {
            if (ans[i])
                cout << i + 1;
        }
        cout << "\n";
        return;
    }

    ans[idx] = rev;
    gen(n - 1, k - rev, idx + 1, false);
    ans[idx] = !rev;
    gen(n - 1, k - !rev, idx + 1, true);
}

void all_combinations_rec(int n, int k) {
    ans.resize(n);
    gen(n, k, 0, false);
}

///  ######################
///  ###  MAIN_ALL_REC  ###
///  ######################

int main_acr() {
    /// neumi od nuly
    ans = {99,99,99,99};
    all_combinations(6, 4);
    return 0;
}