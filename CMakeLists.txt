cmake_minimum_required(VERSION 3.1)

project(CppErr VERSION 0.1 
    LANGUAGES CXX
)

set(SRCS
    src/CppErr/Error.cpp
    src/CppErr/ErrorMessage.cpp
)

add_library(${PROJECT_NAME} ${SRCS})

# Add include path
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
