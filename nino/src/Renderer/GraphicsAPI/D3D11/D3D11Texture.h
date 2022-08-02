#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"

#include "Renderer/Texture.h"

namespace nino
{
	class D3D11Texture
	{
	public:
		D3D11Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize = 1);
		D3D11Texture(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image);

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;
	};
}