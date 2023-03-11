

#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <algorithm>
#include <sys/_types/_size_t.h>
#include <type_traits>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include "third_party/cityhash/src/city.h"

namespace bf {

template<typename T>
struct hash;

template<>
struct hash<std::string> {
    uint64_t operator()(const std::string& key, int seed) {
        return CityHash64WithSeed(key.c_str(),
                key.size(), seed);
    }
};

template<>
struct hash<int> {
    uint64_t operator()(int key, int seed) {
        return CityHash64WithSeed(reinterpret_cast<const char*>(&key),
                sizeof(key), seed);
    }
};

template<typename Key, typename Hash = hash<Key>>
class bloom_filter {
public:

    // Use optimal values of k and m.
    explicit bloom_filter(double precision, uint32_t n)
            : _bits(_getArraySize(precision, n)),
              _k{_getNumHashFunctions(n, _bits.size())} { }

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
    // Optimal formula:
    // https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions
    static size_t _getArraySize(double precision, uint32_t numKeys) {
        auto x = (numKeys * std::log(precision)
            / std::pow(std::log(2), 2)) * -1;

        return static_cast<size_t>(x);
    }

    static uint8_t _getNumHashFunctions(uint32_t numKeys, uint32_t numBits) {
        auto x = numBits * std::log(2) / numKeys;

        return std::ceil(x);
    }

    std::vector<bool> _bits;
    uint8_t _k;
};

}

