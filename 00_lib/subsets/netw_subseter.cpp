//
// Created by kuzdavoj@fel.cvut.cz on 1/5/2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;



void print_sub(vector<int> pick) {
    for (auto i: pick) {
        cout << " " << i;
    }
    cout << endl;
}


class Subseter {
public:
    Subseter()=default;
    vector<vector<int>> subsets;


    void k_subsets3a(const vector<int>& set, vector<int>& res,
                     const int start, const int end,
                     const int depth, const int& k) {
        if(depth >= k) {
            subsets.push_back(res);
            return;
        }
        for (int i = start; i <= end && end - i + 1 >= k - depth; i++) {
            res[depth] = set[i];
            k_subsets3a(set, res, i + 1, end, depth + 1, k);
        }
    }

    void subset(const vector<int>& set, const int& k){
        subsets.clear();
        vector<int> results(k);
        k_subsets3a(set, results, 0, (int)set.size() - 1, 0, k);
    }

};

int main() {

    Subseter f = Subseter();
    Subseter s = Subseter();
    f.subset({2,4,6,8,9}, 5);
    s.subset({0,1,3,5,7}, 0);

    cout << "fast pick: " << endl;
    for (const auto& fast_subsets : f.subsets) {
        print_sub(fast_subsets);
    }
    cout << "slow pick: " << endl;
    for (const auto& slow_subsets : s.subsets) {
        print_sub(slow_subsets);
    }
    cout << "#1 done----------------" << endl << endl;



    f.subset({2,4,6,8,9}, 4);
    s.subset({0,1,3,5,7}, 1);

    cout << "fast pick: " << endl;
    for (const auto& fast_subsets : f.subsets) {
        print_sub(fast_subsets);
    }
    cout << "slow pick: " << endl;
    for (const auto& slow_subsets : s.subsets) {
        print_sub(slow_subsets);
    }
    cout << "#2 done-----------" << endl << endl;



    f.subset({2,4,6,8,9}, 3);
    s.subset({0,1,3,5,7}, 2);

    cout << "fast pick: " << endl;
    for (const auto& fast_subsets : f.subsets) {
        print_sub(fast_subsets);
    }
    cout << "slow pick: " << endl;
    for (const auto& slow_subsets : s.subsets) {
        print_sub(slow_subsets);
    }
    cout << "#3 done-----------" << endl << endl;

}









