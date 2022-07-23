cmake_minimum_required(VERSION 3.10)

#set the project name
project(KEngineOpenGL)


add_library(KEngineOpenGL STATIC 
OpenGLUtils.h
OpenGLUtils.cpp
ShaderFactory.h
ShaderFactory.cpp
SpriteRenderer.h
SpriteRenderer.cpp
TextureFactory.h
TextureFactory.cpp
)
add_subdirectory(SOIL)
add_subdirectory(glad)
find_package(OpenGL REQUIRED)
target_link_libraries(KEngineOpenGL PRIVATE KEngineCore KEngine2D soil glad ${OPENGL_LIBRARIES})
target_include_directories(KEngineOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(KEngineOpenGL PRIVATE SOIL/inc)