message(STATUS "Creating sfml dependencies for build type ${CMAKE_BUILD_TYPE}")

add_library(audio SHARED IMPORTED)
set_target_properties(audio PROPERTIES 
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-audio-2.dll
  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-audio.lib
  IMPORTED_LOCATION_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-audio-d-2.dll
  IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-audio-d.lib
  IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
)

add_library(graphics SHARED IMPORTED)
set_target_properties(graphics PROPERTIES 
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-graphics-2.dll
  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-graphics.lib
  IMPORTED_LOCATION_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-graphics-d-2.dll
  IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-graphics-d.lib
  IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
)

add_library(network SHARED IMPORTED)
set_target_properties(network PROPERTIES 
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-network-2.dll
  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-network.lib
  IMPORTED_LOCATION_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-network-d-2.dll
  IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-network-d.lib
  IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
)

add_library(system SHARED IMPORTED)
set_target_properties(system PROPERTIES 
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-system-2.dll
  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-system.lib
  IMPORTED_LOCATION_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-system-d-2.dll
  IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-system-d.lib
  IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
)

add_library(window SHARED IMPORTED)
set_target_properties(window PROPERTIES 
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-window-2.dll
  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-window.lib
  IMPORTED_LOCATION_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/bin/sfml-window-d-2.dll
  IMPORTED_IMPLIB_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/lib/sfml-window-d.lib
  IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
)

set(SFML_INCLUDE ${CMAKE_SOURCE_DIR}/thirdparty/SFML-2.5.1-64bit/SFML-2.5.1/include)
set(SFML_LIBS audio graphics system window network)