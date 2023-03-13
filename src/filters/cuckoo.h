
#pragma once

#include "hash/city.h"
#include "hash/hash.h"
#include <array>
#include <type_traits>
#include <vector>

namespace filters {

template<typename Key, size_t bucket_size = 5
    , typename Hash = filters::hash<Key>>
class cuckoo_filter {
    using fingerprint = uint32_t;

    static_assert(std::is_default_constructible_v<Hash>, "Hash must be default constructible.")
public:
    void insert(const Key& key) {
        _hasher(key, 0);

    }
    void remove(const Key& key) {

    }
    void query(const Key& key) {

    }
private:
    Hash _hasher;
    std::vector<std::array<fingerprint, bucket_size>> _buckets;
};


}
