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

int main_chatgpt() {
    int n, k;
    std::cin >> n >> k;

    // Vektor pro ukládání kombinace
    std::vector<int> combination(k);

    // Zavolejte funkci pro generování kombinací
    combinations_with_repetition(combination, 0, n, k);

    return 0;
}






void print_vector(const std::vector<int> &pos,
                  const std::vector<std::string> &str) {
    for (size_t i = 1; i < pos.size(); ++i) // offset: i:1..N
        printf("%s\t", str[pos[i]].c_str());  // str:      0..N-1
    printf("\n");
}

// idea: custom number system with 2s complement like 0b10...0==MIN stop case
void combination_with_repetiton(int n, int k,
                                const std::vector<std::string> &str) {
    std::vector<int> pos(k + 1, 0);
    while (true) {
        for (int i = k; i > 0; i -= 1) {
            if (pos[i] > n - 1) // if number spilled over: xx0(n-1)xx
            {
                pos[i - 1] += 1; // set xx1(n-1)xx
                for (int j = i; j <= k; j += 1)
                    pos[j] = pos[j - 1]; // set xx11..1
            }
        }
        if (pos[0] > 0) // stop condition: 1xxxx
            break;
        print_vector(pos, str);
        pos[k] += 1; // xxxxN -> xxxxN+1
    }
}

int main_rosetta() {
    std::vector<std::string> str{"iced", "jam", "plain"};
    combination_with_repetiton(3, 2, str);
    return 0;
}