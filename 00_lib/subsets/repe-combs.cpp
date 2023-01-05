//
// Created by kuzdavoj@fel.cvut.cz on 1/5/2023.
//


#include <iostream>
#include <vector>

// Funkce pro generování kombinací s opakováním
void combinations_with_repetition(std::vector<int> &combination, int index, int n, int k) {
    if (index == k) {
        // Vytiskněte kombinaci
        for (int i = 0; i < k; i++) {
            std::cout << combination[i] << " ";
        }
        std::cout << std::endl;
    } else {
        // Pro každé možné hodnoty z rozsahu [0, n)
        for (int i = 0; i < n; i++) {
            // Přidejte hodnotu do kombinace
            combination[index] = i;
            // Rekurzivně zavolejte funkci pro další index
            combinations_with_repetition(combination, index + 1, n, k);
        }
    }
}

int main() {
    int n, k;
    std::cin >> n >> k;

    // Vektor pro ukládání kombinace
    std::vector<int> combination(k);

    // Zavolejte funkci pro generování kombinací
    combinations_with_repetition(combination, 0, n, k);

    return 0;
}