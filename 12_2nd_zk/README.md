# 📝 Exam 2 — Graph Edge Modification

**Exam Problem 2** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given two graphs A and B, find the optimal edge to delete from B and insert into A such that certain graph properties are optimized (e.g., minimize fast/slow node imbalance).

## Approach

1. **Edge Enumeration** — Try all delete-insert combinations
2. **Property Evaluation** — Check resulting graph properties
3. **Multi-criteria Optimization** — Sort by multiple objectives (a, b, c, d)

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| K-subset Generation | Edge selection | $O(\binom{E}{k})$ |
| Graph Property Check | Validation | $O(V + E)$ |

## Files

| File | Description |
|------|-------------|
| `2zk.cpp` | Exam solution |

## Usage

```bash
g++ -O2 -o 2zk 2zk.cpp
./2zk < input.txt
```
