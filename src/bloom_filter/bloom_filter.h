

#include <_types/_uint8_t.h>
#include <algorithm>
#include <sys/_types/_size_t.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include "cityhash/src/city.h"

namespace bf {

template<typename T>
struct BufferRetriever;

template<>
struct BufferRetriever<std::string> {
    const char* getBuffer(const std::string& x) {
        return x.data();
    }
    int getBufferSize(const std::string& x) {
        return x.size();
    }
};

template<>
struct BufferRetriever<int> {
    const char* getBuffer(const int& x) {
        return (const char*)&x;
    }
    int getBufferSize(const int& x) {
        return sizeof(int);
    }
};

template<typename Key>
class bloom_filter {
public:

    explicit bloom_filter(double precision, uint32_t n)
            : _bits(_getArraySize(precision, n)),
              _k{_getNumHashFunctions(n, _bits.size())} { }

    void insert(const Key& key) {
        for (uint8_t i = 0; i < _k; ++i) {
            uint64_t idx = CityHash64WithSeed(_bufferRetriever.getBuffer(key),
                _bufferRetriever.getBufferSize(key), i);
            _bits[idx % _bits.size()] = true;
        }
    }

    bool query(const Key& key) {
        for (uint8_t i = 0; i < _k; ++i) {
            uint64_t idx = CityHash64WithSeed(_bufferRetriever.getBuffer(key),
                _bufferRetriever.getBufferSize(key), i);
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
    BufferRetriever<Key> _bufferRetriever;
};

}

