
SET(QT_HEADERS visualizer.h localization_viewer.h match_viewer.h)
SET(QT_SOURCES visualizer.cpp localization_viewer.cpp match_viewer.cpp)
qt5_wrap_cpp(QT_SOURCES ${QT_HEADERS})

add_library(visualizer ${QT_SOURCES} ${QT_HEADERS})
target_link_libraries(visualizer 
	Qt5::Widgets 
	Qt5::Core 
	node
)

add_library(full_matrix_visualizer
    full_matrix_visualizer.cpp 
)
target_link_libraries(full_matrix_visualizer   
    cost_matrix_database
    node
    ${OpenCV_LIBS}
)