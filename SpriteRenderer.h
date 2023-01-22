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
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/gltypes.h>
    #else
        #include <glad/glad.h>
    #endif
#elif _WIN32
    #include <glad/glad.h>
#else
    #include <glad/glad.h>
#endif

namespace KEngineCore
{
    class TweenSystem;
}


namespace KEngineOpenGL
{
    struct ShaderProgram;
    
    struct Sprite
    {
        Sprite();
        ~Sprite();
        void Init();
        void Deinit();
        int width;
        int height;
        const ShaderProgram * shaderProgram;
        GLuint vertexBuffer;
        GLuint indexBuffer;
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
        void Init(SpriteRenderer * renderer, Sprite const * sprite, KEngine2D::Transform const * transform, int layer);
        void Deinit();
        Sprite const * GetSprite() const;
        void SetSprite(Sprite const * sprite);
        KEngine2D::Transform const * GetTransform() const;
        float GetAlpha() const;
        void SetAlpha(float alpha);

        int GetLayer();

        static const char MetaName[];
   
    protected:
        Sprite const*                   mSprite{ nullptr };
        KEngine2D::Transform const *    mTransform{ nullptr };
        SpriteRenderer*                 mRenderer{ nullptr };
        float                           mAlpha{ 1.0f };
        int                             mLayer{ 0 };

        friend class SpriteLibrary;
    };
    
    class SpriteRenderer : public KEngine2D::Renderer //Eh...
    {
    public:
        SpriteRenderer();
        ~SpriteRenderer();
        void Init(int width, int height, int layers);
        void Deinit();
        void Render() const;
        void AddToRenderList(SpriteGraphic * spriteGraphic);
        void RemoveFromRenderList(SpriteGraphic * spriteGraphic);
        int GetWidth() const;
        int GetHeight() const;
    protected:
        
        std::vector<std::list<SpriteGraphic*>>  mRenderLists;
        bool                                    mInitialized{ false };
        int                                     mWidth;
        int                                     mHeight;
        float                                   mProjection[4][4];
    };

    class SpriteLibrary : public KEngineCore::LuaLibrary
    {
    public:
        SpriteLibrary();
        virtual ~SpriteLibrary() override;
        void Init(KEngineCore::LuaScheduler* scheduler, KEngineCore::TweenSystem* tweenSystem);
        void Deinit();
        virtual void RegisterLibrary(lua_State* luaState, char const* name = "sprites") override;

        const KEngineCore::LuaWrapping<SpriteGraphic>& GetSpriteWrapping() const;
    private:
        KEngineCore::LuaWrapping<SpriteGraphic>	mSpriteLuaWrapping;
        KEngineCore::LuaScheduler* mScheduler{ nullptr };
        KEngineCore::TweenSystem* mTweenSystem{ nullptr };
    };
}
