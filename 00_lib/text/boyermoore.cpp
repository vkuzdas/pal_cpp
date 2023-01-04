//
// Created by kuzdavoj@fel.cvut.cz on 12/18/2022.
//

#include <vector>
#include <iostream>
#include <string>

using namespace std;


// Function to find the occurrence of pattern in text using Boyer-Moore algorithm
void boyerMoore(string text, string pattern) {
    int n = (int) text.length();
    int m = (int) pattern.length();

    // create an array of size 256 (ASCII upper bound) to store the bad character skip distances
    int badchar[256];

    // initialize all elements of badchar[] as -1
    for (int & el : badchar)
        el = -1;

    // fill the actual value of last occurrence of a character
    for (int i = 0; i < m; i++)
        badchar[(int) pattern[i]] = i;


    int shift = 0; // shift is shift of the pattern with respect to text
    while (shift <= (n - m)) {
        int j = m - 1;

        // Keep reducing index j of pattern while characters of pattern and text are matching at this shift shift
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;

        // If the pattern is present at current shift, then index j will become -1 after the above loop
        if (j < 0) {
            std::cout << "pattern occurs at shift = " << shift << std::endl;

            // Shift the pattern so that the next character in text aligns with the last occurrence of it in pattern.
            // The condition shift+m < n is necessary for the case when pattern occurs at the end of text
            shift += (shift + m < n) ? m - badchar[text[shift + m]] : 1;
        }
        else {
            // Shift the pattern so that the bad character in text aligns with the last occurrence of it in pattern
            shift += std::max(1, j - badchar[text[shift + j]]);
        }
    }
}

int main() {
//    string text = "ABAAABCDBBABCDDEBCABC";
//    string pattern = "ABC";
    string text = "abdabaacdacaabdabaacdaca";
    string pattern = "aa";

    boyerMoore(text, pattern);
    // pattern occurs at shift = 4
    // pattern occurs at shift = 10
    // pattern occurs at shift = 18

    return 0;
}
