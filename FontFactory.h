#pragma once

#include "TextRenderer.h"

namespace KEngineCore {
	class DataTree;
}

namespace KEngineOpenGL {
	class TextureFactory;
	class ShaderFactory;

	class FontFactory
	{
	public:
		FontFactory();
		~FontFactory();
		void Init(TextureFactory * textureFactory, ShaderFactory* shaderFactory);
		void Deinit();

		void CreateFonts(KEngineCore::DataTree* fontsData); 
		void CreateFont(KEngineCore::DataTree* fontData);
		const FixWidthBitmapFont& GetFont(KEngineCore::StringHash name);
	private:
		TextureFactory* mTextureFactory{ nullptr };
		ShaderFactory* mShaderFactory{ nullptr };
		std::map<KEngineCore::StringHash, FixWidthBitmapFont> mFonts;
	};
}

