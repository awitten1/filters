

#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <algorithm>
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

template<typename Key, typename Hash = filters::hash<Key>>
class bloom_filter {
public:
    // Use optimal values of k and m.
    explicit bloom_filter(bloom_filter_options opts)
            : _bits(opts.numBits),
              _k{opts.numHashFunctions} { }

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

    uint8_t getNumHashFunctions() {
        return _k;
    }

    std::vector<bool>::size_type getNumBits() {
        return _bits.size();
    }

private:
    std::vector<bool> _bits;
    uint8_t _k;
};

// Optimal formula:
// https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
static bloom_filter_options getOptimalBloomFilterOptions(double precision, uint32_t numKeys) {
    auto x = (numKeys * std::log(precision)
            / std::pow(std::log(2), 2)) * -1;
    auto numBits = static_cast<size_t>(x);
    uint8_t numHashFunctions = std::ceil(numBits * std::log(2) / numKeys);
    return {numHashFunctions, numBits};
}

}

