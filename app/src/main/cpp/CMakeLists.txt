cmake_minimum_required(VERSION 3.4.1)
# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform         status
#   (0 11)           ES2/ES3 not supported
#   [11, 18)         ES2 only, for ES3, app do dynamic load
#   [18, 24)         ES2 & ES3
#   [24, infinite)   ES2 & ES3 & Vulkan


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")

set(OPENGL_LIB GLESv2)

add_library(gles2jni SHARED
            gles2jni.h
            gles2jni.cpp
            Game.cpp
            Game.h
            Renderer.h
            Renderer.cpp)

# Include libraries needed for gles2jni lib
target_link_libraries(gles2jni
                      ${OPENGL_LIB}
                      android
                      EGL
                      log
                      m)
