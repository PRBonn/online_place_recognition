
add_executable(estimate_non_match_cost  
    estimate_non_match_cost.cpp
    non_match_cost_estimator.cpp
)
target_link_libraries(estimate_non_match_cost 
	feature_factory
)