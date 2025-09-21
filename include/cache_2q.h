#ifndef CACHE_2Q_H
#define CACHE_2Q_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <cstddef>

//#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif

template <typename Elem_t, typename Key_t, typename Function_get_elem_t>
struct Cache_2q
{
    std::list<std::pair<Key_t, Elem_t>> buffer_in_;
    std::list<std::pair<Key_t, Elem_t>> buffer_lru_;
    std::list<Key_t> buffer_out_;

    std::unordered_map<Key_t, typename std::list<std::pair<Key_t, Elem_t>>::iterator> hash_table_in_;
    std::unordered_map<Key_t, typename std::list<std::pair<Key_t, Elem_t>>::iterator> hash_table_lru_;
    std::unordered_map<Key_t, typename std::list<Key_t>::iterator> hash_table_out_;

    size_t size_of_buffer_in_  = 0;
    size_t size_of_buffer_lru_ = 0;
    size_t size_of_buffer_out_ = 0;

    Cache_2q(size_t size_of_cache)
    {
        std::cout << "Common size of buffer: " << size_of_cache << "\n";

        size_of_buffer_in_ = size_of_cache / 4;
        std::cout << "Buffer in size: " << size_of_buffer_in_ <<  "\n";

        size_of_buffer_lru_ = size_of_cache / 2;
        std::cout << "Buffer lru size: " << size_of_buffer_lru_ << "\n";

        size_of_buffer_out_ = size_of_cache / 4;
        std::cout << "Buffer out size: " << size_of_buffer_out_ << "\n";
    }

    bool cache_2q_lookup_update(Key_t key, Function_get_elem_t slow_get_page)
    {
        ON_DEBUG(std::cout << "Analyzing started!\n";)

        auto hit_in_buffer_in = hash_table_in_.find(key);
        if (hit_in_buffer_in != hash_table_in_.end())
        {
            ON_DEBUG(std::cout << "Found in buffer_in\n";)
            return true;
        }

        auto hit_in_buffer_lru = hash_table_lru_.find(key);
        if (hit_in_buffer_lru != hash_table_lru_.end())
        {
            ON_DEBUG(std::cout << "Found in buffer_lru\n";)
            buffer_lru_.splice(buffer_lru_.begin(), buffer_lru_, hit_in_buffer_lru->second);
            hash_table_lru_[key] = buffer_lru_.begin();
            return true;
        }

        auto hit_in_buffer_out = hash_table_out_.find(key);
        if (hit_in_buffer_out != hash_table_out_.end())
        {
            ON_DEBUG(std::cout << "Found in buffer_out\n";)
            add_to_lru(key, slow_get_page(key));
            delete_from_out(hit_in_buffer_out->second);
            return true;
        }

        add_to_in(key, slow_get_page(key));
        ON_DEBUG(std::cout << "Not found, added to buffer_in\n";)
        return false;
    }

    void add_to_lru(Key_t key, Elem_t element)
    {
        if (buffer_lru_.size() >= size_of_buffer_lru_)
        {
            delete_from_lru();
        }

        buffer_lru_.push_front(std::make_pair(key, element));
        hash_table_lru_[key] = buffer_lru_.begin();
    }

    void add_to_in(Key_t key, Elem_t element)
    {
        if (buffer_in_.size() >= size_of_buffer_in_)
        {
            auto old = buffer_in_.back();
            add_to_out(old.first);
            delete_from_in();
        }

        buffer_in_.push_front(std::make_pair(key, element));
        hash_table_in_[key] = buffer_in_.begin();
    }

    void add_to_out(Key_t key)
    {
        if (buffer_out_.size() >= size_of_buffer_out_)
        {
            
            Key_t last = buffer_out_.back();
            hash_table_out_.erase(last);
            buffer_out_.pop_back();
        }
        buffer_out_.push_front(key);
        hash_table_out_[key] = buffer_out_.begin();
    }

    
    void delete_from_out(typename std::list<Key_t>::iterator iterator_on_buffer_out)
    {
        hash_table_out_.erase(*iterator_on_buffer_out);
        buffer_out_.erase(iterator_on_buffer_out);
    }

    void delete_from_in()
    {
        Key_t k = buffer_in_.back().first;
        hash_table_in_.erase(k);
        buffer_in_.pop_back();
    }

    void delete_from_lru()
    {
        Key_t k = buffer_lru_.back().first;
        hash_table_lru_.erase(k);
        buffer_lru_.pop_back();
    }

    void print_buffer_in()
    {
        std::cout << "Buffer in: ";
        for (auto &element: buffer_in_)
        {
            std::cout << "(" << element.first << ":" << element.second << ") ";
        }
        std::cout << "\n";
    }

    void print_buffer_out()
    {
        std::cout << "Buffer out: ";
        for (auto &element: buffer_out_)
        {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }

    void print_buffer_lru()
    {
        std::cout << "Buffer lru: ";
        for (auto &element: buffer_lru_)
        {
            std::cout << "(" << element.first << ":" << element.second << ") ";
        }
        std::cout << "\n";
    }

};

typedef int Page_t;

int simulate_optimal_cache(const std::vector <Page_t> data, size_t data_size, size_t cache_size);

#endif
