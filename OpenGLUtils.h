//
//  OpenGLUtils.h
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/26/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//

#pragma once

#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define NOT_GLAD
        #include <OpenGLES/ES3/gl.h>
    #else
        #include "glad/glad.h"
    #endif
#elif(_WIN32)
#include "glad/glad.h"
#else
#include <glad/glad.h>
#endif


#define CHECK_GL_ERROR() CheckGLError();

namespace KEngineOpenGL {
    
    struct Vertex
    {
        float Position[3];
        float Color[4];
        float Texture[2];
    };
    GLuint UploadModel(const KEngineOpenGL::Vertex* vertics, int numVertics, const GLushort *indices, int numIndices);
    GLuint UploadVertices(const Vertex* vertices, int numVertices);
    GLuint UploadIndices(const GLubyte* indices, int numIndices);
    void CheckGLError();

    void DeleteVAO(GLuint vaoId);
    void DeleteBuffer(GLuint bufferId);
#ifndef NOT_GLAD
	void InitializeGlad(GLADloadproc loadProc);
#endif //NOT_GLAD
    
    
}



