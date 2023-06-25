#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using uint = unsigned int;
using ll = long long;
using lli = long long int;


uint F, Mmax, x0, x1;
uint COUNT = 0;
vector<uint> PRIMES;

uint FIRST_INDEX = 0;
uint FIRST_SIZE = 0;
ll PF_PROD = 1;


vector<uint> sieve(uint mmax);
bool am_lesser(lli& A, lli& M);
bool prod_prod_lesser(const lli &acc, const vector<uint> &vector, uint& mmax, uint& i);
void get_M(const uint& size, const vector<uint>& combs, const ll& pf_prod, const int c_prev, const ll& pfp);
void search_combinations(uint& size, vector<uint>& combinations, uint& index, ll& pf_prod, const uint& f);





int main() {



    cin >> F >> Mmax >> x0 >> x1;
    PRIMES = sieve(Mmax);

    vector<uint> combs(F);
    search_combinations(FIRST_SIZE, combs, FIRST_INDEX, PF_PROD, F);
    cout << COUNT << endl;
}


bool am_lesser(lli& A, lli& M) {
    return A < M;
}

bool prod_prod_lesser(const lli &acc, const vector<uint> &vector, uint& mmax, uint& i) {
    return acc * vector[i] <= Mmax;
}

void get_M(const uint& size, const vector<uint>& combs, const ll& pf_prod, const uint& c_prev, const ll& pfp){
    for (uint i = size; i < combs.size(); ++i) {

        if (!prod_prod_lesser(pfp, combs, Mmax, i)) {
            break;
        } else {

            ll M = pfp * combs[i];
            ll A = pf_prod + 1;
            if (M % 4 == 0) A = pf_prod * 2 + 1;

            uint C_prev = c_prev;
            if (am_lesser(A, M)) {
                bool AM_coprime = (1 == __gcd(A, M));
                if (!AM_coprime) continue;
                ll c = (x1 - A * x0) % M;
                c = c < 0 ? c + M : c;
                bool cm_coprime = (1 == __gcd(c, M));
                bool eq_x = (A * x0 + c) % M == x1;
                if (cm_coprime && eq_x) {
                    COUNT += 1;
                    C_prev = c;
                }
            }
            get_M(i, combs, pf_prod, C_prev, M);
        }
    }
}


void search_combinations(uint& size, vector<uint>& combinations, uint& index, ll& pf_prod, const uint& f){

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
                search_combinations(sz, combinations, idx, pp, f);
            }
        }
    }

        /// pokud MAME delku stejnou jako prvocisla
    else if (size == f){
        get_M(FIRST_SIZE, combinations, pf_prod, FIRST_INDEX, pf_prod);
    }

}


vector<int> first_primes = {2,3,5,7,11,13,17,19,23,29,31,37};;
vector<uint> sieve(int mmax, uint lim){
    // staci nam cisla do sqrt(M_max)


    for (int i = 0; i < lim; i++) {
        mmax = mmax / first_primes[i];
    }


    const uint limit = mmax;
    vector<bool> A(limit+1, true);
    for (uint i = 2; i <= limit; ++i) {
        if(A[i]) {
            uint i_incr = 0;
            uint j = i*i + i*i_incr;
            while (j <= limit) {
                A[j] = false;
                i_incr++;
                j = i*i + i*i_incr;
            }
        }
    }

    vector<uint> primes;

    for (uint i = 2; i < A.size(); ++i) { // 2 neni validni generator
        if (A[i]) {
            // i is prime
            primes.push_back(i);
            // valid generator?
            if(i==2)  {
                continue;
            }
        }
    }

    return primes;
}