
find_package( OpenCV REQUIRED )
if( OpenCV_FOUND)
	include_directories( ${OpenCV_INCLUDE_DIRS} )

	add_executable(create_cost_matrix create_cost_matrix.cpp)
	target_link_libraries(create_cost_matrix
		online_database
		config_parser
		${OpenCV_LIBS}
	)

endif()