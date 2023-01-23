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

using cul = const unsigned long;
using cll = const long long;
using ci = const int;





uint F, Mmax, x0, x;
vector<uint> PRIMES;
int COUNT = 0;
int FIRST_SIZE = 0;
int FIRST_INDEX = 0;

vector<uint> sieve(int mmax, uint lim);
cll get_A(cll& combs_product, cll& M);
void search_modulo( const vector<int>& combinations, cll& combs_product,ci c_prev, cll& pfp, ci& size);
void search_the_combinations(ci& index, ci& size, vector<int>& combs, cll& acc, ci& f);


//vector<int> primesfirst8 = {2, 3, 5, 7, 11, 13, 17, 19};
//    int primeMax = Mmax;
//    for (int t = 0; t < F-1; t++) {
//        primeMax = primeMax / primesfirst8[t];
//    }

int main() {
    cin >> F >> Mmax >> x0 >> x;
    PRIMES = sieve(Mmax, F-1);
    vector<int> combs(F);
    search_the_combinations(FIRST_INDEX, FIRST_SIZE, combs,1, F);
    cout << COUNT << endl;
}

vector<int> first_primes = {2,3,5,7,11,13,17,19,23,29,31,37};;


// hele na zacatek
//kdyz mas eratosthena
//tak ho pak generujes jen do toho primeMax
//namisto do Mmax
//takze mezi input read a  to generovani cisel to dej

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




cll get_A(cll& combs_product, cll& M) {
    if (M % 4 != 0) {
        return combs_product + 1;
    } else {
        return combs_product * 2 + 1;
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
        search_modulo(combs, acc, FIRST_INDEX, acc, FIRST_SIZE);
    }

}



void search_modulo( const vector<int>& combinations, cll& combs_product,ci c_prev, cll& pfp, ci& size){
    for (int i = size; i < combinations.size(); ++i) {
        if (pfp * combinations[i] > Mmax) {
            break;
        } else {
            cll &M =  combinations[i] * pfp;
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
            search_modulo(combinations, combs_product, C_prev, M, i);
        }
    }
}