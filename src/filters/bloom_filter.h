
#pragma once

#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <algorithm>
#include <bitset>
#include <sys/_types/_size_t.h>
#include <type_traits>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include "hash/city.h"
#include "hash/hash.h"

namespace filters {

struct bloom_filter_options {
    uint8_t numHashFunctions;
    size_t numBits;
};

namespace details {
template<typename Container, typename Key, typename Hash>
class bloom_filter_base {
    static_assert(std::is_default_constructible_v<Hash>, "Cannot instantiate hasher.");

public:
    explicit bloom_filter_base(Container&& bits, uint8_t k) :
        _bits{std::move(bits)},
        _k{k} {}

    void insert(const Key& key) {
        Hash hasher;
        for (uint8_t i = 0; i < _k; ++i) {
            auto idx = hasher(key, i);
            _bits[idx % _bits.size()] = true;
        }
    }

    bool query(const Key& key) {
        Hash hasher;
        for (uint8_t i = 0; i < _k; ++i) {
            auto idx = hasher(key, i);
            if (!_bits[idx % _bits.size()]) {
                return false;
            }
        }
        return true;
    }

    size_t getNumBits() {
        return _bits.size();
    }

    uint8_t getNumHashFunctions() {
        return _k;
    }

private:
    Hash hasher;
    Container _bits;
    uint8_t _k;
};
}


template<typename Key, typename Hash = filters::hash<Key>,
    typename Container = std::vector<bool>>
class bloom_filter : public details::bloom_filter_base<Container, Key, Hash> {
public:
    explicit bloom_filter(bloom_filter_options opts)
            : details::bloom_filter_base<Container, Key, Hash>{Container(opts.numBits),
                    opts.numHashFunctions} { }
};

template<typename Key, size_t size, typename Hash = filters::hash<Key>>
class static_bloom_filter : public details::bloom_filter_base<std::bitset<size>, Key, Hash> {
public:
    explicit static_bloom_filter(uint8_t k)
            : details::bloom_filter_base<std::bitset<size>, Key, Hash>{std::bitset<size>{}, k} { }

};

// Optimal formula:
// https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
inline bloom_filter_options getOptimalBloomFilterOptions(double precision, uint32_t numKeys) {
    auto x = (numKeys * std::log(precision)
            / std::pow(std::log(2), 2)) * -1;
    auto numBits = static_cast<size_t>(x);
    uint8_t numHashFunctions = std::ceil(numBits * std::log(2) / numKeys);
    return {numHashFunctions, numBits};
}

}

