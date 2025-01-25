cmake_minimum_required(VERSION 3.10)  # Ensure the minimum CMake version
project(Exercise)                    # Project name

enable_testing()
find_package(GTest REQUIRED)

# Specify C++ standard version
set(CMAKE_CXX_STANDARD 17)

# Add the source files for the project
file (GLOB SOURCES "./src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "main.cpp")
file (GLOB TESTS "./test/*.cpp")

set(CMAKE_BUILD_TYPE Debug)

# Create the executable from the source files
add_executable(ChatServer ${SOURCES} "./src/main.cpp")
add_executable(runTests ${TESTS} ${SOURCES})

# Include the directory where the header files are located
target_include_directories(ChatServer PRIVATE ${CMAKE_SOURCE_DIR})
find_package(Threads REQUIRED)
target_link_libraries(ChatServer PRIVATE Threads::Threads)
target_link_libraries(runTests GTest::GTest GTest::Main)

add_test(NAME your_test_name COMMAND runTests)

# If you had any additional libraries or dependencies, you would link them here
# target_link_libraries(ChatServer <other_libraries>)
