#include "corepch.h"
#include "Texture.h"

namespace nino
{
	using namespace Microsoft::WRL;

	Texture::Texture(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image)
		: m_Image(std::move(image))
	{
		
	}

	Texture::Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize)
	{
		
	}
}