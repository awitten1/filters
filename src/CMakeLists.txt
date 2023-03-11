
include_directories(.)
include_directories(cityhash)

add_library(cityHash SHARED cityhash/src/city.cc)

add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE cityHash
)
