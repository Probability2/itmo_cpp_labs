add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE weather)
target_link_libraries(${PROJECT_NAME} PRIVATE argparser)
target_link_libraries(${PROJECT_NAME} PRIVATE string_argument_class)
target_link_libraries(${PROJECT_NAME} PRIVATE int_argument_class)
target_link_libraries(${PROJECT_NAME} PRIVATE flag_class)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
