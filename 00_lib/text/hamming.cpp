//
// Created by kuzdavoj@fel.cvut.cz on 1/3/2023.
//


#include <iostream>
#include <string>
using namespace std;


// Hammingova vzdálenost je měřítkem toho, jak jsou si dva řetězce podobné. Je to
// počet pozic, na kterých se znaky v řetězcích liší. Například Hammingova
// vzdálenost mezi řetězci "karolína" a "katerina" je 2, protože se na
// pozicích 3 a 6 liší o jeden znak. Hammingova vzdálenost je vždy mezi
// 0 a délkou řetězce, přičemž nižší hodnota znamená větší podobnost řetězců.

// Levensteinova vzdálenost je také měřítkem podobnosti dvou řetězců, ale
// měří počet editačních operací (přidání, odebrání nebo změna znaku), které
// jsou potřeba k tomu, aby se jeden řetězec přeměnil na druhý. Například Levensteinova
// vzdálenost mezi řetězci "karolína" a "katerina" je 1, protože je potřeba
// pouze jedna operace (změna znaku "o" na "e") k přeměně jednoho řetězce na druhý.


int hamm(string str1, string str2) {
    // Return -1 if the strings are of different lengths
    if (str1.length() != str2.length()) {
        return -1;
    }

    // Initialize the distance to 0
    int distance = 0;

    // Iterate through the characters in the strings and
    // increment the distance if the characters are different
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] != str2[i]) {
            distance++;
        }
    }

    return distance;
}

int main() {
    string str1 = "hello";
    string str2 = "world";
    cout << hamm(str1, str2) << std::endl; // Outputs: -1
    return 0;
}