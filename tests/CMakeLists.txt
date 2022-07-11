add_executable(ecs_tests ecs_tests.cpp)
target_include_directories(ecs_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ecs_tests PRIVATE Catch2::Catch2WithMain retroenginelib)

add_executable(event_tests event_tests.cpp)
target_include_directories(event_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(event_tests PRIVATE Catch2::Catch2WithMain retroenginelib)

add_executable(input_manager_tests input_manager_tests.cpp)
target_include_directories(input_manager_tests PRIVATE ${CMAKE_SOURCE_DIR}/include ${SFML_INCLUDE})
target_link_libraries(input_manager_tests PRIVATE Catch2::Catch2WithMain retroenginelib)
