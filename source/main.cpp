#include <iostream>
#include <list>
#include <unordered_map>
#include <cstring>
#include <vector>

#include "cache_2q.h"

int slow_get_page(int key)
{
    return key;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "\x1b[31mERROR\x1b[0m:There is no or too many parametres for test\n";
        return 1;
    }
    char file_name[50] = "tests/";
    snprintf(file_name + strlen(file_name), 50, "%s", argv[1]);
    printf("\x1b[33mfile_with_tests = %s\x1b[0m\n", file_name);
    FILE *file_with_tests = fopen(file_name, "rb");
    if (file_with_tests == NULL)
    {
        std::cout << "\x1b[31mERROR\x1b[0m:The test file can not be opened\n";
        return 1;
    }
    size_t size_of_array = 0;
    while (fscanf(file_with_tests, "%lu", &size_of_array) == 1)
    {
        struct Cache_2q <Page_t, Page_t, decltype(&slow_get_page)> cache{size_of_array};
        size_t misses = 0;
        std::vector <Page_t> data_array(size_of_array);
        for (size_t index = 0; index < size_of_array; index++)
        {
            fscanf(file_with_tests, "%d", &(data_array[index]));
        }
        std::cout << "Test: ";
        for (Page_t elem: data_array)
        {
            std:: cout << elem << " ";
        }
        std::cout << "\n";
        for (size_t index = 0; index < size_of_array; index++)
        {
            ON_DEBUG(cache.print_buffer_in();)
            ON_DEBUG(cache.print_buffer_lru();)
            ON_DEBUG(cache.print_buffer_out();)
            if (!cache.cache_2q_lookup_update(data_array[index], slow_get_page))
            {
                misses++;
            }
        }
        std::cout << "\x1b[34mmisses in cache_2Q\x1b[0m = " << misses << "\n";

        int misses_in_ideal_cache = simulate_optimal_cache(data_array, size_of_array, size_of_array / 2);
        std::cout << "\x1b[35mmisses in ideal_cache\x1b[0m = " << misses_in_ideal_cache << "\n";
        std::cout << "\n\n\n";
    }
    return 0;
}