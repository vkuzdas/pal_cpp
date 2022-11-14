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
    cout << "sifted in " << elapsed_seconds.count() << "ms\n";
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
    ulli M_max, D;
    cin >> M_max >> D;



    vector<uint> primes = sift_under_sqrt(M_max);
//    print_vec(primes, "primes");
    ulli count = primes.size();


    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    uint last_prime = primes[primes.size()-1];
    for (ulli candidate = last_prime+1; candidate < M_max; ++candidate) {
        /// per candidate


        bool is_prime = true;
        for (auto p : primes) {
            // ozkousej zda kandidat je delitelny prvocislama z primes under sqrt
            // je-li kandidat delitelny, neni prvocislo
            if(candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) count++;


    }

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "candidates in " << elapsed_seconds.count() << "ms \n";
}