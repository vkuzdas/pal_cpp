#include <iostream>
#include <vector>
#include<iostream>
#include<cmath>
#include <regex>
#include <algorithm>

using namespace std;


int F, Mmax, Xo, x, G;
vector<int> primes;


void eratosthenes(int mmax){
    vector<bool> primes_erat(mmax, true);
    primes_erat[0] = false;
    primes_erat[1] = false;
    for (int prime = 2; prime < primes_erat.size(); prime++) {
        if (primes_erat[prime]) {
            primes.push_back(prime);
            for (int i = prime * prime; i < primes_erat.size(); i += prime) {
                primes_erat[i] = false;
            }
        }
    }
}

unsigned long modulo_multiplication(const unsigned long& a, const unsigned long& b, const unsigned long& m) {
    unsigned long a_hi = a >> 24, a_lo = a & ((1 << 24) - 1);
    unsigned long b_hi = b >> 24, b_lo = b & ((1 << 24) - 1);
    unsigned long result = ((((a_hi*b_hi << 16) % m) << 16) % m) << 16;
    result += ((a_lo*b_hi+a_hi*b_lo) << 24) + a_lo*b_lo;
    return result % m;
}




void find_M(const int& size, const vector<int>& combs, const long long& combs_product,const int c_prev, const long long& acc){
    for (int i = size; i < combs.size(); ++i) {
        if (acc * combs[i] <= Mmax){
            const long long& M = acc * combs[i];
            const long long& A = (M % 4 == 0) ? combs_product*2 + 1 : combs_product + 1;
            auto C_prev = c_prev;
            if (A < M){
                if (__gcd(A,M)== 1) {
                    //Oba kody zakomenteny fungujou
//                    long long c = (x - (long long) modulo_multiplication(A, Xo, M)) % M;
//                    c = c < 0 ? c + M : c;
//                    if (__gcd(c, M) == 1 && (modulo_multiplication(A,Xo,M)+c)%M == x) {
//                        G += 1;
//                        C_prev = c;
//                    }
                    long long c = (x - A*Xo) % M;
                    c = c < 0 ? c + M : c;
                    if (__gcd(c, M) == 1 && (A*Xo+c)%M == x) {
                        G += 1;
                        C_prev = c;
                    }
                }/*else{
                    for (long long c = 1; c < M; ++c) {
                        if (__gcd(c, M) == 1 && (modulo_multiplication(A,Xo,M)+c)%M == x*//*(A*Xo+c)%M==x*//* ){
                            G += 1;
                            C_prev +=1;
                        }
                    }
                }*/

            };
            find_M(i, combs, combs_product, C_prev, M);
        }else{
            break;
        }
    }
}


void find_combs(const int& index, const int& size, vector<int>& combs, const long long& acc, const int& f, const vector<int>& prms){
    if (size == f){
        find_M(0, combs,acc,0, acc);
    }else{
        for (int i = index; i < prms.size(); ++i) {
            combs[size] = prms[i];
            if (acc*prms[i] <= Mmax){
                find_combs(i + 1, size + 1, combs,acc*prms[i], f, prms);
            }else{
                break;
            }
        }
    }

}




int main() {
    cin >> F >> Mmax >> Xo >> x;
    G = 0;
    eratosthenes(Mmax);
    vector<int> combs(F);
    find_combs(0, 0, combs,1, F, primes);
    cout << G << endl;
}
