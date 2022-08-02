#include "corepch.h"
#include "D3D11Texture.h"

namespace nino
{
	auto device = D3D11Backend::GetDevice();
	auto context = D3D11Backend::GetContext();

	D3D11Texture::D3D11Texture(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize)
	{
		D3D11_TEXTURE2D_DESC tex2Ddesc = {};
		tex2Ddesc.ArraySize = arraySize;
		tex2Ddesc.SampleDesc.Count = 1;
		tex2Ddesc.Width = width;
		tex2Ddesc.Height = height;
		tex2Ddesc.MipLevels = 1;
		tex2Ddesc.Usage = D3D11_USAGE_DEFAULT;

		if (type == TextureType::TextureCube)
			tex2Ddesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

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
			tex2Ddesc.Format = DXGI_FORMAT_R32_TYPELESS;
			tex2Ddesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		case TextureUsage::StencilBinding:
		{
			tex2Ddesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			tex2Ddesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		}

		ThrowOnError(device->CreateTexture2D(&tex2Ddesc, nullptr, &m_Resource));

		if (usage == TextureUsage::ColorBinding)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			switch (type)
			{
			case nino::TextureType::Texture2D:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;
				srvDesc.Texture2D.MostDetailedMip = 0;

				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				break;
			}
			case nino::TextureType::Texture2DArray:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.ArraySize = arraySize;
				srvDesc.Texture2DArray.MipLevels = 1;
				srvDesc.Texture2DArray.MostDetailedMip = 0;

				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				break;
			}
			case nino::TextureType::TextureCube:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = 1;
				srvDesc.TextureCube.MostDetailedMip = 0;
				break;
			}
			}
			ThrowOnError(device->CreateShaderResourceView(m_Resource.Get(), &srvDesc, &m_ShaderResourceView));
			ThrowOnError(device->CreateUnorderedAccessView(m_Resource.Get(), &uavDesc, &m_UnorderedAccessView));
		}
		else if (usage == TextureUsage::DepthBinding)
		{
			m_UnorderedAccessView = nullptr;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			ThrowOnError(device->CreateShaderResourceView(m_Resource.Get(), &srvDesc, &m_ShaderResourceView));
		}
		else
		{
			m_ShaderResourceView = nullptr;
			m_UnorderedAccessView = nullptr;
		}
	}

	D3D11Texture::D3D11Texture(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image)
	{
		D3D11_TEXTURE2D_DESC tex2Ddesc = {};
		tex2Ddesc.ArraySize = image.GetMetadata().arraySize;
		tex2Ddesc.SampleDesc.Count = 1;
		tex2Ddesc.Width = image.GetMetadata().width;
		tex2Ddesc.Height = image.GetMetadata().height;
		tex2Ddesc.MipLevels = image.GetMetadata().mipLevels == 1 ? 0 : image.GetMetadata().mipLevels;
		tex2Ddesc.Format = image.GetMetadata().format;
		tex2Ddesc.Usage = D3D11_USAGE_DEFAULT;
		tex2Ddesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		switch (usage)
		{
		case TextureUsage::ColorBinding:
		{
			tex2Ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			break;
		}
		default:
			throw std::exception("Only color binding usage supported for textures created from images.");
			break;
		}

		if (type == TextureType::TextureCube)
			tex2Ddesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		ThrowOnError(device->CreateTexture2D(&tex2Ddesc, nullptr, &m_Resource));

		std::vector<D3D11_SUBRESOURCE_DATA> imageData(image.GetImageCount());

		for (size_t i = 0; i < image.GetImageCount(); i++)
		{
			imageData[i].pSysMem = static_cast<void*>(image.GetImages()[i].pixels);
			imageData[i].SysMemPitch = image.GetImages()[i].rowPitch;
			imageData[i].SysMemSlicePitch = image.GetImages()[i].slicePitch;

			context->UpdateSubresource(m_Resource.Get(), i, nullptr, imageData[i].pSysMem, imageData[i].SysMemPitch, imageData[i].SysMemSlicePitch);
		}

		if (usage == TextureUsage::ColorBinding)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = image.GetMetadata().format;

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = image.GetMetadata().format;

			switch (type)
			{
			case nino::TextureType::Texture2D:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = -1;
				srvDesc.Texture2D.MostDetailedMip = 0;

				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				break;
			}
			case nino::TextureType::Texture2DArray:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.ArraySize = image.GetMetadata().arraySize;
				srvDesc.Texture2DArray.MipLevels = -1;
				srvDesc.Texture2DArray.MostDetailedMip = 0;

				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				break;
			}
			case nino::TextureType::TextureCube:
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = -1;
				srvDesc.TextureCube.MostDetailedMip = 0;
				break;
			}
			}


			ThrowOnError(device->CreateShaderResourceView(m_Resource.Get(), &srvDesc, &m_ShaderResourceView));
			ThrowOnError(device->CreateUnorderedAccessView(m_Resource.Get(), &uavDesc, &m_UnorderedAccessView));
		}
		else
		{
			m_ShaderResourceView = nullptr;
			m_UnorderedAccessView = nullptr;
		}

		if (image.GetMetadata().mipLevels == 1)
		{
			context->GenerateMips(m_ShaderResourceView.Get());
		}
	}
}