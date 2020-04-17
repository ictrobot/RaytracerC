cmake_minimum_required(VERSION 3.7)
project(RaytracerC C)

set(CMAKE_C_STANDARD 99)

add_library(libattopng lib/libattopng/libattopng.h lib/libattopng/libattopng.c)

add_executable(RaytracerC src/main.c src/vect/rgb.c src/vect/rgb.h src/vect/vector3.c src/vect/vector3.h src/objects/objects.h src/raycast.c src/raycast.h src/objects/sphere.c src/objects/plane.c src/objects/objects.c src/objects/light.c src/objects/light.h src/camera.c src/camera.h src/image.c src/image.h src/renderer.c src/renderer.h src/objects/scene.c src/objects/scene.h)
target_link_libraries(RaytracerC m libattopng pthread)