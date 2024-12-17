add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE parsing)
target_link_libraries(${PROJECT_NAME} PRIVATE sandpile_model)
target_link_libraries(${PROJECT_NAME} PRIVATE bmp_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
