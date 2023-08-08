project(serial_support)

## Sources
set(serial_support_SRCS
)

if(APPLE)
    # If OSX
    list(APPEND serial_support_SRCS list_ports/list_ports_osx.cc)
elseif(UNIX)
    # If unix
    list(APPEND serial_support_SRCS list_ports/list_ports_linux.cc)
else()
    # If windows
    list(APPEND serial_support_SRCS list_ports/list_ports_win.cc)
endif()

## Add serial support library
add_library(${PROJECT_NAME} STATIC ${serial_support_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_PROJECT_INC_DIR})

## Install executable
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${BASE_PROJECT_LIB_DIR}
	LIBRARY DESTINATION ${BASE_PROJECT_LIB_DIR}
	RUNTIME DESTINATION ${BASE_PROJECT_BIN_DIR}
)

## Install headers
#install(FILES include/serial/serial.h include/serial/v8stdint.h
#  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}/serial)
