#pragma once
#include "SpriteRenderer.h"
#include "OpenGLUtils.h"
#include <string_view>

namespace KEngineOpenGL
{
    struct ShaderProgram;
    struct Sprite;

    class TextRenderer;

    struct FixWidthBitmapFont
    {
        int mWidth;
        int mHeight;
        int mGlyphWidth;
        int mGlyphHeight;
        std::string_view glyphs;
        const ShaderProgram* shaderProgram;
        GLuint texture;

        /*
         shader name
        vertexShader name for regeneration
        fragmentShader  name for regeneration
        uniform : value maps for various types including textures
         */
    };

    class TextSprite
    {
    public:
        TextSprite();
        ~TextSprite();
        void Init(int width, int height, const KEngineOpenGL::ShaderProgram* shaderProgram, TextRenderer* renderer);
        void Init(std::string_view text, const FixWidthBitmapFont& font, const KEngineOpenGL::ShaderProgram* shaderProgram, TextRenderer* renderer);
        void Deinit();

        void Resize(int width, int height);
        void RenderText(std::string_view text, const FixWidthBitmapFont& font);

        Sprite& GetSprite();
        const Sprite& GetSprite() const;
        
        const float* GetProjection() const;
        TextRenderer* GetRenderer() const;
    private:
        Sprite  mSprite;
        GLuint  mFramebuffer{ 0 };
        float   mProjection[4][4];
        TextRenderer*   mTextRenderer;
    };

    class TextRenderer
    {
    public:
        TextRenderer();
        ~TextRenderer();
        void Init(int startingBufferSize = 256, int maxBufferSize = -1);
        void Deinit();

        void RenderText(TextSprite* target, std::string_view text, const FixWidthBitmapFont& font);
        int GetTextWidth(std::string_view text, const FixWidthBitmapFont& font);
        int GetTextHeight(std::string_view text, const FixWidthBitmapFont& font);
    private:

        void Resize(int newBufferSize);

        int mUsedSize{ 0 };
        int mBufferSize;
        int mMaxBufferSize;

        std::vector< KEngineOpenGL::Vertex> mVertexBufferVector;
        std::vector< GLshort>                mIndexBufferVector;

        GLuint  mBuffers[2];
        GLuint  mVertexArrayObjectHandle{ 0 };

    };

}
