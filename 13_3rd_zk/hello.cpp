#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using uint = unsigned int;
using ll = long long;
using lli = long long int;


uint F, Mmax, Xo, x;
uint COUNT = 0;
vector<uint> PRIMES;

uint FIRST_INDEX = 0;
uint FIRST_SIZE = 0;


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


void find_combs(uint& size, vector<uint>& combinations, uint& index, ll& pf_prod, const uint& f){

    /// pokud NEMAME delku stejnou jako prvocisla
    if (size != f){
        for (uint i = index; i < PRIMES.size(); ++i) {
            combinations[size] = PRIMES[i];
            if (pf_prod * PRIMES[i] > Mmax) {
                break;
            } else if (pf_prod * PRIMES[i] <= Mmax) {
                uint idx = i + 1;
                uint sz = size + 1;
                ll pp = PRIMES[i] * pf_prod;
                find_combs(sz, combinations, idx, pp, f);
            }
        }
    }

        /// pokud MAME delku stejnou jako prvocisla
    else if (size == f){
        find_M(FIRST_SIZE, combinations, pf_prod, FIRST_INDEX, pf_prod);
    }

}




int main() {



    cin >> F >> Mmax >> Xo >> x;
    PRIMES = sieve(Mmax);

    vector<uint> combs(F);
//    int& size, vector<uint>& combs, int& index, ll& pf_prod, const uint& f
    ll pf_prod = 1;
    find_combs(FIRST_SIZE, combs, FIRST_INDEX,  pf_prod, F);
    cout << COUNT << endl;
}
