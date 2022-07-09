#include "corepch.h"
#include "Texture.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Bindable/ComputeShader.h"

#include "WICTextureLoader.h"
#include "DirectXTex.h"

namespace nino
{
	using namespace DirectX;
	using namespace Microsoft::WRL;

	void ConvertToDDS(ScratchImage& inputImage, const std::filesystem::path& filepath, ScratchImage& outputImage)
	{
		SaveToDDSFile(inputImage.GetImages(), inputImage.GetImageCount(), inputImage.GetMetadata(), DDS_FLAGS_NONE, filepath.c_str());
		LoadFromDDSFile(filepath.c_str(), DDS_FLAGS_NONE, nullptr, outputImage);
	}

	Texture::Texture(const std::string& filepath, const uint32_t& slot)
		: m_TexturePath(filepath), m_Slot(slot)
	{
		auto device = GraphicsAPI::GetDevice();
		auto context = GraphicsAPI::GetContext();

		ScratchImage loadedImage;
		ScratchImage mipChain;

		if (filepath.empty())
			m_TexturePath = "Assets/Textures/DefaultWhite.bmp";

		std::filesystem::path ddsTexturePath{ m_TexturePath };

		std::filesystem::directory_entry ddsTexture{ ddsTexturePath.replace_extension(".dds") };

		if (ddsTexture.exists())
			m_TexturePath = ddsTexturePath;

		if (m_TexturePath.extension() == ".dds")
		{
			ThrowOnError(LoadFromDDSFile(m_TexturePath.c_str(), DDS_FLAGS_NONE, nullptr, loadedImage));

			if (loadedImage.GetMetadata().mipLevels == 1 && loadedImage.GetMetadata().width > 1)
			{
				ThrowOnError(GenerateMipMaps(loadedImage.GetImages(), loadedImage.GetImageCount(), loadedImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain));
				ConvertToDDS(mipChain, m_TexturePath, loadedImage);
			}
		}

		else if (m_TexturePath.extension() == ".tga")
		{
			ThrowOnError(LoadFromTGAFile(m_TexturePath.c_str(), nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds"), loadedImage);
		}

		else if (m_TexturePath.extension() == ".hdr")
		{
			ThrowOnError(LoadFromHDRFile(m_TexturePath.c_str(), nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds"), loadedImage);
		}

		else if (m_TexturePath.extension() == ".bmp" || m_TexturePath.extension() == ".jpg" || m_TexturePath.extension() == ".png" 
			|| m_TexturePath.extension() == ".tiff" || m_TexturePath.extension() == ".gif")
		{
			ThrowOnError(LoadFromWICFile(m_TexturePath.c_str(), WIC_FLAGS_NONE, nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds"), loadedImage);
		}

		else
		{
			throw std::exception("Unsupported texture file format. Currently only supports BMP, JPG, PNG, TIFF and GIF files.");
		}

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = loadedImage.GetMetadata().width;
		textureDesc.Height = loadedImage.GetMetadata().height;
		textureDesc.MipLevels = loadedImage.GetMetadata().mipLevels;
		textureDesc.ArraySize = loadedImage.GetMetadata().arraySize;
		textureDesc.Format = loadedImage.GetMetadata().format;
		textureDesc.SampleDesc = { 1, 0 };
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ComPtr<ID3D11Texture2D> texture2D;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = loadedImage.GetMetadata().format;

		if (loadedImage.GetMetadata().arraySize % 6 == 0)
		{
			textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = loadedImage.GetMetadata().mipLevels;
		}
		else
		{
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = loadedImage.GetMetadata().mipLevels;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		}

		if (loadedImage.GetMetadata().width / loadedImage.GetMetadata().height == 2)
		{
			//Temporarily create texture from loaded image.
			ThrowOnError(device->CreateTexture2D(&textureDesc, nullptr, &texture2D));
			context->UpdateSubresource(texture2D.Get(), 0, nullptr, loadedImage.GetImages()[0].pixels, loadedImage.GetImages()[0].rowPitch, loadedImage.GetImages()[0].slicePitch);

			ThrowOnError(device->CreateShaderResourceView(texture2D.Get(), &srvDesc, &m_ShaderResourceView));

			//Prepare compute program
			ComputeShader equirect2Cubemap("Assets/Shaders/EquirectToCubemap.hlsl");
			equirect2Cubemap.Bind();

			ComPtr<ID3D11Texture2D> unfilteredTexture;
			ComPtr<ID3D11SamplerState> unfilteredSampler;
			ComPtr<ID3D11ShaderResourceView> unfilteredSRV;
			ComPtr<ID3D11UnorderedAccessView> unfilteredUAV;

			D3D11_TEXTURE2D_DESC unfilteredDesc = {};
			unfilteredDesc.Width = 1024;
			unfilteredDesc.Height = 1024;
			unfilteredDesc.MipLevels = 1;
			unfilteredDesc.ArraySize = 6;
			unfilteredDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			unfilteredDesc.SampleDesc.Count = 1;
			unfilteredDesc.Usage = D3D11_USAGE_DEFAULT;
			unfilteredDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
			unfilteredDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

			ThrowOnError(device->CreateTexture2D(&unfilteredDesc, nullptr, &unfilteredTexture));

			D3D11_SHADER_RESOURCE_VIEW_DESC unfilteredSRVDesc = {};
			unfilteredSRVDesc.Format = unfilteredDesc.Format;
			unfilteredSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			unfilteredSRVDesc.TextureCube.MostDetailedMip = 0;
			unfilteredSRVDesc.TextureCube.MipLevels = -1;

			ThrowOnError(device->CreateShaderResourceView(unfilteredTexture.Get(), &unfilteredSRVDesc, &unfilteredSRV));

			D3D11_UNORDERED_ACCESS_VIEW_DESC unfilteredUAVDesc = {};
			unfilteredUAVDesc.Format = unfilteredDesc.Format;
			unfilteredUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			unfilteredUAVDesc.Texture2DArray.MipSlice = 0;
			unfilteredUAVDesc.Texture2DArray.FirstArraySlice = 0;
			unfilteredUAVDesc.Texture2DArray.ArraySize = unfilteredDesc.ArraySize;

			ThrowOnError(device->CreateUnorderedAccessView(unfilteredTexture.Get(), &unfilteredUAVDesc, &unfilteredUAV));

			D3D11_SAMPLER_DESC unfilteredSamplerDesc = {};
			unfilteredSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			unfilteredSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			unfilteredSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			unfilteredSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

			ThrowOnError(device->CreateSamplerState(&unfilteredSamplerDesc, &unfilteredSampler));

			equirect2Cubemap.SetResources(nullptr, unfilteredSampler, m_ShaderResourceView, unfilteredUAV);
			equirect2Cubemap.Run(unfilteredDesc.Width / 32, unfilteredDesc.Height / 32, 6);

			//Reset texture
			m_ShaderResourceView = nullptr;

			unfilteredUAV->GetResource(&m_Texture);

			ThrowOnError(CaptureTexture(device.Get(), context.Get(), m_Texture.Get(), loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds"), loadedImage);

			ThrowOnError(device->CreateShaderResourceView(m_Texture.Get(), &unfilteredSRVDesc, &m_ShaderResourceView));
		}
		else
		{
			ThrowOnError(device->CreateTexture2D(&textureDesc, nullptr, &texture2D));

			ThrowOnError(texture2D.As(&m_Texture));

			std::vector<D3D11_SUBRESOURCE_DATA> subresources(loadedImage.GetImageCount());

			for (size_t i = 0; i < loadedImage.GetImageCount(); i++)
			{
				subresources[i].pSysMem = static_cast<void*>(loadedImage.GetImages()[i].pixels);
				subresources[i].SysMemPitch = loadedImage.GetImages()[i].rowPitch;
				subresources[i].SysMemSlicePitch = loadedImage.GetImages()[i].slicePitch;

				context->UpdateSubresource(m_Texture.Get(), i, nullptr, subresources[i].pSysMem, subresources[i].SysMemPitch, subresources[i].SysMemSlicePitch);
			}

			ThrowOnError(device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_ShaderResourceView));
		}
	}

	void Texture::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

