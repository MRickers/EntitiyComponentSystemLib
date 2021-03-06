cmake_minimum_required(VERSION 3.10)

project(retroengine VERSION 0.1.0 LANGUAGES CXX)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
# GS	- checks buffer security
# Gs	- Controles stack probes
# GT	- Using thread-local storage
# guard:cf	- Adds control flow guard security checks
set(COMPILE_OPTIONS /W4 /GT /GS /Gs /guard:cf $<$<CONFIG:Debug>:/RTC1> $<$<CONFIG:Debug>:/fsanitize=address>)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)

include(InstallRequiredSystemLibraries)

elseif(UNIX)
# -ggdb 				- Produce debugging information for use by GDB
# -D-FORTIFY-SOURCE 	- Run-time buffer overflow detection
# -Wformat-security 	- Checks for security problems
set(COMPILE_OPTIONS -Wall -Wextra -Werror -fstack-protector-all -Wno-unused-value -Wformat-security $<$<CONFIG:Debug>:-ggdb> $<$<CONFIG:Release>:-D_FORTIFY_SOURCE=2>)
set(GCC_FS_LIB "stdc++fs")

endif()

add_compile_options(${COMPILE_OPTIONS})

if(UNIX)
set(LIBS ${GCC_FS_LIB})
endif()

include("link_sfml.cmake")

option(build_examples "Building examples" ON)
option(build_tests "Buildingtests" ON)


add_library(retroenginelib STATIC
src/core/entity_manager.cpp
src/core/component_layout.cpp

src/utils/clock_chrono.cpp
src/event/communicator.cpp
)
target_include_directories(retroenginelib PRIVATE include ${SFML_INCLUDE})
target_link_libraries(retroenginelib ${LIBS} ${SFML_LIBS})

if(build_examples)
message(STATUS "Building ecs examples")

add_executable(cube_example examples/cube_example.cpp)
target_include_directories(cube_example PRIVATE include)
target_link_libraries(cube_example ${LIBS})

endif()

if(build_tests)
message(STATUS "Building ecs tests")
enable_testing()
add_subdirectory(Catch2)
add_subdirectory(tests)
endif()