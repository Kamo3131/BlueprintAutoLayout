cmake_minimum_required(VERSION 4.0.0)

project(BpAutoLayout LANGUAGES C CXX)
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    add_compile_definitions(SFML_STATIC)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

include(FetchContent)

set(SFML_STATIC_LIBRARIES ON)
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_TESTS OFF)
set(SFML_BUILD_DOC OFF)
set(SFML_VERSION 3.0.2)


FetchContent_Declare(
    SFML
    URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)

set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "")
set(JSONCPP_VERSION 1.9.6)

FetchContent_Declare(
    jsoncpp
    URL "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/${JSONCPP_VERSION}.zip"
)
FetchContent_MakeAvailable(SFML jsoncpp)


file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${jsoncpp_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    jsoncpp_static
)
add_dependencies(${PROJECT_NAME} jsoncpp_static)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glu32 winmm gdi32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE 
            X11 Xrandr Xcursor Xi udev GL freetype
        )
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

enable_testing()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()