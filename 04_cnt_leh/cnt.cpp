//
// Created by kuzdavoj@fel.cvut.cz on 11/5/2022.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

using namespace std;
using uint = unsigned int;
using ulli = unsigned long long int;

const bool DBG_PRINT = true;


template <typename S>
void print_vec(vector<S> &v, const string &name) {
    if(!DBG_PRINT) return;
    cout << name << "=[";
    for(auto i : v) {
        cout << i << ", ";
    }
    cout << "]\n";
}

vector<ulli> factor_decomposition(ulli composite, ulli max_factor) {
    vector<ulli> factors;
    for (ulli d = 2; d * d <= composite; d++) {
        while (composite % d == 0) {
            if(d > max_factor) return {};
            factors.push_back(d);
            composite = composite / d;
        }
    }
    if (composite > 1) {
        if(composite > max_factor) return {};
        factors.push_back(composite);
    }
    return factors;
}


vector<ulli> sieve(ulli p_up_to) {
    ulli size_estimate = (ulli)1.25506 * p_up_to;
    vector<ulli> isprime(size_estimate , true);
    vector<ulli> prime;
    vector<ulli> SPF(size_estimate);
    isprime[0] = isprime[1] = false ;

    for (ulli i = 2; i < p_up_to ; i++) {
        if (isprime[i]) {
            prime.push_back(i);
            SPF[i] = i;
        }

        for (ulli j = 0;
             j < (ulli)prime.size() &&
             i*prime[j] < p_up_to && prime[j] <= SPF[i];
             j++) {
            isprime[i*prime[j]] = false;
            SPF[i*prime[j]] = prime[j] ;
        }
    }
    return prime;
}



vector<ulli> factorize_phi(ulli num) {
    vector<ulli> factors;
    for (ulli i = 2; i*i <= num; ++i) {
        ulli remainder = num % i;
        if (remainder == 0) {
            factors.push_back (i);
            while (num % i == 0) {
                num = num / i;
            }
        }
    }

    if (num > 1) {
        factors.push_back (num);
    }
    return factors;
}

ulli power_modulus(ulli a, ulli phi_fac, ulli prime) {
    ulli rslt = 1;
    while (phi_fac) {
        if (phi_fac & 1) {
            ulli r = rslt * a % prime;
            rslt = r;
            phi_fac = phi_fac - 1;
        }
        else {
            a = a * a % prime;
            phi_fac = phi_fac >> 1;
        }
    }
    return rslt;
}

ulli get_g(ulli prime) {
    ulli phi = prime - 1;
    ulli num = phi;

    vector<ulli> factors = factorize_phi(num);

    string s("factors");
    print_vec(factors, s);

    for (ulli g = 2; g <= prime; ++g) {
        bool ok = true;
        for (size_t i = 0; i < factors.size(); ++i) {
            if(!ok) continue;
            ok &= power_modulus(g, phi / factors[i], prime) != 1;
        }
        if (ok) {
            return g;
        }
    }
    return 0;
}


///    ######################
///    ######-- MAIN --######
///    ######################


int main() {
    ulli M_max;    // rozloz na soucin prvocisel
    ulli D;         // faktory mohou byt max D
    cin >> M_max >> D;
    // vygeneruj vsechny primes do M_max (to jsou M)
    //      M= 1 + 2, M = 1 + 2*2, M = 1 + 2*3 ...

    vector<ulli> primes = sieve(M_max);
    vector<vector<ulli>> p_f(primes.size()); // bez 2
    vector<ulli> roots(primes.size());

    ulli L = 0;
    ulli R = 0;

    for (ulli i = 0; i < primes.size(); ++i) {
        ulli prime = primes[i];

        p_f[i].push_back(1); // M = 1+prime[1]*...*prime[n]
        /// zadny faktor nesmi byt vetsi nez D
        vector<ulli> fs = factor_decomposition(prime - 1, D);
        p_f[i].insert(p_f[i].end(), fs.begin(), fs.end());

        if(fs.empty()) {
//            roots[i] = get_g(prime);
            // bez incrementu, pokud je fs prazdne, existuje faktor vetsi nez D
        } else {
            ulli g = get_g(prime);
            roots[i] = g;
            R = max(R, g);
            L++;
        }
    }
    cout << L << " " << R;
    return 0;
}

// pub07 is slow
// pub08 is slow
// pub09 is slow
// pub10 crashes is slow

