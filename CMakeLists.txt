cmake_minimum_required (VERSION 3.20)
project ("StrongTypes")

option(BUILD_TEST "Build Tests for ${PROJECT_NAME}" OFF)
option(BUILD_EXAMPLES "Build Examples for ${PROJECT_NAME}" OFF)

# Add source to this project's executable.
set(PCH_FILE "src/pch.h")
set(SRC_FILES  "include/StrongTypes/StrongTypes.h")

add_library (StrongTypes INTERFACE ${SRC_FILES} ${PCH_FILE})

target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_precompile_headers(${PROJECT_NAME} INTERFACE ${PCH_FILE})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if(${BUILD_EXAMPLES}) 
    add_executable(example examples/example.cpp examples/example.h)
    set_property(TARGET example PROPERTY CXX_STANDARD 20)
    target_link_libraries(example PRIVATE ${PROJECT_NAME})
endif()

if(${BUILD_TEST})
    enable_testing()
    find_package(GTest CONFIG REQUIRED)

    add_executable(${PROJECT_NAME}_tests tests/StrongTypesTest.cpp )
    set_property(TARGET ${PROJECT_NAME}_tests PROPERTY CXX_STANDARD 20)

    target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME} GTest::gtest GTest::gtest_main)
endif()