# 📝 Exam 4 — Hierarchical District MST

**Exam Problem 4** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Partition a graph into districts based on BFS depth from source nodes, then compute MST within and between districts.

## Approach

1. **BFS Distance Assignment** — Assign nodes to districts by depth level
2. **Graph Condensation** — Merge nodes within same district
3. **Prim's MST** — Compute spanning tree on condensed graph

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| BFS | Depth/district assignment | $O(V + E)$ |
| Graph Condensation | District merging | $O(V + E)$ |
| Prim's MST | Minimum spanning tree | $O(E \log V)$ |

## Files

| File | Description |
|------|-------------|
| `4zk.cpp` | Exam solution |

## Usage

```bash
g++ -O2 -o 4zk 4zk.cpp
./4zk < input.txt
```
