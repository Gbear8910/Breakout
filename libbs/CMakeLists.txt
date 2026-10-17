project(libbs)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")


add_executable(${PROJECT_NAME}_test)
target_sources(${PROJECT_NAME}_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" )
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME})
