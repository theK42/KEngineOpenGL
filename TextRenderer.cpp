#include "TextRenderer.h"
#include "TextRenderer.h"
#include "TextRenderer.h"
#include "TextRenderer.h"
#include "OpenGLUtils.h"
#include "ShaderFactory.h"
#include "StaticTransform2D.h"
#include <array>

#include <iostream>

KEngineOpenGL::TextSprite::TextSprite()
{
}

KEngineOpenGL::TextSprite::~TextSprite()
{
	Deinit();
}

void KEngineOpenGL::TextSprite::Init(int width, int height, const KEngineOpenGL::ShaderProgram* shaderProgram, TextRenderer * renderer)
{
	assert(mFramebuffer == 0);
	mTextRenderer = renderer;
	glGenFramebuffers(1, &mFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

	glGenTextures(1, &mSprite.texture);
	glBindTexture(GL_TEXTURE_2D, mSprite.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mSprite.texture, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); 

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CheckGLError();
	}
	mSprite.width = width;
	mSprite.height = height;

	
	const KEngineOpenGL::Vertex Vertices[] = {
		{ { (float)width, 0.f, 0.f },{ 1.f, 1.f, 1.f, 1.f },{ 1.f, 0.f } },
		{ { (float)width, (float)height, 0.f },{ 1.f, 1.f, 1.f, 1.f },{ 1.f,1.f } },
		{ { 0.f, (float)height, 0.f },{ 1.f, 1.f, 1.f, 1.f },{ 0.f,1.f } },
		{ { 0.f, 0.f, 0.f },{ 1.f, 1.f, 1.f, 1.f },{ 0.f,0.f } }
	};

	const GLushort Indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSprite.vertexArrayObject = KEngineOpenGL::UploadModel(Vertices, 4, Indices, 6);
	mSprite.shaderProgram = shaderProgram;
	mSprite.indexCount = 6;

	float left = 0.0f;
	float right = (float)width;
	float top = 0.0f;
	float bottom = (float)height;
	float near = -1.0f;
	float far = 1.0f;

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

	mProjection[3][0] = -(right + left) / (right - left);
	mProjection[3][1] = -(top + bottom) / (top - bottom);
	mProjection[3][2] = -(far + near) / (far - near);
	mProjection[3][3] = 1.0f;

}

void KEngineOpenGL::TextSprite::Init(std::string_view text, const FixWidthBitmapFont& font, const KEngineOpenGL::ShaderProgram* shaderProgram, TextRenderer* renderer)
{
	int width = font.mGlyphWidth * text.length();
	int height = font.mGlyphHeight;

	Init(width, height, shaderProgram, renderer);
	RenderText(text, font);
}

void KEngineOpenGL::TextSprite::Deinit()
{
	glDeleteTextures(1, &mSprite.texture);
	mSprite.texture = 0;
	glDeleteFramebuffers(1, &mFramebuffer);
	mFramebuffer = 0;
}

void KEngineOpenGL::TextSprite::RenderText(std::string_view text, const FixWidthBitmapFont& font)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glViewport(0, 0, mSprite.width, mSprite.height);

	mTextRenderer->RenderText(this, text, font);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

KEngineOpenGL::Sprite& KEngineOpenGL::TextSprite::GetSprite()
{
	return mSprite;
}

const KEngineOpenGL::Sprite& KEngineOpenGL::TextSprite::GetSprite() const
{
	return mSprite;
}

const float* KEngineOpenGL::TextSprite::GetProjection() const
{
	return &mProjection[0][0];
}

KEngineOpenGL::TextRenderer::TextRenderer()
{
}

KEngineOpenGL::TextRenderer::~TextRenderer()
{
}

void KEngineOpenGL::TextRenderer::Init(int startingBufferSize, int maxBufferSize)
{
	mBufferSize = startingBufferSize;
	mMaxBufferSize = maxBufferSize;


	glGenVertexArrays(1, &mVertexArrayObjectHandle);
	glBindVertexArray(mVertexArrayObjectHandle);

	/* Allocate and assign two Vertex Buffer Objects to our handles */
	glGenBuffers(2, mBuffers);
	Resize(startingBufferSize);

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 7));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[1]);

	glBindVertexArray(0);

	

}

