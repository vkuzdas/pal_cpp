# 🌐 Network — Subgraph Isomorphism

**Assignment 03** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given an old network (unweighted graph) and a new network (weighted graph with "fast" server nodes), find the optimal subset of nodes in the new network such that:
1. The induced subgraph is **isomorphic** to the old network
2. Maximize the number of **fast servers** in the subset
3. Minimize the total **edge weight** (network delay)

## Approach

1. **Generate k-subsets** — Enumerate all $\binom{n}{k}$ subsets of size `k` (old network size)
2. **Check Connectivity** — Verify the induced subgraph is connected (BFS)
3. **Degree Invariant** — Compare sorted degree sequences as isomorphism filter
4. **Optimize Selection** — From valid subsets, choose max fast servers, then min delay

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Subset Generation | Enumerate k-combinations | $O(\binom{n}{k})$ |
| BFS | Connectivity check | $O(V + E)$ |
| Degree Sequence | Isomorphism invariant | $O(V \log V)$ |

## Files

| File | Description |
|------|-------------|
| `netw.cpp` | Main solution |
| `gen_perm.cpp` | Permutation generator utility |
| `invariants.txt` | Notes on graph invariants |
| `postup.txt` | Solution approach notes |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o netw netw.cpp
./netw < datapub/pub01.in
```
