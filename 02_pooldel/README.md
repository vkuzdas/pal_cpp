# 🏊 Pool Delivery — SCC Analysis

**Assignment 02** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given a directed graph representing a pool network with one-way connections (streets), find the optimal set of pools to keep open such that:
1. All remaining pools are **strongly connected** (reachable from each other)
2. Minimize deletion cost while maximizing the number of retained pools

A "weak street" leads outside the strongly connected component and must be removed.

## Approach

1. **Find SCCs** — Use Tarjan's algorithm to identify strongly connected components
2. **Delete Weak Connections** — Mark nodes with outgoing edges to other SCCs
3. **Build Distance Matrix** — BFS within each SCC to compute pairwise distances
4. **Evaluate Components** — Compare SCCs by size (variance) and deletion cost

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Tarjan's SCC | Find strongly connected components | $O(V + E)$ |
| BFS | Distance matrix within SCCs | $O(V \cdot (V + E))$ |

## Files

| File | Description |
|------|-------------|
| `pooldel.cpp` | Main solution |
| `tarjan.cpp` | Standalone Tarjan's implementation |
| `postup.txt` | Solution approach notes |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o pooldel pooldel.cpp
./pooldel < datapub/pub01.in
```
