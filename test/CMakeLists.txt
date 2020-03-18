cmake_minimum_required(VERSION 3.10)

project(TeslynTests VERSION 0.1)

# add the executable
add_executable(TeslynTests 
    src/tests.cpp
    src/tensor.cpp
    # src/neural_network.cpp
    # src/display.cpp
)

target_link_libraries(TeslynTests 
PRIVATE
    Teslyn
)

set(COMPILE_OPTIONS "")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    list(APPEND COMPILE_OPTIONS "DEBUG")
endif()

if (WIN32)
    list(APPEND COMPILE_OPTIONS "PLATFORM_WINDOWS")
elseif(UNIX)
    list(APPEND COMPILE_OPTIONS "PLATFORM_LINUX")
endif()

target_compile_definitions(TeslynTests PRIVATE ${COMPILE_OPTIONS})

target_include_directories(TeslynTests 
PRIVATE
    "include/"
    "${PROJECT_BINARY_DIR}"
    "${LIBRARY_OUTPUT_PATH}"
)
