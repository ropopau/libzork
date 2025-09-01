cmake_minimum_required(VERSION 3.21.2)
project(libzork VERSION 1.0.0 LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")


add_compile_options(-Wall -Wextra -fsanitize=address -g -O0 -std=c++20)
#add_compile_options(-Wall -Wextra -pedantic -std=c++20 -Wold-style-cast)
add_link_options(-fsanitize=address)



#find_library(YAML-CPP NAMES yaml-cpp libyaml-cpp REQUIRED)

find_package(yaml-cpp REQUIRED)

add_library(libzork SHARED)

target_include_directories(libzork PUBLIC include)
target_include_directories(libzork PRIVATE src ) 

#target_link_libraries(libzork PRIVATE ${YAML-CPP})
target_link_libraries(libzork PRIVATE yaml-cpp::yaml-cpp)

set_target_properties(libzork PROPERTIES OUTPUT_NAME "libzork")

add_subdirectory(src)

add_subdirectory(zorkxplorer)




