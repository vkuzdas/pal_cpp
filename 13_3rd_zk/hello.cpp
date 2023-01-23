#include <iostream>
#include <vector>
#include<iostream>
#include<cmath>
#include <regex>
#include <algorithm>

using namespace std;
using uint = unsigned int;


uint F, Mmax, Xo, x;
uint COUNT = 0;
vector<uint> PRIMES;


vector<uint> sieve(uint mmax){
    vector<uint> result;
    vector<bool> p_bool(mmax, true);
    p_bool[0] = false;
    p_bool[1] = false;
    for (int p = 2; p < p_bool.size(); p++) {
        if (p_bool[p]) {
            result.push_back(p);
            for (int i = p*p; i < p_bool.size(); i = i + p) {
                p_bool[i] = false;
            }
        }
    }
    return result;
}

unsigned long modulo_multiplication(const unsigned long& a, const unsigned long& b, const unsigned long& m) {
    unsigned long a_hi = a >> 24, a_lo = a & ((1 << 24) - 1);
    unsigned long b_hi = b >> 24, b_lo = b & ((1 << 24) - 1);
    unsigned long result = ((((a_hi*b_hi << 16) % m) << 16) % m) << 16;
    result += ((a_lo*b_hi+a_hi*b_lo) << 24) + a_lo*b_lo;
    return result % m;
}



void find_M(const int& size, const vector<uint>& combs, const long long& pf_prod, const int c_prev, const long long& acc){
    for (int i = size; i < combs.size(); ++i) {
        if (acc * combs[i] <= Mmax){
            const long long& M = acc * combs[i];
            long long A = pf_prod + 1;
            if(M % 4 == 0) A = pf_prod * 2 + 1;
            auto C_prev = c_prev;
            if (A < M){
                if (__gcd(A,M)== 1) {

                    long long c = (x - A*Xo) % M;
                    c = c < 0 ? c + M : c;
                    if (__gcd(c, M) == 1 && (A*Xo+c)%M == x) {
                        COUNT += 1;
                        C_prev = c;
                    }
                }
            }
            find_M(i, combs, pf_prod, C_prev, M);
        }else{
            break;
        }
    }
}


void find_combs(const int& index, const int& size, vector<uint>& combs, const long long& pf_prod, const uint& f){
    if (size == f){
        find_M(0, combs, pf_prod, 0, pf_prod);
    }else{
        for (int i = index; i < PRIMES.size(); ++i) {
            combs[size] = PRIMES[i];
            if (pf_prod * PRIMES[i] <= Mmax){
                find_combs(i + 1, size + 1, combs, pf_prod * PRIMES[i], f);
            }else{
                break;
            }
        }
    }

}




int main() {
    int frist_index = 0;
    int first_size = 0;


    cin >> F >> Mmax >> Xo >> x;
    PRIMES = sieve(Mmax);

    vector<uint> combs(F);
    find_combs(frist_index, first_size, combs,1, F);
    cout << COUNT << endl;
}
