# ⛰️ Surveyor Peaks — Longest Path in DAG

**Assignment 11** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given a directed graph of mountain peaks with mandatory checkpoints, find the longest path that visits at least one checkpoint.

## Approach

1. **Find SCCs** — Use Tarjan's algorithm to identify strongly connected components
2. **DAG Condensation** — Collapse SCCs into single nodes
3. **Longest Path** — DP on the resulting DAG with checkpoint constraints

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Tarjan's SCC | Component detection | $O(V + E)$ |
| DAG Longest Path | Optimal route finding | $O(V + E)$ |
| BFS Variant | Path enumeration | $O(V + E)$ |

## Files

| File | Description |
|------|-------------|
| `surv_peaks.cpp` | Main solution with SCC |
| `surv_peaks_BFS.cpp` | BFS-based approach |

## Usage

```bash
g++ -O2 -o surv surv_peaks.cpp
./surv < input.txt
```
