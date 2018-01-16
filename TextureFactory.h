#pragma once
#include "StringHash.h"
#include <string>
#include <map>

#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/gltypes.h>
    #endif
#elif defined(WIN32)
    #include <Windows.h>
    #include <gl/GL.h>
#endif

namespace KEngineOpenGL {
	class TextureFactory
	{
	public:
		void Init();
		void CreateTexture(KEngineCore::StringHash name, const std::string& textureFilename);
		const GLuint GetTexture(KEngineCore::StringHash name);
	private:
		std::map<KEngineCore::StringHash, GLuint> mTextures;
	};
}
