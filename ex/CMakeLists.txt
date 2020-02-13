cmake_minimum_required(VERSION 3.10)

#project(TeslynExamples VERSION 0.1)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/ex)

file( GLOB EX_SOURCES *.cpp)

foreach( exsourcefile ${EX_SOURCES} )

    get_filename_component(exname ${exsourcefile} NAME_WE )

    add_executable( ${exname} ${exsourcefile} )

    set( COMPILE_OPTIONS "" )

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        list(APPEND COMPILE_OPTIONS "DEBUG" "PROFILING")
    endif()

    if (WIN32)
        list(APPEND COMPILE_OPTIONS "PLATFORM_WINDOWS")
    elseif(UNIX)
        list(APPEND COMPILE_OPTIONS "PLATFORM_LINUX")
    endif()

    target_compile_definitions(${exname} PRIVATE ${COMPILE_OPTIONS})

    target_include_directories(${exname}
    PRIVATE
      "${PROJECT_BINARY_DIR}"
      "${PROJECT_BINARY_DIR}"
      "${LIBRARY_OUTPUT_PATH}"
    )

    target_link_libraries(${exname}
    PRIVATE 
        Teslyn
    )

endforeach( exsourcefile ${EX_SOURCES} )