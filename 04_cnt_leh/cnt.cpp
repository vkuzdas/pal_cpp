//
// Created by kuzdavoj@fel.cvut.cz on 11/5/2022.
//

#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <chrono>

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

size_t prime_size(ulli n) {
    if(n < 100) return 25;
    else {
        auto a = (double)n / log(n);
        return static_cast<size_t>(a + (a * 0.13));
    }
}

void get_root() {

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

vector<uint> sift_under_sqrt(ulli n) {
    // staci nam cisla do sqrt(M_max)
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    uint limit = (uint)sqrt(n);
    vector<bool> A(limit, true);
    for (uint i = 2; i < limit; ++i) {
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
    for (uint i = 2; i < A.size(); ++i) {
        if(A[i]) primes.push_back(i);
    }

    return primes;
}


///    ######################
///    ######-- MAIN --######
///    ######################



int main() {
    ulli M_max; uint D;
    cin >> M_max >> D;

    vector<uint> p_under_D = get_p_under_D(D);
    vector<uint> p_under_sqrt = sift_under_sqrt(M_max);
    ulli count = p_under_sqrt.size();
    uint last_prime = p_under_sqrt[p_under_sqrt.size() - 1];

    /// per candidate M: M-1 |? PF()>D
    for (ulli M = last_prime + 1; M < M_max; ++M) {
        bool is_prime = true;
        bool ok_root = true;
        vector<ulli> PF;
//        vector<uint> more_primes = ( p_under_sqrt.size() > p_under_D.size() ) ? p_under_sqrt : p_under_D;

        /// iteruj do p_under_D/p_under_sqrt - podle toho co je delsi
        for (auto p : p_under_sqrt) { // TODO: kdyz PUR neobsahuje primes do D? nebo nad D?
            ulli R_res = (M-1) % p;
            ulli M_res = M % p;

            /// alespon jedno p <= D musi delit M-1
            if(p > D && PF.empty()) {
                ok_root = false;
                break;
            }

            /// p nesmi delit M aby M byl prime,
            if (M_res == 0) {
                is_prime = false;
                break;
            }

            /// p > D NESMI delit M-1
            if(R_res == 1 && p > D) {
                ok_root = false;
                break;
            }
            else if(R_res == 0) {
                if(p <= D) {
                    PF.push_back(p);
                }
                else if(p > D) {
                    ok_root = false;
                    break;
                }
            }
        }
        if (is_prime && ok_root) {
            count++;
            if (DBG_PRINT) cout << "Prime with root: " << M << endl;
            get_root();
        }
    }
    print_vec(p_under_sqrt, "PUR");
}