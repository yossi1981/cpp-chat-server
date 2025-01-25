cmake_minimum_required(VERSION 3.10)  # Ensure the minimum CMake version
project(Exercise)                    # Project name

# Specify C++ standard version
set(CMAKE_CXX_STANDARD 17)

# Add the source files for the project
file (GLOB SOURCES "./src/*.cpp")

set(CMAKE_BUILD_TYPE Debug)

# Create the executable from the source files
add_executable(ChatServer ${SOURCES})

# Include the directory where the header files are located
target_include_directories(ChatServer PRIVATE ${CMAKE_SOURCE_DIR})
find_package(Threads REQUIRED)
target_link_libraries(ChatServer PRIVATE Threads::Threads)


# If you had any additional libraries or dependencies, you would link them here
# target_link_libraries(ChatServer <other_libraries>)
