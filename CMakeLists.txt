cmake_minimum_required(VERSION 3.5)

project(CHUNK_GENERATOR CXX)

add_executable(main main.cpp)

add_library(all-a
  STATIC
   main.cpp 
)
include_directories("${PROJECT_SOURCE_DIR}/FastNoise2/include")

add_subdirectory(FastNoise2)

target_link_libraries(main FastNoise)
target_link_libraries(main sfml-system sfml-graphics sfml-window)
target_link_libraries(main all-a)
