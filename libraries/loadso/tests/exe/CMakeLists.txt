project(testexe)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE loadso)

target_compile_definitions(${PROJECT_NAME} PRIVATE DLL_NAME="$<TARGET_FILE_NAME:testdll>")