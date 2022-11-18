//
// Created by kuzdavoj@fel.cvut.cz on 11/5/2022.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <set>

using namespace std;
using uint = unsigned int;
using ulli = unsigned long long int;
using namespace std::chrono;



const bool DBG_PRINT = false;


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
void print_set(set<S> &s, const string &name) {
    if(!DBG_PRINT) return;
    setbuf(stdout, nullptr);
    cout << name << "=[";
    for(auto i : s) {
        cout << i << ", ";
    }
    cout << "]\n";
}

/// toto by mozna slo zrychlit pokud bych rovnou spocital i nasobky tohoto cisla ?
set<ulli> get_prime_factors_under_D(uint D, ulli n) {
    set<ulli> pf;
    ulli prime = 2;
    while(n > 1) {
        if(n % prime == 0) {
            pf.insert(prime);
            n = n / prime;
        }
        else {
            prime = prime + 1;
            if (prime > D) {
                return {};
            }
        }
    }
    return pf;
}

/// D je z rozsahu <3,40>
vector<uint> get_p_under_D(uint D) {
    vector<uint> ps_under_DMAX{2,3,5,7,11,13,17,19,23,29,31,37};
    if(D>=37) return ps_under_DMAX;
    for (uint i = 0; i < ps_under_DMAX.size(); ++i) {
        if (D < ps_under_DMAX[i])
            return vector<uint>(ps_under_DMAX.begin(),ps_under_DMAX.begin()+i);
    }
    return {};
}

// zaroven musime spocitat ktera cisla z techto obsahuji PF < D
vector<uint> sift_under_sqrt(ulli n, uint D, uint& subtract) {
    // staci nam cisla do sqrt(M_max)
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    uint limit = (uint)sqrt(n);
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
ulli power(ulli base, ulli exp, ulli mod) {
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


ulli get_root(ulli M, set<ulli>& PF) {
    ulli phi = M-1;
    for (ulli r = 2; r <= phi; r++) {
        bool valid_root = false;
        for (ulli p : PF) {
            // validni root neni kongruentni 1 mod M pro vsechna p v PF
            if (power(r, phi / p, M) == 1) {
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
    ulli R = 0;
    uint last_prime = p_under_sqrt[p_under_sqrt.size() - 1];

    /// generujme prime-kandidaty (M) inkrementaci od posledniho prime
    for (ulli M = last_prime + 1; M < M_max; ++M) {
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