void KEngineOpenGL::TextRenderer::Resize(int newBufferSize)
{
	mBufferSize = newBufferSize;
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, mBufferSize * 4 * sizeof(KEngineOpenGL::Vertex), 0, GL_DYNAMIC_DRAW); //Vertex buffer will be updated every time we use it

	mVertexBufferVector.resize(newBufferSize * 4);

	for (unsigned short indexOffset = mIndexBufferVector.size(); indexOffset < newBufferSize * 4; indexOffset += 4)
	{
		std::array<GLushort, 6> glyphIndices = { indexOffset + 0, indexOffset + 1, indexOffset + 2,
			indexOffset + 2, indexOffset + 3, indexOffset + 0 };
		mIndexBufferVector.insert(mIndexBufferVector.end(), glyphIndices.begin(), glyphIndices.end());
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferSize * 6 * sizeof(GLushort), mIndexBufferVector.data(), GL_STATIC_DRAW);


}

void KEngineOpenGL::TextRenderer::Deinit()
{
	mBufferSize = 0;
	mUsedSize = 0;
	mMaxBufferSize = 0;
	mVertexBufferVector.clear();
	mIndexBufferVector.clear();
	glDeleteBuffers(2, mBuffers);
	glDeleteVertexArrays(1, &mVertexArrayObjectHandle);
}

void KEngineOpenGL::TextRenderer::RenderText(TextSprite* target, std::string_view text, const FixWidthBitmapFont& font)
{
	if (text.length() > mBufferSize)
	{
		Resize(std::min(mBufferSize * 2, mMaxBufferSize));
	}

	GLushort indexCounter = 0;
	int vertexIndex = 0;
	for (int characterIndex = 0; characterIndex < text.length(); characterIndex++)
	{
		char character = text[characterIndex];
		int glyphIndex = font.glyphs.find(character);
		if (glyphIndex != -1)
		{
			float leftPos = characterIndex * font.mGlyphWidth;
			float rightPos = leftPos + font.mGlyphWidth;
			int rowSize = font.mWidth / font.mGlyphWidth;
			int row = glyphIndex / rowSize;
			int col = glyphIndex % rowSize;
			float leftTex = (float)col *  (font.mGlyphWidth / (float)font.mWidth);
			float rightTex = (float)(col + 1) * (font.mGlyphWidth / (float)font.mWidth);
			float topTex = row* font.mGlyphHeight / (float)font.mHeight;
			float botTex = (row + 1)* font.mGlyphHeight / (float)font.mHeight;
			
			mVertexBufferVector[vertexIndex++] = { {rightPos, 0.f, 0.f}, { 1.f, 1.f, 1.f, 1.f }, {rightTex, botTex} };
			mVertexBufferVector[vertexIndex++] = { {rightPos, (float)font.mGlyphHeight, 0.f}, {1.f, 1.f, 1.f, 1.f }, {rightTex, topTex} };
			mVertexBufferVector[vertexIndex++] = { {leftPos, (float)font.mGlyphHeight, 0.f}, { 1.f, 1.f, 1.f, 1.f }, {leftTex, topTex} };
			mVertexBufferVector[vertexIndex++] = { {leftPos, 0.f, 0.f}, { 1.f, 1.f, 1.f, 1.f }, {leftTex, botTex} };

			indexCounter += 6; 
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexIndex * sizeof(KEngineOpenGL::Vertex), mVertexBufferVector.data()); //Vertex buffer will be updated every time we use it

	glUseProgram(font.shaderProgram->programHandle);

	CHECK_GL_ERROR();
	glUniformMatrix4fv(font.shaderProgram->projectionMatrixUniform, 1, 0, target->GetProjection());
	CHECK_GL_ERROR();
	const KEngine2D::Matrix& modelMatrix = KEngine2D::StaticTransform::Identity().GetAsMatrix();
	glUniformMatrix4fv(font.shaderProgram->modelMatrixUniform, 1, 1, &modelMatrix.data[0][0]);
	
	
	assert(font.texture != 0 && font.shaderProgram->textureUniform != -1);
	glActiveTexture(GL_TEXTURE0);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, font.texture);
	CHECK_GL_ERROR();
	glUniform1i(font.shaderProgram->textureUniform, 0);
	CHECK_GL_ERROR();
	
	if (font.shaderProgram->alphaUniform != -1)
	{
		glUniform1f(font.shaderProgram->alphaUniform, 1.0);
	}

	glBindVertexArray(mVertexArrayObjectHandle);
	CHECK_GL_ERROR();

	glClearColor(0, 0, 0, 0.0);
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_SHORT, 0);
	CHECK_GL_ERROR();
}


