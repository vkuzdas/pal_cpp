# ⛓️ Chain Cutting — Edit Distance on Grid

**Assignment 06** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given a 2D character grid, find the minimum edit distance to transform one string pattern to another using character operations (insert, delete, replace).

## Approach

1. **Dynamic Programming** — Classic Levenshtein distance with 2D DP table
2. **Grid Navigation** — Apply edit distance concepts to matrix traversal
3. **Backtracking** — Reconstruct the optimal sequence of operations

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Levenshtein Distance | Edit distance computation | $O(mn)$ |
| DP Backtracking | Operation reconstruction | $O(m + n)$ |

## Files

| File | Description |
|------|-------------|
| `chain.cpp` | Main solution with DP |

## Usage

```bash
g++ -O2 -o chain chain.cpp
./chain < input.txt
```
