# 🔤 LSWord — NFA Shortest Word

**Assignment 05** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given a Non-deterministic Finite Automaton (NFA), find the **lexicographically smallest** word that is accepted by the automaton.

## Approach

1. **Parse NFA** — Read states, transitions (alphabet a-e), and final states
2. **BFS Search** — Explore state space level by level to find shortest accepting path
3. **Lexicographic Order** — At each step, try transitions in alphabetical order (a, b, c, d, e)

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| BFS | Shortest path to accepting state | $O(S \cdot A^L)$ |
| NFA Simulation | State transitions | $O(S)$ per step |

Where S = states, A = alphabet size, L = word length.

## Files

| File | Description |
|------|-------------|
| `lsword.cpp` | Main solution |
| `single_bfs.cpp` | Simplified BFS variant |
| `triple_search.cpp` | Alternative search approach |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o lsword lsword.cpp
./lsword < datapub/pub01.in
```
