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





int F, Mmax, Xo, x, G;
vector<int> PRIMES;



vector<int> eratosthenes(int mmax){
    vector<int> res;
    vector<bool> primes_erat(mmax, true);
    primes_erat[0] = false;
    primes_erat[1] = false;
    for (int prime = 2; prime < primes_erat.size(); prime++) {
        if (primes_erat[prime]) {
            res.push_back(prime);
            for (int i = prime * prime; i < primes_erat.size(); i += prime) {
                primes_erat[i] = false;
            }
        }
    }
    return res;
}

unsigned long modulo_multiplication(cul& a, cul& b, cul& m) {
    unsigned long a_hi = a >> 24, a_lo = a & ((1 << 24) - 1);
    unsigned long b_hi = b >> 24, b_lo = b & ((1 << 24) - 1);
    unsigned long result = ((((a_hi*b_hi << 16) % m) << 16) % m) << 16;
    result += ((a_lo*b_hi+a_hi*b_lo) << 24) + a_lo*b_lo;
    return result % m;
}




void find_M(ci& size, const vector<int>& combs, cll& combs_product,ci c_prev, cll& acc){
    for (int i = size; i < combs.size(); ++i) {
        if (acc * combs[i] <= Mmax){
            cll& M = acc * combs[i];
            cll& A = (M % 4 == 0) ? combs_product*2 + 1 : combs_product + 1;
            auto C_prev = c_prev;
            if (A < M){
                if (__gcd(A,M)== 1) {
                    long long c = (x - A*Xo) % M;
                    c = c < 0 ? c + M : c;
                    if (__gcd(c, M) == 1 && (A*Xo+c)%M == x) {
                        G += 1;
                        C_prev = c;
                    }
                }
            }
            find_M(i, combs, combs_product, C_prev, M);
        }else{
            break;
        }
    }
}


void find_combs(ci& index, ci& size, vector<int>& combs, cll& acc, ci& f){
    if (size == f){
        find_M(0, combs,acc,0, acc);
    }else{
        for (int i = index; i < PRIMES.size(); ++i) {
            combs[size] = PRIMES[i];
            if (acc*PRIMES[i] <= Mmax){
                find_combs(i + 1, size + 1, combs,acc*PRIMES[i], f);
            }else{
                break;
            }
        }
    }

}




int main() {
    cin >> F >> Mmax >> Xo >> x;
    G = 0;
    PRIMES = eratosthenes(Mmax);
    vector<int> combs(F);
    find_combs(0, 0, combs,1, F);
    cout << G << endl;
}
