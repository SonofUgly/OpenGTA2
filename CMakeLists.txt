cmake_minimum_required(VERSION 2.8)

project(OpenGTA2)

option(UseValgrind "UseValgrind" OFF)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}")

find_package(OpenGL REQUIRED)

enable_testing()

# The version number.
set(OPENGTA2_VERSION_MAJOR 0)
set(OPENGTA2_VERSION_MINOR 1)
set(OPENGTA2_VERSION_PATCH 0)

# Enable Visual Studio solution "folders"
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
  #message("RELEASE set")
endif ()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/opengta2
    ${OPENGL_INCLUDE_DIRS}
)

SET(opengta2_src
opengta2/anim.cpp
opengta2/anim.h
opengta2/camera.cpp
opengta2/camera.h
opengta2/chunkload.cpp
opengta2/chunkload.h
opengta2/clientlist.cpp
opengta2/clientlist.h
opengta2/cmain.cpp
opengta2/collision.cpp
opengta2/collision.h
opengta2/console.cpp
opengta2/console.h
opengta2/convar.cpp
opengta2/convar.h
opengta2/darray.h
opengta2/debugtest.cpp
opengta2/draw.cpp
opengta2/draw.h
opengta2/font.cpp
opengta2/font.h
opengta2/format_rmp.h
opengta2/game.cpp
opengta2/game.h
opengta2/graphics.cpp
opengta2/graphics.h
opengta2/input.cpp
opengta2/input.h
opengta2/input_keys.h
opengta2/log.cpp
opengta2/log.h
opengta2/main.cpp
opengta2/malloc.cpp
opengta2/malloc.h
opengta2/map.cpp
opengta2/map.h
opengta2/map_geom.cpp
opengta2/map_geom.h
opengta2/map_geom_cl.cpp
opengta2/map_geom_cl.h
opengta2/map_render.cpp
opengta2/map_render.h
opengta2/matrix.cpp
opengta2/matrix.h
opengta2/message.h
opengta2/network.cpp
opengta2/network.h
opengta2/opengta.h
opengta2/opengta_cl.h
opengta2/opengta_sv.h
opengta2/ped.cpp
opengta2/ped.h
opengta2/ped_mgr.cpp
opengta2/ped_mgr.h
opengta2/ped_render.cpp
opengta2/ped_render.h
opengta2/perfhud.cpp
opengta2/perfhud.h
opengta2/random.h
opengta2/screen.cpp
opengta2/screen.h
opengta2/smain.cpp
opengta2/texture_atlas.cpp
opengta2/thread.cpp
opengta2/thread.h
opengta2/timer.cpp
opengta2/timer.h
opengta2/utf8.cpp
opengta2/utf8.h
opengta2/vbuffer.cpp
opengta2/vbuffer.h
opengta2/vector.cpp
opengta2/vector.h
)

# TODO: Tools etc

#if(WIN32)
#SET(WIN32_RESOURCES_SRC
#    rsc/Resource.rc
#    rsc/resource.h
#    rsc/app.ico)
#endif()

add_executable(OpenGTA2 MACOSX_BUNDLE
    ${opengta2_src}
)

target_link_libraries(OpenGTA2 ${OPENGL_LIBRARIES})

if(WIN32)
  target_link_libraries(OpenGTA2 wsock32 ws2_32)
endif()

# Generate header containing window title/version information
#git_shorttag(GIT_SHORT_REV)
#set(OPENGTA2_VERSION "OpenGTA2 v${OPENGTA2_VERSION_MAJOR}.${OPENGTA2_VERSION_MINOR}.${OPENGTA2_VERSION_PATCH} rev(${GIT_SHORT_REV})")
#configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/packaging/opengta2_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/packaging/opengta2_version.h)

SET(OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if (MSVC)
    # Build cpp files on all cores
  
    # TODO: Add /WX when warnings are fixed
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /Wall /wd4503 /wd4351 /wd4514 /wd4350 /wd4710 /wd4571 /wd4820 /wd4668 /wd4626 /wd4625 /wd4365 /wd4640 /wd4628 /wd4061 /wd4826 /wd4127 /wd4711 /wd4738")
    
    # Warnings added in VS2013 and later
    if(MSVC_VERSION GREATER 1800)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd5026 /wd5027")
    endif()
else()
    set_source_files_properties(${stk_src} PROPERTIES COMPILE_FLAGS "${OLD_CMAKE_CXX_FLAGS} -Wno-sign-compare")
    set_source_files_properties(${nanovg_src} PROPERTIES COMPILE_FLAGS "${OLD_CMAKE_CXX_FLAGS} -Wno-unused-result")

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # C++14 by default
        # TODO: -Werror
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fPIC -Wall")
    else()
        # Enable C++14, you may need to use -std=c++0x if using an older gcc compiler
        # TODO:  -Werror
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC -Wall -pedantic")
        
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
    endif()
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
