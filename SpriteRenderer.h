//
//  SpriteRenderer.hpp
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 11/24/17.
//  Copyright © 2017 Kelson Hootman. All rights reserved.
//

#pragma once

#include <list>
#include "Transform2D.h"
#include "Renderer2D.h"
#include "StringHash.h"


#if __APPLE__
    #include <OpenGLES/gltypes.h>
#elif _WIN32
    #include <Windows.h>
    #include <glad/glad.h>
    #include <gl/GL.h>
#endif


namespace KEngineOpenGL
{
    struct ShaderProgram;
    
    struct Sprite
    {
        int width;
        int height;
        const ShaderProgram * shaderProgram;
        GLint vertexBuffer;
        GLint indexBuffer;
        GLuint vertexArrayObject;
        int indexCount;
		GLuint texture;
		
        /*
         shader name
        vertexShader name for regeneration
        fragmentShader  name for regeneration
        uniform : value maps for various types including textures
         */
    };
    
    class SpriteRenderer;
    
    class SpriteGraphic
    {
    public:
        SpriteGraphic();
        ~SpriteGraphic();
        void Init(SpriteRenderer * renderer, Sprite const * sprite, KEngine2D::Transform const * transform);
        void Deinit();
        Sprite const * GetSprite() const;
        void SetSprite(Sprite const * sprite);
        KEngine2D::Transform const * GetTransform() const;
        
    protected:
        Sprite const *                  mSprite;
        KEngine2D::Transform const *    mTransform;
        SpriteRenderer *                mRenderer;
    };
    
    class SpriteRenderer : public KEngine2D::Renderer
    {
    public:
        SpriteRenderer();
        ~SpriteRenderer();
        void Init(int width, int height);
        void Deinit();
        void Render() const;
        void AddToRenderList(SpriteGraphic * cursesGraphic);
        void RemoveFromRenderList(SpriteGraphic * cursesGraphic);
        int GetWidth() const;
        int GetHeight() const;
    protected:
        
        std::list<SpriteGraphic *>    mRenderList;
        bool                          mInitialized;
        int                           mWidth;
        int                           mHeight;
        float                         mProjection[4][4];
    };
}
