#include "TextureFactory.h"
#include "DataTree.h"
#include "SOIL/SOIL.h"
#include <assert.h>
#include "OpenGLUtils.h"

#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        //?
    #else
        #include <OpenGL/gl.h>
    #endif
#endif

KEngineOpenGL::TextureFactory::TextureFactory()
{
}


KEngineOpenGL::TextureFactory::~TextureFactory()
{
	Deinit();
}

void KEngineOpenGL::TextureFactory::Init()
{
	assert(mTextures.empty());
}

void KEngineOpenGL::TextureFactory::Deinit()
{
	for (auto texturePair : mTextures)
	{
		glDeleteTextures(1, &texturePair.second);  //Could probably do this more efficiently
	}
	mTextures.clear();
}


void KEngineOpenGL::TextureFactory::CreateTextures(KEngineCore::DataTree* textureData)
{
	for (auto* branch : textureData->GetBranches())
	{
		CreateTexture(branch->GetHash(HASH("name", 0x5E237E06)), std::string(branch->GetString(HASH("filename", 0x3C0BE965))));
	}
}

void KEngineOpenGL::TextureFactory::CreateTexture(KEngineCore::StringHash name, const std::string_view & textureFilename)
{
	assert(mTextures.find(name) == mTextures.end());
	std::string temp(textureFilename); //TODO find a texture loading service that doesn't require c-strings?
	mTextures[name] = SOIL_load_OGL_texture(temp.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

const GLuint KEngineOpenGL::TextureFactory::GetTexture(KEngineCore::StringHash name)
{
	return mTextures[name];
}

