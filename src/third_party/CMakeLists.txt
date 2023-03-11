


add_custom_target(ConfigureCityhash
    COMMAND "./configure"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cityhash
)

add_library(cityHash SHARED cityhash/src/city.cc)

add_dependencies(cityHash ConfigureCityhash)

target_include_directories(cityHash
    PRIVATE cityhash/src
    PRIVATE cityhash
)
