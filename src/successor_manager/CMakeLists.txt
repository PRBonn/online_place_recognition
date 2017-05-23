add_library(node node.cpp)
add_library(successor_manager successor_manager.cpp)
target_link_libraries(successor_manager
	node 
)
