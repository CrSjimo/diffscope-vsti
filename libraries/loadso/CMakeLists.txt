cmake_minimum_required(VERSION 3.16)

project(loadso)

if(NOT DEFINED LOADSO_BUILD_STATIC)
    set(LOADSO_BUILD_STATIC off)
endif()

if(NOT DEFINED LOADSO_BUILD_TESTS)
    set(LOADSO_BUILD_TESTS on)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

file(GLOB_RECURSE _src include/*.h src/*.h src/*.cpp)

if(LOADSO_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LOADSO_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE LOADSO_LIBRARY)
target_sources(${PROJECT_NAME} PRIVATE ${_src})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE shlwapi)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()

if(LOADSO_BUILD_TESTS)
    add_subdirectory(tests)
endif()