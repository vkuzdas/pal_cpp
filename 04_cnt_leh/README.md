# 🔢 Count Lehmer — Prime Counting

**Assignment 04** for B4M33PAL (Algorithms) at CTU Prague.

## Problem

Count integers in a given range that have no prime factors below a threshold `D`. This is related to the [Lehmer prime counting](https://en.wikipedia.org/wiki/Prime-counting_function#Algorithms_for_evaluating_%CF%80(x)) method.

## Approach

1. **Sieve of Eratosthenes** — Generate primes up to $\sqrt{M_{max}}$
2. **Prime Factorization** — For each candidate, extract prime factors
3. **Filter** — Count numbers whose smallest prime factor ≥ D

## Algorithms Used

| Algorithm | Purpose | Complexity |
|-----------|---------|------------|
| Sieve of Eratosthenes | Prime generation | $O(n \log \log n)$ |
| Trial Division | Prime factorization | $O(\sqrt{n})$ |

## Files

| File | Description |
|------|-------------|
| `cnt.cpp` | Main solution |
| `datapub/` | Public test cases |

## Usage

```bash
g++ -O2 -o cnt cnt.cpp
./cnt < datapub/pub01.in
```
