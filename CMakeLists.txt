cmake_minimum_required(VERSION 3.24)
project(BpAutoLayout C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

set(CONAN_PROVIDER_URL "https://raw.githubusercontent.com/conan-io/cmake-conan/develop2/conan_provider.cmake")
set(CONAN_PROVIDER_PATH "${CMAKE_BINARY_DIR}/conan_provider.cmake")

if(NOT EXISTS "${CONAN_PROVIDER_PATH}")
    message(STATUS "Downloading Conan provider from ${CONAN_PROVIDER_URL}...")
    file(DOWNLOAD "${CONAN_PROVIDER_URL}" "${CONAN_PROVIDER_PATH}")
endif()

message(STATUS "Triggering Conan installation...")
execute_process(
    COMMAND conan install ${CMAKE_SOURCE_DIR} --output-folder=${CMAKE_BINARY_DIR} --build=missing -s build_type=${CMAKE_BUILD_TYPE} -pr:b=default -pr:h=default
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE CONAN_RET
)

if(NOT CONAN_RET EQUAL 0)
    message(FATAL_ERROR "Conan failed to install dependencies.")
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

set(SFML_DIR "${CMAKE_BINARY_DIR}")
list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
find_package(jsoncpp REQUIRED)
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System JsonCpp::JsonCpp)

