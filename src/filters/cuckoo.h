
#pragma once

#include "hash/hash.h"
#include <type_traits>

namespace filters {

template<typename Key, typename Hash = filters::hash<Key>>
class cuckoo_filter {
    static_assert(std::is_default_constructible_v<Hash>, "Hash must be default constructible.")
public:
    void insert(const Key& key) {

    }
    void remove(const Key& key) {

    }
    void query(const Key& key) {

    }
private:
};


}
