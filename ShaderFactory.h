//
//  ShaderFactory.h
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/26/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//

#pragma once


#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/gltypes.h>
    #else
        #include <glad/glad.h>
    #endif
#elif(_WIN32)
#include <glad/glad.h>
#else
#include <glad/glad.h>
#endif

#include "StringHash.h"
#include <map>
#include <vector>
#include <string>

namespace KEngineOpenGL {


    struct ShaderProgram
    {
        GLuint programHandle;
        GLint positionAttribute;
        GLint colorAttribute;
        GLint textureAttribute;
        GLint projectionMatrixUniform;
        GLint modelMatrixUniform;
		GLint textureUniform;
    };
    
    class ShaderFactory
    {
    public:
        void Init();
        void CreateShaderProgram(KEngineCore::StringHash name, const std::string_view& vertexShaderFilename, const std::string_view& fragmentShaderFilename);
        const KEngineOpenGL::ShaderProgram * GetShaderProgram(KEngineCore::StringHash name);
        
    private:
        GLuint CompileShader(const std::string_view& shaderFilename, GLenum shaderType);
        
        std::map<KEngineCore::StringHash, GLuint> mVertexShaders;
        std::map<KEngineCore::StringHash, GLuint> mFragmentShaders;
        std::map<KEngineCore::StringHash, ShaderProgram> mShaderPrograms;
    };
}
