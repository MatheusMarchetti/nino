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
		Texture(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image);
		Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize = 1);
		Texture(const Window* window);
		virtual ~Texture() = default;

		ID3D11Resource* GetResource() { return m_Resource.Get(); }

	private:
		void CreateTexture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height);

	private:
		uint32_t m_ArraySize;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_Resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;
		DirectX::ScratchImage m_Image;
	};
}