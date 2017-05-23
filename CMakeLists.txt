cmake_minimum_required(VERSION 2.8)
project(online_place_recognition)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")
# ubuntu 16.04
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")


set(CMAKE_BUILD_TYPE Debug)
# Enable Release mode
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -O3")
# Enable Debug mode
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")

find_package( OpenCV REQUIRED )
include_directories(src)
add_subdirectory(src)

add_subdirectory(apps)

# GOOGLE TESTING PART
add_subdirectory(gtest)

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
file(GLOB TEST_SRC_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME}
		online_database
		cost_matrix_database
		successor_manager
		online_localizer
        pthread
        gtest
        gtest_main
        ${OpenCV_LIBS}
)
add_test(NAME ${PROJECT_NAME}
         COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_TEST_NAME})
