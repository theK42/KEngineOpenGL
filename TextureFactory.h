#pragma once
#include "StringHash.h"
#include <string>
#include <map>

#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/gltypes.h>
    #else
        #include <glad/glad.h>
    #endif
#elif defined(_WIN32)
	#include <glad/glad.h>
#else
	#include <glad/glad.h>
#endif

namespace KEngineCore {
	class DataTree;
}

namespace KEngineOpenGL {
	class TextureFactory
	{
	public:
		TextureFactory();
		~TextureFactory();
		void Init();
		void Deinit();

		void CreateTextures(KEngineCore::DataTree* textureData);
		void CreateTexture(KEngineCore::StringHash name, const std::string_view& textureFilename);
		const GLuint GetTexture(KEngineCore::StringHash name);
	private:
		std::map<KEngineCore::StringHash, GLuint> mTextures;
	};
}
