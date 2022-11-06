//
// Created by kuzdavoj@fel.cvut.cz on 11/6/2022.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <bits/stdc++.h>
#include <cassert>

using namespace std;
using uint = unsigned int;

using ulli = unsigned long long int;

const bool DBG_PRINT = true;
using namespace std;



template <typename S>
void print_vec(vector<S> &v, string &name) {
    if(!DBG_PRINT) return;
    cout << name << "=[";
    for(auto i : v) {
        cout << i << ", ";
    }
    cout << "]\n";
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



// This functions finds all primes smaller than 'limit'
// using simple sieve of eratosthenes. It also stores
// found primes in vector prime[]
void simpleSieve(ulli limit, vector<ulli> &prime)
{
    // Create a boolean array "mark[0..n-1]" and initialize
    // all entries of it as true. A value in mark[p] will
    // finally be false if 'p' is Not a prime, else true.
    vector<bool> mark(limit + 1, true);

    for (ulli p=2; p*p<limit; p++)
    {
        // If p is not changed, then it is a prime
        if (mark[p] == true)
        {
            // Update all multiples of p
            for (ulli i=p*p; i<limit; i+=p)
                mark[i] = false;
        }
    }

    // Print all prime numbers and store them in prime
    for (ulli p=2; p<limit; p++)
    {
        if (mark[p] == true)
        {
            prime.push_back(p);
            cout << p << " ";
        }
    }
}

// Prints all prime numbers smaller than 'n'
void segmentedSieve(ulli n)
{
    // Compute all primes smaller than or equal
    // to square root of n using simple sieve
    ulli limit = floor(sqrt(n))+1;
    vector<ulli> prime;
    prime.reserve(limit);
    simpleSieve(limit, prime);

    // Divide the range [0..n-1] in different segments
    // We have chosen segment size as sqrt(n).
    ulli low = limit;
    ulli high = 2*limit;

    // While all segments of range [0..n-1] are not processed,
    // process one segment at a time
    while (low < n)
    {
        if (high >= n)
            high = n;

        // To mark primes in current range. A value in mark[i]
        // will finally be false if 'i-low' is Not a prime,
        // else true.
        bool mark[limit+1];
        memset(mark, true, sizeof(mark));

        // Use the found primes by simpleSieve() to find
        // primes in current range
        for (ulli i = 0; i < prime.size(); i++)
        {
            // Find the minimum number in [low..high] that is
            // a multiple of prime[i] (divisible by prime[i])
            // For example, if low is 31 and prime[i] is 3,
            // we start with 33.
            ulli loLim = floor(low/prime[i]) * prime[i];
            if (loLim < low)
                loLim += prime[i];

            /* Mark multiples of prime[i] in [low..high]:
                We are marking j - low for j, i.e. each number
                in range [low, high] is mapped to [0, high-low]
                so if range is [50, 100] marking 50 corresponds
                to marking 0, marking 51 corresponds to 1 and
                so on. In this way we need to allocate space only
                for range */
            for (ulli j=loLim; j<high; j+=prime[i])
                mark[j-low] = false;
        }

        // Numbers which are not marked as false are prime
        for (ulli i = low; i<high; i++)
            if (mark[i - low] == true)
                cout << i << " ";

        // Update low and high for next segment
        low = low + limit;
        high = high + limit;
    }
}

// Driver program to test above function
int main()
{
    ulli n = 6000000;
    cout << "Primes smaller than " << n << ":n";
    segmentedSieve(n);
    return 0;
}


