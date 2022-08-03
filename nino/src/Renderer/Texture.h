#pragma once

#include "Assets/Asset.h"

#include "Core/Core.h"

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
		virtual ~Texture() = default;

		static Ref<Texture> Create(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize = 1);
		static Ref<Texture> Create(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image);
		static Ref<Texture> Create(const Window* window);
	};
}