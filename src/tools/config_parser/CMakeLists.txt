

add_library(config_parser config_parser.cpp)
target_link_libraries(config_parser
	yaml-cpp
)