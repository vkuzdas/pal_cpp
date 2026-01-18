# 🕵️ Smuggler — Connected Subgraph Selection

**Assignment 07** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given an undirected graph, find a connected subset of `K` nodes that optimizes a given objective (e.g., maximize value, minimize risk).

## Approach

1. **K-subset Generation** — Enumerate all $\binom{n}{k}$ subsets of size K
2. **Connectivity Check** — BFS to verify induced subgraph is connected
3. **Optimization** — Track best valid subset by objective function

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Combinatorial Generation | K-subset enumeration | $O(\binom{n}{k})$ |
| BFS | Connectivity verification | $O(V + E)$ per subset |

## Files

| File | Description |
|------|-------------|
| `smug.cpp` | Main solution |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o smug smug.cpp
./smug < datapub/pub01.in
```
