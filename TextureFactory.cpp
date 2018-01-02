#include "TextureFactory.h"
#include "SOIL\SOIL.h"

void KEngineOpenGL::TextureFactory::Init()
{
	mTextures.clear();
}

void KEngineOpenGL::TextureFactory::CreateTexture(KEngineCore::StringHash name, const std::string & textureFilename)
{
	mTextures[name] = SOIL_load_OGL_texture(textureFilename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

const GLuint KEngineOpenGL::TextureFactory::GetTexture(KEngineCore::StringHash name)
{
	return mTextures[name];
}

