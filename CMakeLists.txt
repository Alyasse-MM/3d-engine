cmake_minimum_required(VERSION 3.10)

# 1. Project name
project(3d-engine)

set(CMAKE_CXX_STANDARD 17)

# 2. Find SFML 3 (CRITICAL FIX: Capitalize 'Graphics', 'Window', 'System')
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

include_directories(src)
file(GLOB SOURCES "src/*.cpp" "src/*.h")

add_executable(3d-engine ${SOURCES})

# 3. Link against the new SFML 3 targets
target_link_libraries(3d-engine SFML::Graphics SFML::Window SFML::System)