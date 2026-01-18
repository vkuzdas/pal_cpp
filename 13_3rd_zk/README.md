# 📝 Exam 3 — Prime Generator Selection

**Exam Problem 3** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Select a subset of prime numbers that can generate all required values through multiplication, minimizing the number of primes used.

## Approach

1. **Prime Sieve** — Generate primes up to $\sqrt{M_{max}}$
2. **Prime Combination** — Try subsets of primes as generators
3. **Coverage Check** — Verify all target values can be generated

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Sieve of Eratosthenes | Prime generation | $O(n \log \log n)$ |
| Combination Generation | Prime subset selection | $O(\binom{P}{F})$ |
| GCD/LCM | Divisibility checks | $O(\log n)$ |

## Files

| File | Description |
|------|-------------|
| `3zk.cpp` | Main exam solution |
| `hello.cpp` | Test variant |
| `prev_hello.cpp` | Previous attempt |

## Usage

```bash
g++ -O2 -o 3zk 3zk.cpp
./3zk < input.txt
```
