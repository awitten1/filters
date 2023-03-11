
include_directories(.)

add_subdirectory(third_party)

add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE cityHash
)
