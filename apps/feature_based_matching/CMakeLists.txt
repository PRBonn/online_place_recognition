find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable(run_localizer_feature_based feature_based_matching.cpp)
target_link_libraries(run_localizer_feature_based
    online_localizer
    online_database
    successor_manager
    visualizer
    config_parser
    yaml-cpp
)