#include <iostream>
#include <string>
#include <iomanip>

#include "filters/bloom_filter.h"

int main() {
    filters::bloom_filter<std::string> bf{filters::getOptimalBloomFilterOptions(.01, 500)};
    for (int i = 0; i < 500; ++i) {
        bf.insert(std::to_string(i));
    }

    int fp = 0;
    int count = 0;
    for (int i = 500; i < 400000; ++i) {
        ++count;
        fp += bf.query(std::to_string(i));
    }
    std::cout << std::setprecision(5)
        << static_cast<double>(fp) / count << std::endl;
}
