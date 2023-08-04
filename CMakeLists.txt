cmake_minimum_required (VERSION 3.20)
project ("StrongTypes")

option(BUILD_TEST "Build Tests for ${PROJECT_NAME}" OFF)
option(BUILD_EXAMPLES "Build Examples for ${PROJECT_NAME}" OFF)
option(SHARED_LIB "Build as shared Library" OFF)

# Add source to this project's executable.
set(PCH_FILE "src/pch.h")
set(SRC_FILES "src/StrongTypes.cpp" "include/StrongTypes/StrongTypes.h")

if(${SHARED_LIB})
    add_library (StrongTypes SHARED ${SRC_FILES} ${PCH_FILE})
else()
    add_library (StrongTypes STATIC ${SRC_FILES} ${PCH_FILE})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_FILE})
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