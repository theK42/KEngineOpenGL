#include "FontFactory.h"
#include "DataTree.h"
#include "TextureFactory.h"
#include "ShaderFactory.h"

KEngineOpenGL::FontFactory::FontFactory()
{
}

KEngineOpenGL::FontFactory::~FontFactory()
{
	Deinit();
}

void KEngineOpenGL::FontFactory::Init(TextureFactory* textureFactory, ShaderFactory* shaderFactory)
{
	assert(mTextureFactory == nullptr);
	mTextureFactory = textureFactory;
	mShaderFactory = shaderFactory;
}

void KEngineOpenGL::FontFactory::Deinit()
{
	mFonts.clear();
}

void KEngineOpenGL::FontFactory::CreateFonts(KEngineCore::DataTree* fontsData)
{
	for (auto* branch : fontsData->GetBranches())
	{
		CreateFont(branch);
	}
}

void KEngineOpenGL::FontFactory::CreateFont(KEngineCore::DataTree* fontData)
{
	int mWidth;
	int mHeight;
	int mGlyphWidth;
	int mGlyphHeight;
	std::string_view glyphs;
	const ShaderProgram* shaderProgram;
	GLuint texture;

	FixWidthBitmapFont font;
	font.texture = mTextureFactory->GetTexture(fontData->GetHash(HASH("texture", 0x82660D72)));
	font.mWidth = fontData->GetInt(HASH("width", 0x8C1A452F));
	font.mHeight = fontData->GetInt(HASH("height", 0xF54DE50F));

	font.mGlyphWidth = fontData->GetInt(HASH("glyphWidth", 0x5ACF4F5E));
	font.mGlyphHeight = fontData->GetInt(HASH("glyphHeight", 0xD29971AF));
	font.glyphs = fontData->GetString(HASH("glyphs", 0xDE1F4262));
	font.shaderProgram = mShaderFactory->GetShaderProgram(fontData->GetHash(HASH("shader", 0xFFF39E28)));

	mFonts[fontData->GetHash(HASH("name", 0x5E237E06))] = font;
}

const KEngineOpenGL::FixWidthBitmapFont& KEngineOpenGL::FontFactory::GetFont(KEngineCore::StringHash name)
{
	return mFonts[name];
}