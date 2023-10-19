cmake_minimum_required (VERSION 3.5.0)

set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)

# Project name
project (gamebase)

if (WIN32)
  SET(SDL2_DIR "../SDL2/cmake")
  SET(SDL2_IMAGE_DIR "../SDL2_image/cmake")
endif()

find_package (SDL2 REQUIRED)
find_package (SDL2_IMAGE REQUIRED)

# Include SDL2 support for cmake

if (NOT DEFINED SDL2_INCLUDE_DIR)
  if (DEFINED SDL2_INCLUDE_DIRS)
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
  endif()
endif()
if (NOT DEFINED SDL2_IMAGE_INCLUDE_DIR)
  if (DEFINED SDL2_IMAGE_INCLUDE_DIRS)
    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_INCLUDE_DIRS})
   endif()
endif()

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

# Include header files
include_directories (graphics)

set(SOURCES
   ${PROJECT_SOURCE_DIR}/main.cpp
   ${PROJECT_SOURCE_DIR}/app/baseapp.cpp
   ${PROJECT_SOURCE_DIR}/app/baseapp.h
   ${PROJECT_SOURCE_DIR}/composite/composite.h
   ${PROJECT_SOURCE_DIR}/graphics/atlas.cpp
   ${PROJECT_SOURCE_DIR}/graphics/atlas.h
   ${PROJECT_SOURCE_DIR}/graphics/graphics.cpp
   ${PROJECT_SOURCE_DIR}/graphics/graphics.h   
   #${PROJECT_SOURCE_DIR}/snake/snake.h
   ${PROJECT_SOURCE_DIR}/ztyp/ztyp.h
   )
add_executable(${PROJECT_NAME} ${SOURCES})

if (NOT DEFINED SDL2_LIBRARIES)
  set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

if (NOT DEFINED SDL2_IMAGE_LIBRARIES)
  set(SDL2_IMAGE_LIBRARIES "SDL2_image::SDL2_image")
endif()

target_link_libraries(${PROJECT_NAME}
   ${SDL2_LIBRARIES}
   ${SDL2_IMAGE_LIBRARIES}
   )

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

if(WIN32)
    get_target_property(SDL2_LIBRARY SDL2::SDL2 IMPORTED_LOCATION)
    get_filename_component(SDL2_LIBRARY_NAME "${SDL2_LIBRARY}" NAME)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        MAIN_DEPENDENCY "${SDL2_LIBRARY}"
        BYPRODUCTS "${SDL2_LIBRARY_NAME}"
        COMMENT "Copying SDL2 DLL"
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY}" "${CMAKE_BINARY_DIR}/Debug/${SDL2_LIBRARY_NAME}"
    )

    get_target_property(SDL2_IMAGE_LIBRARY SDL2_image::SDL2_image IMPORTED_LOCATION)
    get_filename_component(SDL2_IMAGE_LIBRARY_NAME "${SDL2_IMAGE_LIBRARY}" NAME)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        MAIN_DEPENDENCY "${SDL2_IMAGE_LIBRARY}"
        BYPRODUCTS "${SDL2_IMAGE_LIBRARY_NAME}"
        COMMENT "Copying SDL2_image DLL"
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY}" "${CMAKE_BINARY_DIR}/Debug/${SDL2_IMAGE_LIBRARY_NAME}"
    )    
endif()