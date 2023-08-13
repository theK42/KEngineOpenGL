//
//  OpenGLUtils.cpp
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/26/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//
#include "OpenGLUtils.h"
#include <assert.h>
#include <iostream>
#include <vector>

GLuint KEngineOpenGL::UploadModel(const KEngineOpenGL::Vertex* vertices, int numVertices, const GLushort *indices, int numIndices)
{
    GLuint vao, vbo[2];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);
    
    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(KEngineOpenGL::Vertex), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);

    glDeleteBuffers(2, vbo);

    return vao;
}

GLuint KEngineOpenGL::UploadVertices(const KEngineOpenGL::Vertex* vertices, int numVertices)
{
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(KEngineOpenGL::Vertex), vertices, GL_STATIC_DRAW);
    return vertexBuffer;
}

GLuint KEngineOpenGL::UploadIndices(const GLubyte* indices, int numIndices)
{
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLubyte), indices, GL_STATIC_DRAW);
    return indexBuffer;
}

void KEngineOpenGL::CheckGLError()
{
	GLenum errCode = glGetError();
	const GLubyte *errString;
    if (errCode != GL_NO_ERROR) {
        while (errCode != GL_NO_ERROR) {
            std::string error;
        
            switch(errCode) {
                case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION"; break;
            }
        
            std::cerr << "CheckGLError -- GL_" << error.c_str() << std::endl;
            errCode = glGetError();
        }
        assert(false);
        exit(1);
	}
}

void KEngineOpenGL::DeleteVAO(GLuint vaoId)
{
    glDeleteVertexArrays(1, &vaoId);
}

void KEngineOpenGL::DeleteBuffer(GLuint bufferId)
{
    glDeleteBuffers(1, &bufferId);
}

static std::vector<GLint> framebufferStack;

void KEngineOpenGL::PushFramebuffer(GLint bufferId)
{
    glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
    framebufferStack.push_back(bufferId);
}

void KEngineOpenGL::PopFramebuffer()
{
    framebufferStack.pop_back();
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferStack.back());
}


#ifndef NOT_GLAD
void KEngineOpenGL::InitializeGlad(GLADloadproc loadProc)
{
    if (loadProc) {
        gladLoadGLLoader(loadProc);
    }
    else {
        gladLoadGL();
    }
}
#endif //NOT_GLAD
