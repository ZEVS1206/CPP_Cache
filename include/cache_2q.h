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

enum Zone
{
    UNKNOWN = 0,
    IN      = 1,
    LRU     = 2,
    OUT     = 3
};

template <typename Elem_t, typename Key_t, typename Function_get_elem_t>
struct Cache_2q
{
    struct Cache_Entry
    {
        typename std::list<Key_t>::iterator it_key_list;
        Elem_t value;
        Zone zone = UNKNOWN;
    };

    std::list <Key_t> buffer_in_;
    std::list <Key_t> buffer_lru_;
    std::list <Key_t> buffer_out_;

    std::unordered_map<Key_t, Cache_Entry> common_hash_table_;

    size_t size_of_buffer_in_  = 0;
    size_t size_of_buffer_lru_ = 0;
    size_t size_of_buffer_out_ = 0;

    Cache_2q(size_t size_of_cache)
    {
        ON_DEBUG(std::cout << "Common size of buffer: " << size_of_cache << "\n";)

        if (size_of_cache == 1)
            size_of_buffer_in_ = size_of_cache;
        else
            size_of_buffer_in_ = size_of_cache / 2;

        ON_DEBUG(std::cout << "Buffer in size: " << size_of_buffer_in_ <<  "\n";)

        size_of_buffer_lru_ = size_of_cache;
        ON_DEBUG(std::cout << "Buffer lru size: " << size_of_buffer_lru_ << "\n";)

        if (size_of_cache == 1)
            size_of_buffer_out_ = size_of_cache;
        else
            size_of_buffer_out_ = size_of_cache / 2;

        ON_DEBUG(std::cout << "Buffer out size: " << size_of_buffer_out_ << "\n";)
    }

    bool cache_2q_lookup_update(Key_t key, Function_get_elem_t slow_get_page)
    {
        ON_DEBUG(std::cout << "Analyzing started!\n";)

        auto it_in_table = common_hash_table_.find(key);
        if (it_in_table != common_hash_table_.end())
        {
            Cache_Entry &element = it_in_table->second;

            if (element.zone == IN)
            {
                ON_DEBUG(std::cout << "Found in buffer_in\n";)
                return true;
            }
            else if (element.zone == LRU)
            {
                ON_DEBUG(std::cout << "Found in buffer_lru\n";)
                buffer_lru_.splice(buffer_lru_.begin(), buffer_lru_, element.it_key_list);
                element.it_key_list = buffer_lru_.begin();
                return true;
            }
            else if (element.zone == OUT)
            {
                ON_DEBUG(std::cout << "Found in buffer_out\n";)
                delete_from_out(&element);
                add_to_lru(key, &element);
                return true;
            }
        }

        add_to_in(key, slow_get_page(key), nullptr);
        ON_DEBUG(std::cout << "Not found, added to buffer_in\n";)
        return false;
    }

    void add_to_lru(Key_t key, Cache_Entry *element)
    {
        if (buffer_lru_.size() >= size_of_buffer_lru_)
        {
            delete_from_lru();
        }

        buffer_lru_.push_front(key);

        if (element != nullptr)
        {
            element->it_key_list = buffer_lru_.begin();
            element->zone = LRU;
        }
        else
        {
            Cache_Entry new_entry;
            new_entry.value = Elem_t();
            new_entry.zone = LRU;
            new_entry.it_key_list = buffer_lru_.begin();
            common_hash_table_[key] = std::move(new_entry);
        }
    }

    void add_to_in(Key_t key, Elem_t elem, Cache_Entry * /*element*/)
    {
        if (buffer_in_.size() >= size_of_buffer_in_)
        {
            Key_t old = buffer_in_.back();
            delete_from_in();
            add_to_out(old);
        }

        buffer_in_.push_front(key);

        Cache_Entry new_element;
        new_element.value = std::move(elem);
        new_element.zone = IN;
        new_element.it_key_list = buffer_in_.begin();

        common_hash_table_[key] = std::move(new_element);
    }

    void add_to_out(Key_t key)
    {
        auto it_old = common_hash_table_.find(key);
        if (it_old != common_hash_table_.end())
        {
            if (buffer_out_.size() >= size_of_buffer_out_)
            {
                Key_t last = buffer_out_.back();
                common_hash_table_.erase(last);
                buffer_out_.pop_back();
            }
            it_old->second.zone = OUT;
            buffer_out_.push_front(key);
            it_old->second.it_key_list = buffer_out_.begin();
        }
    }

    void delete_from_out(Cache_Entry *element)
    {
        if (element == nullptr) 
        {
            return;
        }
        buffer_out_.erase(element->it_key_list);
        element->zone = UNKNOWN;
    }

    void delete_from_in()
    {
        if (buffer_in_.empty()) 
        {
            return;
        }
        buffer_in_.pop_back();
    }

    void delete_from_lru()
    {
        if (buffer_lru_.empty())
        {
            return;
        }

        Key_t k = buffer_lru_.back();
        common_hash_table_.erase(k);
        buffer_lru_.pop_back();
    }

    void print_buffer_in()
    {
        std::cout << "Buffer in: ";
        for (auto &k: buffer_in_)
        {
            auto it = common_hash_table_.find(k);
            if (it != common_hash_table_.end())
                std::cout << "(" << k << ":" << it->second.value << ") ";
            else
                std::cout << "(" << k << ":?) ";
        }
        std::cout << "\n";
    }

    void print_buffer_out()
    {
        std::cout << "Buffer out: ";
        for (auto &k: buffer_out_)
        {
            std::cout << k << " ";
        }
        std::cout << "\n";
    }

    void print_buffer_lru()
    {
        std::cout << "Buffer lru: ";
        for (auto &k: buffer_lru_)
        {
            auto it = common_hash_table_.find(k);
            if (it != common_hash_table_.end())
                std::cout << "(" << k << ":" << it->second.value << ") ";
            else
                std::cout << "(" << k << ":?) ";
        }
        std::cout << "\n";
    }

};

typedef int Page_t;

int simulate_optimal_cache(const std::vector<Page_t>& data, size_t data_size, size_t cache_size);

#endif
