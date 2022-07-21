#include "corepch.h"
#include "Texture.h"

#include "Core/Window.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	Texture::Texture(TextureType type, TextureUsage::TextureUsage usage, const std::string& fileName)
	{
		SetUUID(fileName);

		CreateTexture(type, usage, 1024, 1024); // Change to image dimensions
	}

	Texture::Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize)
		: m_ArraySize(arraySize)
	{
		CreateTexture(type, usage, width, height);
	}

	Texture::Texture(const Window* window)
	{
		auto device = GraphicsAPI::GetDevice();

		ThrowOnError(GraphicsAPI::GetSwapChain(window)->GetBuffer(0, IID_PPV_ARGS(&m_Resource)));

		m_ShaderResourceView = nullptr;
		m_UnorderedAccessView = nullptr;
	}

	void Texture::CreateTexture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height)
	{
		ComPtr<ID3D11Texture2D> texture2D;

		auto device = GraphicsAPI::GetDevice();

		D3D11_TEXTURE2D_DESC tex2Ddesc = {};
		tex2Ddesc.ArraySize = m_ArraySize;
		tex2Ddesc.SampleDesc.Count = 1;
		tex2Ddesc.Width = width;
		tex2Ddesc.Height = height;
		tex2Ddesc.MipLevels = 1;
		tex2Ddesc.Usage = D3D11_USAGE_DEFAULT;
		
		switch (usage)
		{
			case TextureUsage::ColorBinding:	
			{
				tex2Ddesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				tex2Ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
				break;
			}
			case TextureUsage::DepthBinding:	
			{
				tex2Ddesc.Format = DXGI_FORMAT_D32_FLOAT;
				tex2Ddesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
				break;
			}
			case TextureUsage::StencilBinding:	
			{
				tex2Ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				tex2Ddesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
				break;
			}
		}

		ThrowOnError(device->CreateTexture2D(&tex2Ddesc, nullptr, &texture2D));

		ThrowOnError(texture2D.As(&m_Resource));

		if (TextureUsage::ColorBinding)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			ThrowOnError(device->CreateShaderResourceView(m_Resource.Get(), &srvDesc, &m_ShaderResourceView));
			ThrowOnError(device->CreateUnorderedAccessView(m_Resource.Get(), &uavDesc, &m_UnorderedAccessView));
		}
		else
		{
			m_ShaderResourceView = nullptr;
			m_UnorderedAccessView = nullptr;
		}
	}
}