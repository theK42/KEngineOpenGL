//
//  SpriteRenderer.cpp
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/24/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//
#include "SpriteRenderer.h"
#include <cassert>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include "OpenGLUtils.h"
#include "ShaderFactory.h"

KEngineOpenGL::SpriteGraphic::SpriteGraphic()
{
    mTransform = nullptr;
    mSprite = nullptr;
}

KEngineOpenGL::SpriteGraphic::~SpriteGraphic()
{
    Deinit();
}

void KEngineOpenGL::SpriteGraphic::Init( SpriteRenderer * renderer, Sprite const * sprite, KEngine2D::Transform const * transform )
{
    assert(transform != nullptr);
    assert(renderer != nullptr);
    mRenderer = renderer;
    mSprite = sprite;
    mTransform = transform;
    renderer->AddToRenderList(this);
}

void KEngineOpenGL::SpriteGraphic::Deinit()
{
    if (mRenderer != 0)
    {
        mRenderer->RemoveFromRenderList(this);
    }
    mTransform = nullptr;
    mSprite = nullptr;
    mRenderer = nullptr;
}

KEngineOpenGL::Sprite const * KEngineOpenGL::SpriteGraphic::GetSprite() const
{
    return mSprite;
}

void KEngineOpenGL::SpriteGraphic::SetSprite(const KEngineOpenGL::Sprite *sprite)
{
    assert(mTransform != nullptr); /// Initialized
    mSprite = sprite;
}

KEngine2D::Transform const * KEngineOpenGL::SpriteGraphic::GetTransform() const
{
    assert(mTransform != nullptr);
    return mTransform;
}

KEngineOpenGL::SpriteRenderer::SpriteRenderer()
{
    mInitialized = false;
}

KEngineOpenGL::SpriteRenderer::~SpriteRenderer()
{
    Deinit();
}

void KEngineOpenGL::SpriteRenderer::Init( int width, int height )
{
    assert(!mInitialized);
    //TODO:  initialize OpenGL stuff
    mWidth = width;
    mHeight = height;
    
    float left = 0;
    float right = width;
    float top = 0;
    float bottom = height;
    float near = -1;
    float far = 1;
    
    mProjection[0][0] = 2.0f / (right - left);
    mProjection[0][1] = 0.0f;
    mProjection[0][2] = 0.0f;
    mProjection[0][3] = 0.0f;
    
    mProjection[1][0] = 0.0f;
    mProjection[1][1] = 2.0f / (top - bottom);
    mProjection[1][2] = 0.0f;
    mProjection[1][3] = 0.0f;
    
    mProjection[2][0] = 0.0f;
    mProjection[2][1] = 0.0f;
    mProjection[2][2] = -2.0f / (far - near);
    mProjection[2][3] = 0.0f;
    
    mProjection[3][0] = -(right + left  ) / (right - left  );
    mProjection[3][1] = -(top   + bottom) / (top   - bottom);
    mProjection[3][2] = -(far   + near  ) / (far   - near  );
    mProjection[3][3] = 1.0f;
    
    mInitialized = true;
}

void KEngineOpenGL::SpriteRenderer::Deinit()
{
    mInitialized = false;
    mRenderList.clear();
}

void KEngineOpenGL::SpriteRenderer::Render() const
{
    assert(mInitialized);
    glViewport(0, 0, mWidth, mHeight);
    CHECK_GL_ERROR();
    glClearColor(0.0, 0, 0, 1.0);
    CHECK_GL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR();
    
    for (SpriteGraphic * graphic : mRenderList)
    {
        
        const Sprite * sprite = graphic->GetSprite();
        const KEngine2D::Matrix& modelMatrix = graphic->GetTransform()->GetAsMatrix();
        const ShaderProgram * shaderProgram = sprite->shaderProgram;
        glUseProgram(shaderProgram->programHandle);
        
        CHECK_GL_ERROR();
        glUniformMatrix4fv(shaderProgram->projectionMatrixUniform, 1, 0, &mProjection[0][0]);
        CHECK_GL_ERROR();
        glUniformMatrix4fv(shaderProgram->modelMatrixUniform, 1, 1, &modelMatrix.data[0][0]);
        
        CHECK_GL_ERROR();
        
        glBindVertexArray(sprite->vertexArrayObject);

        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, 0);
        CHECK_GL_ERROR();
    }
}

void KEngineOpenGL::SpriteRenderer::AddToRenderList( SpriteGraphic * spriteGraphic )
{
    assert(mInitialized);
    mRenderList.push_back(spriteGraphic);
}

void KEngineOpenGL::SpriteRenderer::RemoveFromRenderList( SpriteGraphic * spriteGraphic )
{
    assert(mInitialized);
    mRenderList.remove(spriteGraphic);
}

int KEngineOpenGL::SpriteRenderer::GetWidth() const {
    return mWidth;
}

int KEngineOpenGL::SpriteRenderer::GetHeight() const {
    return mHeight;
}

