#include <iostream>
#include <vector>
#include<iostream>
#include<cmath>
#include <regex>
#include <algorithm>

using namespace std;

using namespace std;
using uint = unsigned int;
using ll = long long;
using lli = long long int;

using cul = const unsigned long;
using cll = const long long;
using ci = const int;





int F, Mmax, x0, x;
vector<int> PRIMES;
int COUNT = 0;



vector<int> sieve(int mmax){
    vector<int> res;
    vector<bool> primes_erat(mmax, true);
    primes_erat[0] = false;
    primes_erat[1] = false;
    for (int prime = 2; prime < primes_erat.size(); prime++) {
        if (!primes_erat[prime]) continue;
        res.push_back(prime);
        for (int i = prime * prime; i < primes_erat.size(); i += prime) {
            primes_erat[i] = false;
        }
    }
    return res;
}




cll get_A(cll& combs_product, cll& M) {
    if (M % 4 != 0) {
        return combs_product + 1;
    } else {
        return combs_product * 2 + 1;
    }
}

void search_modulo(ci& size, const vector<int>& combinations, cll& combs_product,ci c_prev, cll& pfp){
    for (int i = size; i < combinations.size(); ++i) {
        if (pfp * combinations[i] > Mmax) {
            break;
        } else {
            cll &M = pfp * combinations[i];
            cll &A = get_A(combs_product, M);
            auto C_prev = c_prev;
            if (A >= M) {}
            else {
                if (__gcd(A, M) != 1) {}
                else {
                    long long c = (x - A * x0) % M;
                    c = c < 0 ? c + M : c;
                    if (__gcd(c, M) != 1 || (A * x0 + c) % M != x) {}
                    else {
                        COUNT += 1;
                        C_prev = c;
                    }
                }
            }
            search_modulo(i, combinations, combs_product, C_prev, M);
        }
    }
}


void search_the_combinations(ci& index, ci& size, vector<int>& combs, cll& acc, ci& f){
    if (size != f) {
        for (int i = index; i < PRIMES.size(); ++i) {
            combs[size] = PRIMES[i];
            if (acc * PRIMES[i] > Mmax) {
                break;
            } else {
                search_the_combinations(i + 1, size + 1, combs, acc * PRIMES[i], f);
            }
        }
    } else {
        search_modulo(0, combs, acc, 0, acc);
    }

}




int main() {
    cin >> F >> Mmax >> x0 >> x;
    PRIMES = sieve(Mmax);
    vector<int> combs(F);
    search_the_combinations(0, 0, combs,1, F);
    cout << COUNT << endl;
}
