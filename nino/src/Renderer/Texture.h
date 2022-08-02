#pragma once

#include "Assets/Asset.h"

#include "Renderer/GraphicsAPI/GraphicsCore.h"

#include "DirectXTex.h"

namespace nino
{
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
	};
}