if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Set up the test executable
add_executable(test_hello_world test_foo.cpp)

# Link the test executable with Google Test and your function library
target_link_libraries(test_hello_world gtest my_functions)

# Add a test to be run by ctest
add_test(NAME test_hello_world COMMAND test_hello_world)