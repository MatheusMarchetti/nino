#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

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

	class Texture
	{
	public:
		Texture(TextureType type, TextureUsage::TextureUsage usage, const std::string& filePath = {});
		Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize = 1);
		Texture(const Window* window);
		~Texture() = default;

		ID3D11Resource* GetResource() { return m_Resource.Get(); }

	private:
		void CreateTexture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height);

	private:
		uint32_t m_ArraySize;
		std::filesystem::path m_TexturePath;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_Resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;
	};
}