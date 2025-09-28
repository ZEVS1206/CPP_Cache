#include <iostream>
#include <list>
#include <unordered_map>
#include <cstring>
#include <vector>

#include "cache_2q.h"

const int max_static_size = 1000000;

int slow_get_page(int key)
{
    return key;
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

        Cache_2q<Page_t, Page_t, decltype(&slow_get_page)> cache(cache_size);

        size_t hits = 0;
        for (size_t i = 0; i < data_array.size(); i++)
        {
            if (cache.cache_2q_lookup_update(data_array[i], slow_get_page))
            {
                hits++;
            }
        }

        std::cout << hits << "\n";
    }
    if (file_with_tests != stdin)
    {
        fclose(file_with_tests);
    }

    return 0;
}