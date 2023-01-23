#include <iostream>
#include <vector>
#include<iostream>
#include<cmath>
#include <regex>
#include <algorithm>

using namespace std;


int F, Mmax, Xo, x, COUNT;


vector<int> sieve(int mmax){
    vector<bool> p(mmax, true);
    vector<int> res;
    p[0] = false;
    p[1] = false;
    for (int prime = 2; prime < p.size(); prime++) {
        if (p[prime]) {
            res.push_back(prime);
            for (int i = prime * prime; i < p.size(); i += prime) {
                p[i] = false;
            }
        }
    }
    return res;
}

vector<int> sift_under_sqrt(const int Mmax) {
    // staci nam cisla do sqrt(M_max)

    const int limit = (int)sqrt(Mmax);
    vector<bool> A(limit+1, true);
    for (int i = 2; i <= limit; ++i) {
        if(A[i]) {
            int i_incr = 0;
            int j = i*i + i*i_incr;
            while (j <= limit) {
                A[j] = false;
                i_incr++;
                j = i*i + i*i_incr;
            }
        }
    }

    vector<int> primes;

    for (int i = 2; i < A.size(); ++i) { // 2 neni validni generator
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

unsigned long modulo_multiplication(const unsigned long& a, const unsigned long& b, const unsigned long& m) {
    unsigned long a_hi = a >> 24, a_lo = a & ((1 << 24) - 1);
    unsigned long b_hi = b >> 24, b_lo = b & ((1 << 24) - 1);
    unsigned long result = ((((a_hi*b_hi << 16) % m) << 16) % m) << 16;
    result += ((a_lo*b_hi+a_hi*b_lo) << 24) + a_lo*b_lo;
    return result % m;
}




void find_M(const int& size, const vector<int>& combs, const long long& combs_product, const long long& acc, int& count){
    for (int i = size; i < combs.size(); ++i) {
        if (acc * combs[i] <= Mmax){
            const long long& M = acc * combs[i];
            const long long& A = (M % 4 == 0) ? combs_product*2 + 1 : combs_product + 1;
            if (A < M){
                for (long long c = 1; c < M; ++c) {
                    if (__gcd(c, M) == 1 && (modulo_multiplication(A,Xo,M)+c)%M == x/*(A*Xo+c)%M==x*/ ){
                        count += 1;
                    }
                }
            };
            find_M(i, combs, combs_product, M, count);
        }else{
            break;
        }
    }
}


void find_combs(const int& index, const int& size, vector<int>& combs, const long long& acc, const int& f, const vector<int>& prms, int& count){
    if (size == f){
        find_M(0, combs,acc, acc, count);
    }else{
        for (int i = index; i < prms.size(); ++i) {
            combs[size] = prms[i];
            if (acc*prms[i] <= Mmax){
                find_combs(i + 1, size + 1, combs,acc*prms[i], f, prms, count);
            }else{
                break;
            }
        }
    }

}




int main() {
    cin >> F >> Mmax >> Xo >> x;
    int count = 0;
    vector<int> PRIMES = sieve(Mmax);
    vector<int> combs(F);
    find_combs(0, 0, combs,1, F, PRIMES, count);
    cout << count << endl;
}
