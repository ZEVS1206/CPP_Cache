#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cstring>

#include "cache_2q.h"

int simulate_optimal_cache(const std::vector <Page_t> data, size_t data_size, size_t cache_size) 
{
    std::unordered_set<Page_t> cache;
    int hits = 0;

    for (size_t index = 0; index < data_size; index++) 
    {
        int page = data[index];
        if (cache.find(page) != cache.end()) {
            hits++;
            continue;
        }
        // miss

        if (cache.size() < cache_size) 
        {
            cache.insert(page);
        } 
        else 
        {
            Page_t victim = -1;
            size_t farthest = 0;
            for (Page_t c : cache) 
            {
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


int main(int argc, char *argv[])
{
    FILE *file_with_tests = NULL;
    if (argc == 2)
    {
        char file_name[50] = "tests/";
        snprintf(file_name + strlen(file_name), 50, "%s", argv[1]);
        //printf("\x1b[33mfile_with_tests = %s\x1b[0m\n", file_name);
        file_with_tests = fopen(file_name, "rb");
        if (file_with_tests == NULL) 
        {
            std::cout << "\x1b[31mERROR\x1b[0m: The test file cannot be opened\n";
            return 1;
        }
    }
    else
    {
        file_with_tests = stdin;
    }

    size_t cache_size = 0;
    size_t n = 0;

    while (fscanf(file_with_tests, "%zu %zu", &cache_size, &n) == 2)
    {
        std::vector<Page_t> data_array(n);
        for (size_t i = 0; i < n; i++)
        {
            if (fscanf(file_with_tests, "%d", &data_array[i]) != 1)
            {
                std::cerr << "ERROR: unexpected end of input\n";
                return 1;
            }
        }

        size_t hits = simulate_optimal_cache(data_array, n, cache_size);

        std::cout << hits << "\n";
    }
    if (file_with_tests != stdin)
    {
        fclose(file_with_tests);
    }

    return 0;
}