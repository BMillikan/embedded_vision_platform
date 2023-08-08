project(serial_support_example)

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} serial_support)
target_link_libraries(${PROJECT_NAME} serial_support)
target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_PROJECT_LIB_DIR} ${BASE_PROJECT_INC_DIR})

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${BASE_PROJECT_BIN_DIR}
)
