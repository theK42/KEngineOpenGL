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
TextRenderer.h
TextRenderer.cpp
FontFactory.h
FontFactory.cpp
)
add_subdirectory(SOIL)
add_subdirectory(glad)

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
	target_link_libraries(KEngineOpenGL PUBLIC glad)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	target_link_libraries(KEngineOpenGL PUBLIC glad EGL GLESv3)
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(KEngineOpenGL PUBLIC glad ${OPENGL_LIBRARIES})
endif()
target_link_libraries(KEngineOpenGL PUBLIC glad ${OPENGL_LIBRARIES})
target_link_libraries(KEngineOpenGL PRIVATE KEngineCore KEngine2D soil)
target_include_directories(KEngineOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(KEngineOpenGL PRIVATE SOIL/inc)
