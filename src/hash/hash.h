#include <string>
#include <type_traits>
#include <iostream>

#include "city.h"

namespace filters {

namespace detail {

template<typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
struct hash_detail {
    uint64_t operator()(int key, int seed) {
        return CityHash64WithSeed(reinterpret_cast<const char*>(&key),
                sizeof(key), seed);
    }
};

template<>
struct hash_detail<std::string, void> {
    uint64_t operator()(const std::string& key, int seed) {
        return CityHash64WithSeed(key.c_str(),
                key.size(), seed);
    }
};

}

template<typename T>
struct hash : detail::hash_detail<T> {};

template<>
struct hash<std::string> : detail::hash_detail<std::string, void> {};

}