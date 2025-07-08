
cmake_minimum_required(VERSION 4.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:nlohmann/json@3.12.0")

project(snowcrash-project 
	LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(snowcrash)
add_subdirectory(editor)
add_subdirectory(testing)

