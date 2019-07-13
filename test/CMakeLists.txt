cmake_minimum_required(VERSION 3.1)

project(CppErrTests VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#############################################
# Creates a test.                           #
# Takes the test name and the sources names.#
#############################################
function(CREATE_TEST)
    set(TEST_NAME ${ARGV0})
    set(BIN_NAME ${TEST_NAME})

    # Adds test binary to binaries list (for linking, if needed).
    set(TEST_BIN_NAMES ${TEST_BIN_NAMES} "${BIN_NAME}" PARENT_SCOPE)

    # Adds sources.
    foreach(INDEX RANGE 1 ${ARGC})
        set(SRC_LIST ${SRC_LIST} ${ARGV${INDEX}})
    endforeach()

    # Creates executable and associated test.
    add_executable("${BIN_NAME}" ${SRC_LIST})
    add_test(NAME "${TEST_NAME}" COMMAND "${BIN_NAME}")
endfunction()

CREATE_TEST(error_test ErrorTest.cpp)
CREATE_TEST(error_type_test ErrorTypeTest.cpp)

# Link libraries for all tests.
foreach(TEST_BIN_NAME IN LISTS TEST_BIN_NAMES)
    target_link_libraries(${TEST_BIN_NAME} CppErr)
endforeach()
