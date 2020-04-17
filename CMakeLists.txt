cmake_minimum_required(VERSION 3.7)
project(RaytracerC C)

set(CMAKE_C_STANDARD 99)

find_library(PTHREAD_LIB pthread)
IF (PTHREAD_LIB)
    ADD_DEFINITIONS(-D_RAYTRACERC_PTHREAD)
ELSEIF (WIN32)
    ADD_DEFINITIONS(-D_RAYTRACERC_WIN32)
ENDIF ()

add_library(lodepng lib/lodepng/lodepng.h lib/lodepng/lodepng.c)

add_executable(RaytracerC src/main.c src/vect/rgb.c src/vect/rgb.h src/vect/vector3.c src/vect/vector3.h src/objects/objects.h src/raycast.c src/raycast.h src/objects/sphere.c src/objects/plane.c src/objects/objects.c src/objects/light.c src/objects/light.h src/camera.c src/camera.h src/image.c src/image.h src/renderer.c src/renderer.h src/objects/scene.c src/objects/scene.h src/threading/thread.c src/threading/thread.h src/threading/pthread.c src/threading/win.c)
target_link_libraries(RaytracerC lodepng)

IF (PTHREAD_LIB)
    target_link_libraries(RaytracerC pthread)
ENDIF ()

find_library(M_LIB m)
if (M_LIB)
    target_link_libraries(RaytracerC m)
ENDIF ()