#include <iostream>
#include <list>
#include <unordered_map>
#include <cstring>
#include <vector>

#include "cache_2q.h"

const int max_static_size = 100;

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
        //std::cout << "\x1b[32mReading tests from console...\x1b[0m\n";
        file_with_tests = stdin;
    }

    // if (file_with_tests == stdin)
    // {
    //     std::cout << "Amount: ";
    // }
    char buffer_for_input[max_static_size] = "";
    while (fgets(buffer_for_input, sizeof(buffer_for_input), file_with_tests))
    {
        size_t hits = 0;
        std::vector <Page_t> data_array;
        // if (file_with_tests == stdin)
        // {
        //     std::cout << "Test: ";
        // }
        int number = 0;
        char *pointer = buffer_for_input;
        while (sscanf(pointer, "%d", &number) == 1)
        {
            data_array.push_back(number);
            while (*pointer && *pointer != ' ' && *pointer != '\t' && *pointer != '\n')
            {
                pointer++;
            }
            while (*pointer == ' ' || *pointer == '\t')
            {
                pointer++;
            }
        }
        //std::cout << "Test: ";
        // for (Page_t elem: data_array)
        // {
        //     std:: cout << elem << " ";
        // }
        // std::cout << "\n";
        struct Cache_2q <Page_t, Page_t, decltype(&slow_get_page)> cache{data_array.size()};
        for (size_t index = 0; index < data_array.size(); index++)
        {
            ON_DEBUG(cache.print_buffer_in();)
            ON_DEBUG(cache.print_buffer_lru();)
            ON_DEBUG(cache.print_buffer_out();)
            if (cache.cache_2q_lookup_update(data_array[index], slow_get_page))
            {
                hits++;
            }
        }
        std::cout << hits << "\n\n";
        //std::cout << "\x1b[34mmisses in cache_2Q\x1b[0m = " << misses << "\n";

        //int hits_in_ideal_cache = simulate_optimal_cache(data_array, data_array.size(), data_array.size() / 2);
        //std::cout << "\x1b[35mhits in ideal_cache\x1b[0m = " << hits_in_ideal_cache << "\n";
        //std::cout << "\n\n\n";
        // if (file_with_tests == stdin)
        // {
        //     std::cout << "Amount: ";
        // }
    }
    return 0;
}