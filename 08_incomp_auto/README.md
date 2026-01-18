# 🤖 Incomplete Automaton — DFA Completion

**Assignment 08** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Given an incomplete Deterministic Finite Automaton (DFA) with positive and negative example strings, determine which states should be marked as final so that:
- All positive examples are accepted
- All negative examples are rejected

## Approach

1. **Parse Automaton** — Read states, alphabet, transitions
2. **Simulate Strings** — Run each example through the DFA
3. **Final State Assignment** — Determine valid final state configurations

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| DFA Simulation | String acceptance check | $O(L)$ per string |
| Constraint Satisfaction | Final state assignment | $O(2^S \cdot (P + N))$ |

Where S = states, L = string length, P/N = positive/negative examples.

## Files

| File | Description |
|------|-------------|
| `iauto.cpp` | Main solution |
| `inca_2.cpp` | Alternative approach |

## Usage

```bash
g++ -O2 -o iauto iauto.cpp
./iauto < input.txt
```
