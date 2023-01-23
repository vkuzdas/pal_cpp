#include <iostream>
#include <vector>
#include<iostream>
#include<cmath>
#include <regex>
#include <algorithm>

using namespace std;
using uint = unsigned int;
using ll = long long;
using lli = long long int;


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

bool am_lesser(lli A, const lli M) {
    return A < M;
}

bool prod_prod_lesser(const lli &acc, const vector<uint> &vector, uint& mmax, uint& i) {
    return acc * vector[i] <= Mmax;
}

void find_M(const uint& size, const vector<uint>& combs, const ll& pf_prod, const int c_prev, const ll& acc){
    for (uint i = size; i < combs.size(); ++i) {

        if (prod_prod_lesser(acc, combs, Mmax, i)){

            ll M = acc * combs[i];
            ll A = pf_prod + 1;
            if(M % 4 == 0) A = pf_prod * 2 + 1;

            int C_prev = c_prev;
            if (am_lesser(A, M)){
                bool AM_coprime = __gcd(A, M) == 1;
                if (!AM_coprime) continue;
                ll c = (x - A*Xo) % M;
                c = c < 0 ? c + M : c;
                bool cm_coprime = __gcd(c, M) == 1;
                bool eq_x = (A*Xo+c)%M == x;
                if (cm_coprime  && eq_x) {
                    COUNT += 1;
                    C_prev = c;
                }
            }
            find_M(i, combs, pf_prod, C_prev, M);
        }else{
            break;
        }
    }
}


void find_combs(int& size, vector<uint>& combs, int& index, ll& pf_prod, const uint& f){
    if (size == f){
        find_M(0, combs, pf_prod, 0, pf_prod);
    }else{
        for (int i = index; i < PRIMES.size(); ++i) {
            combs[size] = PRIMES[i];
            if (pf_prod * PRIMES[i] <= Mmax){
                int idx = i+1;
                int sz = size + 1;
                ll pp = PRIMES[i] * pf_prod;
                find_combs(sz,  combs, idx, pp, f);
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
//    int& size, vector<uint>& combs, int& index, ll& pf_prod, const uint& f
    ll pf_prod = 1;
    find_combs(first_size, combs, frist_index,  pf_prod, F);
    cout << COUNT << endl;
}
