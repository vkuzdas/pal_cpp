//
// Created by kuzdavoj@fel.cvut.cz on 1/21/2023.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>

using namespace std;

bool DBG_PRINT = true;

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

bool next_combination(vector<int>& a, int n) {
    int k = (int)a.size();
    for (int i = k - 1; i >= 0; i--) {
        if (a[i] < n - k + i + 1) {
            a[i]++;
            for (int j = i + 1; j < k; j++)
                a[j] = a[j - 1] + 1;
            return true;
        }
    }
    return false;
}

vector<int> pick_primes(const vector<int>& p_indices, const vector<int>& primes, int& F) {
    vector<int> picked_primes(F);
    for (int i = 0; i < p_indices.size(); ++i) {
        int index = p_indices[i];
        picked_primes[i] = primes[index];
    }
    return picked_primes;
}

int power(int base, int exp) {
    if (exp == 0)
        return 1;

    if (exp == 1)
        return base;
    int res = power(base, exp / 2);
    // exp is even
    if (exp % 2 == 0)
        return res * res;
    else
        return base * res * res; // exp is odd
}

int M1_produce(vector<int >& primes, vector<int >& exponents) {
    int M = 1;
    for (int i = 0; i < primes.size(); ++i) {
        int p = primes[i];
        int e = exponents[i];
        int r = power(p, e);
        M *= r;
    }
    return M;
}

void zero_out_exps(vector<int>& exponents, int from) {
    for (int i = from; i < exponents.size(); ++i) {
        if(i == 0) exponents[i] = 1;
        else exponents[i] = 0;
    }
}

bool pars_valid(int &a, int &m, int &x0, int &x1) {

    return false;
}

int main() {
    int F, Mmax, x0, x1;
    int count = 0;
    cin >> F >> Mmax >> x0 >> x1;
    // eratosten do sqrt Mmax

    vector<int> primes = sift_under_sqrt(Mmax);
//    print_vec(primes, "PRIMES");
    vector<int> p_indices(F);
    for (int i = 0; i < F; ++i) {
        p_indices[i]=i;
    }
//    std::iota(p_indices.begin(), p_indices.end(), 0);

    do {
        vector<int> picked_primes = pick_primes(p_indices, primes, F);
        print_vec(picked_primes, "PICKED_primes");

        vector<int> primes = picked_primes;
        int cap = Mmax;

        vector<int> exponents(primes.size(),0);
        exponents[exponents.size()-1] = 1;
        int incr_index = (int)exponents.size()-1;
        int shift;
        int overflow_on_index; // nastane moment kdy dalsi mocnina cisla nedava smysl, v inkrementu se pak musime posunout dal
        while(true) {
            int M = M1_produce(primes, exponents);
            if(M <= cap) {
                cout << "   M=" << M << " ";
                print_vec(exponents, "exp");
                print_vec(primes, "primes");

                /// nyni jsem spocital M
                /// ted z primes zkusime stejnym zpusobem generovat A
            //TODO/////////////////////////////////
//                int cap2 = M;
//                if(M==120) {
//                    cout << "hello";
//                }
//                vector<int> exp2(primes.size(),0);
//                exp2[exp2.size()-1] = 1;
//                int incr_index2 = (int)exp2.size()-1;
//                int shift2;
//                int overflow_on_index2; // nastane moment kdy dalsi mocnina cisla nedava smysl, v inkrementu se pak musime posunout dal
//                while(true) {
//                    int A = M1_produce(primes, exp2);
//                    if(A < cap2) {
//                        cout << "       A=" << A << " ";
//                        print_vec(exp2, "exp");
//                        /// nyni jsem spocital A
//                        /// ted z musime konstantne zkontrolovat C a generator
//
//                        if(pars_valid(A,M,x0,x1)) {
//                            count++;
//                        }
//
//                        exp2[incr_index2] += 1;
//                        overflow_on_index2 = incr_index2;
//                    }
//                    if(A >= cap2) {
//                        shift2 = int(exp2.size())-overflow_on_index2;
//                        if(incr_index2 < shift2) break;
//                        zero_out_exps(exp2, overflow_on_index2);
//                        exp2[incr_index2-shift2]++;
//                        overflow_on_index2 = incr_index2-shift2;
//                    }
//                }
        //TODO//////////////////////////////////


                exponents[incr_index] += 1;
                overflow_on_index = incr_index;
            }
            if(M > cap) {
                shift = int(exponents.size())-overflow_on_index;
                if(incr_index < shift) break;
                zero_out_exps(exponents, overflow_on_index);
                exponents[incr_index-shift]++;
                overflow_on_index = incr_index-shift;
            }
        }


    } while (next_combination(p_indices, F));

    cout << count;
}


// nejak najit Mka jako kombinace s opakovanim z primes
// ale to neivm jak jeste
// pak hledas minimalni A jako nasobek vsech primes M + 1
// a pak hledas C ktery bude coprime s M

// primes najdes takhle: 1) eratosten do sqrt(Mmax) 2) F-subset z toho co vyhodil eratosten




//   ja to zkusim udelat opacne
//    a jak vis z jakych exponentu delat ty kombinace?
//    ze si zjistim maxpower u kazdyho prvocisla
//    a pak udelam F kombinace bez opakovani
//   _

