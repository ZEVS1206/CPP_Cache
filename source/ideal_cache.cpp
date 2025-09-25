#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "cache_2q.h"

int simulate_optimal_cache(const std::vector <Page_t> data, size_t data_size, size_t cache_size) 
{
    std::unordered_set<Page_t> cache;
    int hits = 0;

    for (size_t index = 0; index < data_size; index++) {
        int page = data[index];
        if (cache.find(page) != cache.end()) {
            hits++;
            continue;
        }
        // miss

        if (cache.size() < cache_size) {
            cache.insert(page);
        } else {
            Page_t victim = -1;
            size_t farthest = 0;
            for (Page_t c : cache) {
                size_t j = index + 1;
                for (; j < data_size; j++) 
                {
                    if (data[j] == c) 
                    {
                        break;
                    }
                }
                if (j > farthest) 
                {
                    farthest = j;
                    victim = c;
                }
            }
            cache.erase(victim);
            cache.insert(page);
        }
    }
    return hits;
}
