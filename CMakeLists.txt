cmake_minimum_required(VERSION 3.22)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(ant-sim src/main.cpp src/
        src/Ant.cpp
        src/Food.cpp
        src/Helpers.cpp
        src/Marker.cpp
        src/Random.cpp
        src/Scene.cpp
        src/Sensor.cpp
        src/Timer.cpp
        src/Timer.h)

target_compile_features(ant-sim PRIVATE cxx_std_17)
target_link_libraries(ant-sim PRIVATE SFML::Graphics)
if(MINGW)
    target_link_options(ant-sim PRIVATE -static)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -g -O2")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -g -O2")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Debug build with profiling information
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")