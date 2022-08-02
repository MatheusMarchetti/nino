#pragma once

#include "Assets/Asset.h"
#include "Renderer/GraphicsAPI/GraphicsCore.h"

#include "DirectXTex.h"

namespace nino
{
	class Window;

	enum class TextureType
	{
		Texture2D,
		Texture2DArray,
		TextureCube,
	};

	namespace TextureUsage
	{
		enum TextureUsage
		{
			ColorBinding,
			DepthBinding,
			StencilBinding
		};
	}

	class Texture : public Asset
	{
	public:
		Texture() = default;
		Texture(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image);
		Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize = 1);
		virtual ~Texture() = default;

	private:
		DirectX::ScratchImage m_Image;
	};
}