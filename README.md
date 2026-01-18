# 🧮 PAL Algorithms

C++ implementations of algorithms and data structures for the [B4M33PAL](https://cw.fel.cvut.cz/wiki/courses/b4m33pal/start) (Algorithms) course at CTU Prague.

## Overview

This repository contains solutions to programming assignments and a reusable algorithm library covering graph theory, string processing, combinatorics, and optimization problems.

## Project Structure

```
├── 00_lib/                    # Reusable algorithm library
│   ├── MST.cpp                # Prim & Kruskal
│   ├── connectivity.cpp       # Graph connectivity
│   ├── euler-path.cpp         # Eulerian path finding
│   ├── longest-dag-path.cpp   # Longest path in DAG
│   ├── shortest-path-dag.cpp  # DAG shortest paths
│   ├── skiplist.cpp           # Skip list implementation
│   ├── topsort.cpp            # Topological sorting
│   ├── scc/                   # Strongly Connected Components
│   │   ├── tarjan.cpp         # Tarjan's algorithm
│   │   └── kosaraju.cpp       # Kosaraju's algorithm
│   ├── text/                  # String algorithms
│   │   ├── trie.cpp           # Trie data structure
│   │   ├── levenstein.cpp     # Levenshtein distance
│   │   ├── hamming.cpp        # Hamming distance
│   │   └── boyermoore.cpp     # Boyer-Moore search
│   ├── traversals/            # Graph traversals
│   │   ├── bfs-paths-parents.cpp
│   │   └── dfs-time-color.cpp
│   ├── subsets/               # Combinatorics
│   └── isomo/                 # Graph isomorphism
│
├── 01_calamity/               # Assignment: Disaster response (District MST)
├── 02_pooldel/                # Assignment: Pool deletion (SCC-based)
├── 03_netw/                   # Assignment: Network optimization (Subgraph isomorphism)
├── 04_cnt_leh/                # Assignment: Prime counting (Lehmer method)
├── 05_lsword/                 # Assignment: NFA shortest word (BFS)
├── 06_chain_cutt/             # Assignment: Grid edit distance (Levenshtein DP)
├── 07_smug/                   # Assignment: Connected subgraph selection
├── 08_incomp_auto/            # Assignment: DFA completion
├── 09_electrification/        # Assignment: Constrained MST (Kruskal)
├── 10_editor/                 # Assignment: Fuzzy string matching
├── 11_surv_peaks/             # Assignment: Longest path in DAG
├── 12_2nd_zk/                 # Exam: Graph edge modification
├── 13_3rd_zk/                 # Exam: Prime generator selection
├── 14_4th_zk/                 # Exam: Hierarchical district MST
└── CMakeLists.txt             # Build configuration
```

## Assignments Overview

| # | Name | Problem | Key Algorithms |
|---|------|---------|----------------|
| 01 | Calamity | Partition towns into districts, minimize MST cost | [BFS](https://en.wikipedia.org/wiki/Breadth-first_search), [Prim's MST](https://en.wikipedia.org/wiki/Prim%27s_algorithm), Graph Condensation |
| 02 | Pool Deletion | Find optimal SCC to keep in directed graph | [Tarjan's SCC](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm), [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) |
| 03 | Network | Find isomorphic subgraph with constraints | [K-subset](https://en.wikipedia.org/wiki/Combination), [Degree sequence](https://en.wikipedia.org/wiki/Degree_(graph_theory)) |
| 04 | Count Lehmer | Count integers with prime factors ≥ D | [Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes), [Prime factorization](https://en.wikipedia.org/wiki/Integer_factorization) |
| 05 | LSWord | Find lexicographically smallest word accepted by NFA | [BFS](https://en.wikipedia.org/wiki/Breadth-first_search), [NFA](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton) |
| 06 | Chain Cutting | Edit distance on character grid | [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) |
| 07 | Smuggler | Select K connected nodes optimizing objective | [K-subset](https://en.wikipedia.org/wiki/Combination), [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) |
| 08 | Incomplete Auto | Complete DFA to accept/reject examples | [DFA](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) |
| 09 | Electrification | MST with forced river crossing edges | [Kruskal's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm), [Union-Find](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) |
| 10 | Editor | Approximate string matching in dictionary | [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) |
| 11 | Surveyor Peaks | Longest path through checkpoints | [Tarjan's SCC](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm), [DAG longest path](https://en.wikipedia.org/wiki/Longest_path_problem) |
| 12 | Exam 2 | Optimal edge insert/delete between graphs | [Subset generation](https://en.wikipedia.org/wiki/Power_set) |
| 13 | Exam 3 | Select prime generators for values | [Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes), [Combinations](https://en.wikipedia.org/wiki/Combination) |
| 14 | Exam 4 | Hierarchical district partitioning | [BFS](https://en.wikipedia.org/wiki/Breadth-first_search), [Prim's MST](https://en.wikipedia.org/wiki/Prim%27s_algorithm) |

## Algorithm Library

### Graph Algorithms

| Algorithm | File | Complexity |
|-----------|------|------------|
| Prim's MST | `MST.cpp` | $O(E \log V)$ |
| Kruskal's MST | `MST.cpp` | $O(E \log E)$ |
| Tarjan's SCC | `scc/tarjan.cpp` | $O(V + E)$ |
| Kosaraju's SCC | `scc/kosaraju.cpp` | $O(V + E)$ |
| Topological Sort | `topsort.cpp` | $O(V + E)$ |
| DAG Shortest Path | `shortest-path-dag.cpp` | $O(V + E)$ |
| Eulerian Path | `euler-path.cpp` | $O(E)$ |

### String Algorithms

| Algorithm | File | Complexity |
|-----------|------|------------|
| Trie | `text/trie.cpp` | $O(m)$ per operation |
| Levenshtein Distance | `text/levenstein.cpp` | $O(mn)$ |
| Boyer-Moore | `text/boyermoore.cpp` | $O(n/m)$ avg |
| Hamming Distance | `text/hamming.cpp` | $O(n)$ |

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++17 |
| Build | CMake |

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Running Assignments

Each assignment folder contains:
- `*.cpp` — Solution source code
- `datapub/` — Public test cases (`*.in` files)
- `postup.txt` — Solution approach notes (Czech)

```bash
# Compile and run with test input
g++ -O2 -o solution 02_pooldel/pooldel.cpp
./solution < 02_pooldel/datapub/pub01.in
```
