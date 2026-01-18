# 🌪️ Calamity — District MST

**Assignment 01** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given a graph of `T` towns with `R` weighted roads, partition the towns into `D` districts and compute the minimum cost to connect all towns such that:
1. Each district forms a connected subgraph (intra-district MST)
2. All districts are connected to each other (inter-district MST)

## Approach

1. **District Assignment** — BFS from node `D` to assign each town to its nearest district
2. **Intra-district MST** — Run Prim's algorithm within each district
3. **Graph Condensation** — Merge all nodes in each district into a single super-node
4. **Inter-district MST** — Run Prim's algorithm on the condensed graph

## Complexity

| Phase | Time |
|-------|------|
| BFS assignment | $O(V + E)$ |
| D × Prim's MST | $O(D \cdot E \log V)$ |
| Condensation | $O(V + E)$ |
| Final MST | $O(E \log D)$ |

## Files

| File | Description |
|------|-------------|
| `calamity.cpp` | Main solution |
| `prim_heap.cpp` | Standalone Prim's implementation |
| `fast_dist_assign.cpp` | Optimized district assignment |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o calamity calamity.cpp
./calamity < datapub/pub01.in
```
