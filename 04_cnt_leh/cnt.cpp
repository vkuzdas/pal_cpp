//
// Created by kuzdavoj@fel.cvut.cz on 11/5/2022.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <set>
#include <algorithm>
#include <array>

using namespace std;
using uint = unsigned int;
using ulli = unsigned long long int;
using namespace std::chrono;



const bool DBG_PRINT = true;


template <typename S>
void print_vec(vector<S> &v, const string &name) {
    if(!DBG_PRINT) return;
    setbuf(stdout, nullptr);
    cout << name << "=[";
    for(auto i : v) {
        cout << i << ", ";
    }
    cout << "]\n";
}
template <typename S>
void print_set(const set<S> &s, const string &name) {
    if(!DBG_PRINT) return;
    setbuf(stdout, nullptr);
    cout << name << "=[";
    for(auto i : s) {
        cout << i << ", ";
    }
    cout << "]\n";
}

/// toto by mozna slo zrychlit pokud bych rovnou spocital i nasobky tohoto cisla ?
set<ulli> get_prime_factors_under_D(const uint D, ulli n) {
    set<ulli> pf;
    ulli prime = 2;
    while(n > 1) {
        if(n % prime == 0) {
            pf.insert(prime);
            n = n / prime;
        }
        else {
            prime = prime + 1;
            if (prime > D) return {};
        }
    }
    return pf;
}


// zaroven musime spocitat ktera cisla z techto obsahuji PF < D
vector<uint> sift_under_sqrt(const ulli n, const uint D, uint& subtract) {
    // staci nam cisla do sqrt(M_max)
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    const uint limit = (uint)sqrt(n);
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

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    if(DBG_PRINT) cout << "sifted in " << elapsed_seconds.count() << "ms\n";
    vector<uint> primes;

    for (uint i = 2; i < A.size(); ++i) { // 2 neni validni generator
        if (A[i]) {
            // i is prime
            primes.push_back(i);
            // valid generator?
            if(i==2)  {
                subtract++;
                continue;
            }
            set<ulli> pf = get_prime_factors_under_D(D, i-1);
            if(pf.empty()) {
                subtract++;
            }
        }
    }

    return primes;
}

// normalne by se muselo nasobit exponent-krat zaklad
// exponentiation by squaring ale dokaze proces zkratit na log(exp)-krat
// pocita base^(exp) % mod v log(exp)

// 3^11 mod 3
    // rslt = 1*3%7 = 3, exp--(10)
    // base = 3*3%7 = 2, exp/2(5)
    // rslt = 3*2%7 = 6, exp--(4)
    // base = 2*2%7 = 4, exp/2(2)
    // base = 4*4%7 = 2, exp/2(1)
    // rslt = 6*2%7 = 5, exp--(0)
ulli power_mod(ulli base, ulli exp, const ulli mod) {
    ulli rslt = 1;
    while (exp > 0) {
        if (exp % 2 == 1) { // exponent je lichy
            ulli tmp = rslt * base % mod;
            rslt = tmp;
            exp = exp - 1;
        }
        else { // exponent je sudy
            base = base * base % mod;
            exp = exp / 2;
        }
    }
    return rslt;
}


uint get_root(const ulli M, const set<ulli>& PF) {
    ulli phi = M-1;
    for (uint r = 2; r <= phi; r++) {
        bool valid_root = false;
        for (ulli p : PF) {
            // validni root neni kongruentni 1 mod M pro vsechna p v PF
            if (power_mod(r, phi / p, M) == 1) {
                valid_root = true;
                break;
            }
        }
        if (!valid_root)
            return r;
    }
}


///    ######################
///    ######-- MAIN --######
///    ######################



int main() {
    ulli M_max; uint D;
    cin >> M_max >> D;

    // nehodici se roots se musi smazat uz z PUR
    uint subtract = 0;
    vector<uint> p_under_sqrt = sift_under_sqrt(M_max, D, subtract);
    ulli count = p_under_sqrt.size() - subtract; // dvojka nema generator
    uint R = 0;
    const uint last_prime = p_under_sqrt[p_under_sqrt.size() - 1];

    /// generujme prime-kandidaty (M) inkrementaci od posledniho prime
    for (ulli M = last_prime + 2; M < M_max; M+=2) {
        bool is_prime = true;

        // zkontrolujeme zda M-1 muze byt vhodny root
        // vhodny root ma cleny v PF mensi nez D
        set<ulli> PF = get_prime_factors_under_D(D, M-1);
        if(PF.empty()) continue; // pokud M-1 deli prime vetsi nez D, vracime empty set

        for (auto p : p_under_sqrt) {
            /// M je prime <=> M%p != 0
            ulli M_res = M % p;
            if (M_res == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if(DBG_PRINT) cout << count <<"-Next prime: " << M;
            R = max(R, get_root(M, PF));
            print_set(PF, "   pf");
            count++;
        }
    }
    print_vec(p_under_sqrt, "PUR");
    cout << count << " " << R << endl;
}


ulli power(uint x, uint exp) {
    if (exp == 0)
        return 1;

    if (exp == 1)
        return x;
    ulli res = power(x, exp / 2);
    //if power_mod is even
    if (exp % 2 == 0)
        return res * res;
    else
        return x * res * res; //if power_mod is odd
}


ulli M_produce(vector<uint>& primes, vector<uint>& exponents) {
    ulli M = 1;
    for (ulli i = 0; i < primes.size(); ++i) {
        uint p = primes[i];
        uint e = exponents[i];
        ulli r = power(p, e);
        M *= r;
    }
    return M;
}


void zero_out_exps(vector<uint>& exponents, uint from) {
    for (uint i = from; i < exponents.size(); ++i) {
        exponents[i] = 0;
    }
}

int __main() {
    vector<uint> primes{11,7,5,3,2};
    ulli cap = 50000000000;

    vector<uint> exponents(primes.size(),0);
    uint incr_index = (uint)exponents.size()-1;
    uint shift;
    uint overflow_on_index;
    while(1) {
        ulli M = M_produce(primes, exponents);
        if(M < cap) {
            exponents[incr_index] += 1;
            overflow_on_index = incr_index;
        }
        if(M >= cap) {
            shift = exponents.size()-overflow_on_index;
            if(incr_index < shift) break;
            zero_out_exps(exponents, overflow_on_index);
            exponents[incr_index-shift]++;
            overflow_on_index = incr_index-shift;
        }
    }
}