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

	Texture::Texture(const std::string& filepath, const uint32_t& slot)
		: m_TexturePath(filepath), m_Slot(slot)
	{
		auto device = GraphicsAPI::GetDevice();
		auto context = GraphicsAPI::GetContext();

		ScratchImage loadedImage;
		TexMetadata imageInfo;
		size_t rowPitch, slicePitch;

		if (filepath.empty())
			m_TexturePath = "Assets/Textures/DefaultWhite.bmp";

		std::filesystem::path ddsTexturePath{ m_TexturePath };

		std::filesystem::directory_entry ddsTexture{ ddsTexturePath.replace_extension(".dds") };

		if (ddsTexture.exists())
			m_TexturePath = ddsTexturePath;

		if (m_TexturePath.extension() == ".dds")
		{
<<<<<<< HEAD
			ThrowOnError(LoadFromDDSFile(m_TexturePath.c_str(), DDS_FLAGS_NONE, &imageInfo, loadedImage));
=======
			ThrowOnError(LoadFromDDSFile(m_TexturePath.c_str(), DDS_FLAGS_NONE, nullptr, loadedImage));

			if (loadedImage.GetMetadata().mipLevels == 1 && loadedImage.GetMetadata().width > 1)
			{
				ThrowOnError(GenerateMipMaps(loadedImage.GetImages(), loadedImage.GetImageCount(), loadedImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain));
				ConvertToDDS(mipChain, m_TexturePath.c_str(), loadedImage);
			}
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		}

		else if (m_TexturePath.extension() == ".tga")
		{
<<<<<<< HEAD
			ThrowOnError(LoadFromTGAFile(m_TexturePath.c_str(), &imageInfo, loadedImage));
=======
			ThrowOnError(LoadFromTGAFile(m_TexturePath.c_str(), nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds").c_str(), loadedImage);
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		}

		else if (m_TexturePath.extension() == ".hdr")
		{
<<<<<<< HEAD
			ThrowOnError(LoadFromHDRFile(m_TexturePath.c_str(), &imageInfo, loadedImage));
=======
			ThrowOnError(LoadFromHDRFile(m_TexturePath.c_str(), nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds").c_str(), loadedImage);
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		}

		else if (m_TexturePath.extension() == ".bmp" || m_TexturePath.extension() == ".jpg" || m_TexturePath.extension() == ".png" 
			|| m_TexturePath.extension() == ".tiff" || m_TexturePath.extension() == ".gif")
		{
<<<<<<< HEAD
			ThrowOnError(LoadFromWICFile(m_TexturePath.c_str(), WIC_FLAGS_NONE, &imageInfo, loadedImage));

			ThrowOnError(SaveToDDSFile(loadedImage.GetImages()[0], DDS_FLAGS_NONE, m_TexturePath.replace_extension(".dds").c_str()));
=======
			ThrowOnError(LoadFromWICFile(m_TexturePath.c_str(), WIC_FLAGS_NONE, nullptr, loadedImage));
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds").c_str(), loadedImage);
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		}

		else
		{
			throw std::exception("Unsupported texture file format. Currently only supports BMP, JPG, PNG, TIFF and GIF files.");
		}

		ComputePitch(imageInfo.format, imageInfo.width, imageInfo.height, rowPitch, slicePitch);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = imageInfo.width;
		textureDesc.Height = imageInfo.height;
		textureDesc.MipLevels = imageInfo.mipLevels;
		textureDesc.ArraySize = imageInfo.arraySize;
		textureDesc.Format = imageInfo.format;
		textureDesc.SampleDesc = { 1, 0 };
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ComPtr<ID3D11Texture2D> texture2D;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = imageInfo.format;

<<<<<<< HEAD
		if (imageInfo.arraySize % 6 == 0)
=======

		if (loadedImage.GetMetadata().arraySize % 6 == 0)
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		{
			imageInfo.miscFlags |= TEX_MISC_TEXTURECUBE;
			textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = imageInfo.mipLevels;
		}
		else
		{
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = imageInfo.mipLevels;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		}

<<<<<<< HEAD
		ThrowOnError(device->CreateTexture2D(&textureDesc, nullptr, &texture2D));

		ThrowOnError(texture2D.As(&m_Texture));

		context->UpdateSubresource(m_Texture.Get(), 0, nullptr, loadedImage.GetPixels(), rowPitch, slicePitch);

		ThrowOnError(device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_ShaderResourceView));

		if (imageInfo.mipLevels == 1)
		{
			context->GenerateMips(m_ShaderResourceView.Get());
=======
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
			ConvertToDDS(loadedImage, m_TexturePath.replace_extension(".dds").c_str(), loadedImage);

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
>>>>>>> 4200d5b (Added conversion from Equirectangular to Cubemap.)
		}
	}

	void Texture::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

