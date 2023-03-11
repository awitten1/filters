
add_library(cityHash SHARED cityhash/src/city.cc)
target_include_directories(cityHash
    PRIVATE cityhash/src
    PRIVATE cityhash
)
