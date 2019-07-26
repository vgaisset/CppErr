cmake_minimum_required(VERSION 3.1)

project(CppErr VERSION 0.1 
    LANGUAGES CXX
)

option(CPPERR_BUILD_TESTS "Must the unit tests be built ?" OFF)
option(CPPERR_BUILD_STATIC "Must the library be build as STATIC ?" OFF)

# Unit tests
if(CPPERR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# CppErr sources
set(src
    src/CppErr/Error.cpp
    src/CppErr/ErrorMessage.cpp
    src/CppErr/Result.cpp
)

# Library creation.
if(CPPERR_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${src})
else()
    add_library(${PROJECT_NAME} SHARED ${src})
endif()

# Include path.
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(project_config_name ${PROJECT_NAME}Config)
set(install_lib_folder ${CMAKE_INSTALL_PREFIX}/lib)
set(install_include_folder ${CMAKE_INSTALL_PREFIX}/include)
set(install_config_folder ${install_lib_folder}/${PROJECT_NAME})

# Installation.
install(TARGETS ${PROJECT_NAME} EXPORT ${project_config_name}
        ARCHIVE  DESTINATION ${install_lib_folder} # static lib
        LIBRARY  DESTINATION ${install_lib_folder} # shared lib
        RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin # Windows only (dll treated as binary)
)

install(DIRECTORY include/CppErr DESTINATION ${install_include_folder}) # headers
install(EXPORT ${project_config_name} DESTINATION ${install_config_folder})

export(TARGETS ${PROJECT_NAME} FILE ${project_config_name}.cmake)

