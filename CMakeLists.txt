cmake_minimum_required(VERSION 3.19)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project( embedded_vision_platform VERSION 1.0  )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

set( BASE_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR} )
message(STATUS "Base project dir: ${BASE_PROJECT_DIR}")

set( BASE_PROJECT_LIB_DIR ${BASE_PROJECT_DIR}/lib )
message(STATUS "Base project lib dir: ${BASE_PROJECT_LIB_DIR}")

set( BASE_PROJECT_INC_DIR ${BASE_PROJECT_DIR}/inc )
message(STATUS "Base project inc dir: ${BASE_PROJECT_INC_DIR}")

set( BASE_PROJECT_SRC_DIR ${BASE_PROJECT_DIR}/src )
message( STATUS "Base project src dir: ${BASE_PROJECT_SRC_DIR}")

list( APPEND CMAKE_MODULE_PATH ${BASE_PROJECT_DIR}/cmake/Modules )

set( BASE_PROJECT_BIN_DIR ${BASE_PROJECT_DIR}/bin )
set( BASE_PROJECT_UNIT_TEST_DIR ${BASE_PROJECT_DIR}/unit_tests )
set( BASE_PROJECT_THIRD_PARTY_DIR ${BASE_PROJECT_DIR}/third_party )

# Set location of libadsb libraries and include files
set( ADSB_LIB_DIR ${BASE_PROJECT_THRID_PARTY_DIR}/libadsb )
set( ADSB_INC_DIR ${BASE_PROJECT_THIRD_PARTY_DIR}/libadsb )

# Set location of NMEAParse libraries and include files
set( NMEA_PARSER_LIB_DIR ${BASE_PROJECT_THIRD_PARTY_DIR}/NMEAParser/Software/build/NMEAParserLib )
set( NMEA_PARSER_INC_DIR ${BASE_PROJECT_THIRD_PARTY_DIR}/NMEAParser/Software/NMEAParserLib )

#target_include_directories( embedded_vision_platform PUBLIC
#	                    "${BASE_PROJECT_INC_DIR}" 
#			  )

set( CMAKE_BINARY_DIR ${BASE_PROJECT_BIN_DIR} )
set( CMAKE_CACHEFILE_DIR ${BASE_PROJECT_DIR}/build )

list(APPEND CMAKE_MODULE_PATH /usr/share/cmake-3.22/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)

# Build third party tools (if necessary)
add_subdirectory(third_party)
add_subdirectory(src/impl)
add_subdirectory(unit_tests)
