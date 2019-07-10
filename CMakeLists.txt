cmake_minimum_required(VERSION 3.1)

project(CppErr VERSION 0.1 
    LANGUAGES CXX
)

if(NOT DEFINED CPPERR_BUILD_TESTS OR CPPERR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

set(SRCS
    src/CppErr/Error.cpp
    src/CppErr/ErrorMessage.cpp
)

if(NOT DEFINED CPPERR_BUILD_TYPE)
    add_library(${PROJECT_NAME} SHARED ${SRCS})
else()
    add_library(${PROJECT_NAME} ${CPPERR_BUILD_TYPE} ${SRCS})
endif()

# Add include path
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
