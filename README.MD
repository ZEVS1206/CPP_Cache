# 2Q Cache Implementation

## Overview
This project provides a simple implementation of the **2Q cache replacement algorithm** in C++.  
The 2Q algorithm is an enhancement of the classic LRU (Least Recently Used) policy that reduces cache pollution by separating *recently accessed* items from *frequently accessed* items.

The cache is divided into three parts:
- **IN (A1in):** Stores items that were accessed recently.
- **OUT (A1out):** Keeps track of recently evicted items.
- **LRU (Am):** Stores items that are accessed frequently.

---

## Build Instructions
### Requirements
- C++17 or newer
- CMake 
- Makefile

### How to start:
```bash
$ git clone https://github.com/ZEVS1206/CPP_Cache.git
$ cd CPP_Cache
$ make
```
Then, if you want to start **Cache_2Q** with test from file, you need to write:
```bash
$ make run_2q file=[name of your file for testing with an extension]
```
But if you want to start **Cache_2Q** with test from console(stdin), you just need:
```bash
$ make run_2q
```
If you want to start **Ideal_cache** with test from file, you need to write:
```bash
$ make run_ideal file=[name of your file for testing with an extension]
```
But if you want to start **Ideal_cache** with test from console(stdin), you just need:
```bash
$ make run_ideal
```
## Important!:
Your file for testing must be in the catalog **CPP_Cache/tests**

## Reference:
Where I got the algorithm: [source](https://arpit.substack.com/p/2q-cache-management-algorithm)
