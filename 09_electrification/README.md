# ⚡ Electrification — Constrained MST

**Assignment 09** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Build a minimum cost spanning tree connecting villages, with the constraint that certain edges (river crossings) must be included or excluded based on selection criteria.

## Approach

1. **Forced Edge Selection** — Pre-select required river crossing edges
2. **Kruskal's MST** — Build MST with Union-Find, incorporating forced edges
3. **Subset Enumeration** — Try different river edge combinations to find optimal

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Kruskal's MST | Minimum spanning tree | $O(E \log E)$ |
| Union-Find | Cycle detection | $O(\alpha(V))$ amortized |
| Subset Generation | River edge combinations | $O(2^R)$ |

## Files

| File | Description |
|------|-------------|
| `river_kruskal.cpp` | Kruskal with forced edges |
| `electri_ham.cpp` | Hamiltonian-based approach |

## Usage

```bash
g++ -O2 -o electri river_kruskal.cpp
./electri < input.txt
```
