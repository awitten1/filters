#include <string>
#include <type_traits>
#include <iostream>

#include "city.h"

namespace filters {

namespace detail {


// Less Hashing, Same Performance!
// https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
// Simulating k hash functions with only 2.
template<typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
struct hash_detail {
    uint64_t operator()(int key, int i) {
        uint128 h = CityHash128(reinterpret_cast<const char*>(&key),
            sizeof(key));
        return h.first + i * h.second;
    }
};

template<>
struct hash_detail<std::string, void> {
    uint64_t operator()(const std::string& key, int i) {
        uint128 h = CityHash128(key.c_str(),
            key.size());
        return h.first + i * h.second;
    }
};

}

template<typename T>
struct hash : detail::hash_detail<T> {};

template<>
struct hash<std::string> : detail::hash_detail<std::string, void> {};

}