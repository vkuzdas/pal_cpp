# ✏️ Editor — Fuzzy String Matching

**Assignment 10** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Find the best approximate match for a pattern in a dictionary of words, minimizing edit distance. Used for spell-checking and autocomplete functionality.

## Approach

1. **Levenshtein Distance** — Compute edit distance between pattern and each word
2. **DP Optimization** — Use only last row of DP table for memory efficiency
3. **Ranking** — Sort matches by (distance, lexicographic order)

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Levenshtein Distance | Edit distance | $O(mn)$ per word |
| Space-Optimized DP | Memory reduction | $O(m)$ space |

## Files

| File | Description |
|------|-------------|
| `editor.cpp` | Main solution |
| `editor-dp.cpp` | Full DP table variant |

## Usage

```bash
g++ -O2 -o editor editor.cpp
./editor < input.txt
```
