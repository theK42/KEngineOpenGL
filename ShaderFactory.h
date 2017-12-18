//
//  ShaderFactory.h
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/26/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//

#pragma once

#if TARGET_OS_IPHONE
#include <OpenGLES/gltypes.h>
#else #ifdef WIN32
#include <Windows.h>
#include <gl/GL.h>
#endif

#include "StringHash.h"
#include <map>
#include <string>

namespace KEngineOpenGL {
    struct ShaderProgram
    {
        GLuint programHandle;
        GLint positionAttribute;
        GLint colorAttribute;
        GLint textureAttribute;
        GLuint projectionMatrixUniform;
        GLuint modelMatrixUniform;
    };
    
    class ShaderFactory
    {
    public:
        void Init();
        void CreateShaderProgram(KEngineCore::StringHash name, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
        const KEngineOpenGL::ShaderProgram * GetShaderProgram(KEngineCore::StringHash name);
        
    private:
        GLuint CompileShader(const std::string& shaderFilename, GLenum shaderType);
        
        std::map<KEngineCore::StringHash, GLuint> mVertexShaders;
        std::map<KEngineCore::StringHash, GLuint> mFragmentShaders;
        std::map<KEngineCore::StringHash, ShaderProgram> mShaderPrograms;
    };
}
