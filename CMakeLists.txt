cmake_minimum_required(VERSION 3.14)
project(DesignPatterns CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

# One executable per .cpp, named <folder>_<file>, e.g. 05-Builder_good_fluent_builder
file(GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/0*/*.cpp")

foreach(src IN LISTS EXAMPLES)
    get_filename_component(file_name "${src}" NAME_WE)
    get_filename_component(dir_path  "${src}" DIRECTORY)
    get_filename_component(dir_name  "${dir_path}" NAME)
    set(target "${dir_name}_${file_name}")

    add_executable(${target} "${src}")
    target_link_libraries(${target} PRIVATE Threads::Threads)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /EHsc)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    endif()
endforeach()
