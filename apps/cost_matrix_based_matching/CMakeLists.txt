find_package( OpenCV REQUIRED )
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)


add_executable(run_localizer_cost_matrix_based cost_matrix_based_matching.cpp)
target_link_libraries( run_localizer_cost_matrix_based
    online_localizer
    cost_matrix_database
    successor_manager
    full_matrix_visualizer
    config_parser
    ${OpenCV_LIBS}
)
