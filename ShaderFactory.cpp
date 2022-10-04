//
//  ShaderFactory.cpp
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/26/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//


#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES3/gl.h>
    #else
        #include <glad/glad.h>
    #endif
#elif(_WIN32)
    #include "glad/glad.h"
#endif
#include "ShaderFactory.h"
#include "TextFile.h"
#include "OpenGLUtils.h"

#include <assert.h>
#include <string>
#include <iostream>

void KEngineOpenGL::ShaderFactory::Init()
{
    mVertexShaders.clear();
    mFragmentShaders.clear();
    mShaderPrograms.clear();
}

void KEngineOpenGL::ShaderFactory::CreateShaderProgram(KEngineCore::StringHash name, const std::string_view & vertexShaderFilename, const std::string_view & fragmentShaderFilename)
{
    KEngineCore::StringHash vertexHash(vertexShaderFilename);
    KEngineCore::StringHash fragmentHash(fragmentShaderFilename);
    
    if (mVertexShaders.find(vertexHash) == mVertexShaders.end()) {
        mVertexShaders[vertexHash] = CompileShader(vertexShaderFilename, GL_VERTEX_SHADER);
    }
    
    if (mFragmentShaders.find(fragmentHash) == mFragmentShaders.end()) {
        mFragmentShaders[fragmentHash] = CompileShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    }
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, mVertexShaders[vertexHash]);
    glAttachShader(programHandle, mFragmentShaders[fragmentHash]);
    
    glBindAttribLocation(programHandle, 0, "Position");
    glBindAttribLocation(programHandle, 1, "VertexColor");
    glBindAttribLocation(programHandle, 2, "TextureCoordinates");
    
    glLinkProgram(programHandle);
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        std::string messageString = messages;
        //Log(messageString);
        assert(false);
        exit(1);
    }
    ShaderProgram &program = mShaderPrograms[name];
    glUseProgram(programHandle);
    program.programHandle = programHandle;
    
    program.positionAttribute = glGetAttribLocation(programHandle, "Position");
    program.colorAttribute = glGetAttribLocation(programHandle, "VertexColor");
    program.textureAttribute = glGetAttribLocation(programHandle, "TextureCoordinates");
    program.projectionMatrixUniform = glGetUniformLocation(programHandle, "Projection");
    program.modelMatrixUniform = glGetUniformLocation(programHandle, "Model");
	program.textureUniform = glGetUniformLocation(programHandle, "Texture");
	CheckGLError();
}

const KEngineOpenGL::ShaderProgram* KEngineOpenGL::ShaderFactory::GetShaderProgram(KEngineCore::StringHash name)
{
	assert(mShaderPrograms.find(name) != mShaderPrograms.end());
    return &mShaderPrograms[name];
}

GLuint KEngineOpenGL::ShaderFactory::CompileShader(const std::string_view &shaderFilename, GLenum shaderType)
{
    KEngineCore::TextFile file;
    file.LoadFromFile(shaderFilename, ".glsl");
    GLuint shaderHandle = glCreateShader(shaderType);
    
    const std::string& shaderString = file.GetContents();
    GLint shaderStringLength = (GLint)shaderString.length();
    const char* sourcesArray[1] = {shaderString.c_str()};
    glShaderSource(shaderHandle, 1, sourcesArray, &shaderStringLength);
    glCompileShader(shaderHandle);
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        std::cerr << "Failed to compile shader: " << shaderFilename << std::endl;
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        std::string messageString = messages;
        std::cerr << messageString << std::endl;
        assert(false);
        exit(1);
    }
    return shaderHandle;
}
