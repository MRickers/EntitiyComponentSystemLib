add_executable(ecs_tests ecs_tests.cpp "event_tests.cpp")
target_include_directories(ecs_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ecs_tests PRIVATE Catch2::Catch2WithMain gameenginelib)

add_executable(event_tests event_tests.cpp)
target_include_directories(event_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(event_tests PRIVATE Catch2::Catch2WithMain gameenginelib)
