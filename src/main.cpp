#include <iostream>
#include <string>
#include <exception>

#include "bloom_filter/bloom_filter.h"

int main() try {
    bf::bloom_filter<int> bf{.1, 500};
    int count = 0;
    for (int i = 0; i < 500; ++i) {
        bf.insert(i);
    }
    for (int i = 501; i < 1000; ++i) {
        count += bf.query(i);
    }
    std::cout << static_cast<double>(count) / 500 << std::endl;
    std::cout << bf.getNumHashFunctions() << std::endl;

    count = 0;

    for (int i = 0; i < 500; ++i) {
        count += bf.query(i);
    }
    std::cout << static_cast<double>(count) / 500 << std::endl;
} catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
}
