#cmake_minimum_required(VERSION 3.1)
project(GPSTEST)

## Target
set(CMAKE_CXX_STANDARD 11)
set(TEST_SRCS main.cpp 
	${BASE_PROJECT_SRC_DIR}/AsyncSerial.cpp 
	${BASE_PROJECT_SRC_DIR}/BufferedAsyncSerial.cpp 
	${BASE_PROJECT_SRC_DIR}/Utilities.cpp 
	${BASE_PROJECT_SRC_DIR}/logger.cpp
	${BASE_PROJECT_SRC_DIR}/NMEAGPS.cpp
	)
add_executable(gpstest ${TEST_SRCS})
target_include_directories(gpstest PRIVATE ${BASE_PROJECT_LIB_DIR} ${BASE_PROJECT_INC_DIR})

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(gpstest ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(gpstest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gpstest serial_support)
target_link_libraries(gpstest NMEAParserLib)
target_compile_definitions(gpstest PRIVATE _DEBUG)
