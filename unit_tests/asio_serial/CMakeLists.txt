#cmake_minimum_required(VERSION 3.1)
project(TEST)

message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")

## Target
set(CMAKE_CXX_STANDARD 11)
set(TEST_SRCS main.cpp ${BASE_PROJECT_SRC_DIR}/AsyncSerial.cpp ${BASE_PROJECT_SRC_DIR}/BufferedAsyncSerial.cpp ${BASE_PROJECT_SRC_DIR}/Utilities.cpp ${BASE_PROJECT_SRC_DIR}/logger.cpp)
add_executable(async ${TEST_SRCS})
target_include_directories(async PRIVATE ${BASE_PROJECT_LIB_DIR} ${BASE_PROJECT_INC_DIR})

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(async ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(async ${CMAKE_THREAD_LIBS_INIT} serial_support)
