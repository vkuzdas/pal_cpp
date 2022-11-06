
// C++ program to Find the kth Permutation
// Sequence of first n natural numbers

#include <bits/stdc++.h>
using namespace std;

// Function to find the index of number
// at first position of
// kth sequence of set of size n
int findFirstNumIndex(int& k, int n)
{
    if (n == 1)
        return 0;
    n--;
    int first_num_index;
    // n_actual_fact = n!
    int n_partial_fact = n;
    while (k >= n_partial_fact
           && n > 1) {
        n_partial_fact
                = n_partial_fact
                  * (n - 1);
        n--;
    }
    // First position of the
    // kth sequence will be
    // occupied by the number present
    // at index = k / (n-1)!
    first_num_index = k / n_partial_fact;
    k = k % n_partial_fact;
    return first_num_index;
}

// Function to find the
// kth permutation of n numbers
vector<int> findKthPermutation(int n, int k, set<int> s)
{
    // Store final answer
    vector<int> ans2;
    set<int>::iterator itr;
    // Mark the first position
    itr = s.begin();
    // subtract 1 to get 0 based indexing
    k = k - 1;
    for (int i = 0; i < n; i++) {
        int index = findFirstNumIndex(k, n - i);
        advance(itr, index);
        // itr now points to the
        // number at index in set s
        ans2.push_back((*itr));
        // remove current number from the set
        s.erase(itr);
        itr = s.begin();
    }
    return ans2;
}

// Driver code
int main() {
    int n = 10, k = 600;
    set<int> s {8,9,10,11,12,13};
    vector<int> a = findKthPermutation((int)s.size(), k, s);
    cout << "a";

    return 0;
}