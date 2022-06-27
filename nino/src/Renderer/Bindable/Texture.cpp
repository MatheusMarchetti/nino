#include "corepch.h"
#include "Texture.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

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

		std::filesystem::path ddsTexturePath{ m_TexturePath };

		std::filesystem::directory_entry ddsTexture{ ddsTexturePath.replace_extension(".dds") };

		if (ddsTexture.exists())
			m_TexturePath = ddsTexturePath;

		if (m_TexturePath.extension() == ".dds")
		{
			ThrowOnError(LoadFromDDSFile(m_TexturePath.c_str(), DDS_FLAGS_NONE, &imageInfo, loadedImage));
		}

		else if (m_TexturePath.extension() == ".tga")
		{
			ThrowOnError(LoadFromTGAFile(m_TexturePath.c_str(), &imageInfo, loadedImage));
		}

		else if (m_TexturePath.extension() == ".hdr")
		{
			ThrowOnError(LoadFromHDRFile(m_TexturePath.c_str(), &imageInfo, loadedImage));
		}

		else if (m_TexturePath.extension() == ".bmp" || m_TexturePath.extension() == ".jpg" || m_TexturePath.extension() == ".png" 
			|| m_TexturePath.extension() == ".tiff" || m_TexturePath.extension() == ".gif")
		{
			ThrowOnError(LoadFromWICFile(m_TexturePath.c_str(), WIC_FLAGS_NONE, &imageInfo, loadedImage));

			ThrowOnError(SaveToDDSFile(loadedImage.GetImages()[0], DDS_FLAGS_NONE, m_TexturePath.replace_extension(".dds").c_str()));
		}

		else
		{
			throw std::exception("Unsupported texture file format. Currently only supports BMP, JPG, PNG, TIFF and GIF files.");
		}

		ComputePitch(imageInfo.format, imageInfo.width, imageInfo.height, rowPitch, slicePitch);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = imageInfo.format;

		if (imageInfo.arraySize % 6 == 0)
		{
			// Cubemap processing. Save to .dds file
		}
		else
		{
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = imageInfo.width;
			textureDesc.Height = imageInfo.height;
			textureDesc.MipLevels = imageInfo.mipLevels;
			textureDesc.ArraySize = imageInfo.arraySize;
			textureDesc.Format = imageInfo.format;
			textureDesc.SampleDesc = { 1, 0 };
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			ComPtr<ID3D11Texture2D> texture2D;

			ThrowOnError(device->CreateTexture2D(&textureDesc, nullptr, &texture2D));

			ThrowOnError(texture2D.As(&m_Texture));

			context->UpdateSubresource(m_Texture.Get(), 0, nullptr, loadedImage.GetPixels(), rowPitch, slicePitch);

			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = -1;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			ThrowOnError(device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_ShaderResourceView));
		}

		if (imageInfo.mipLevels == 0)
		{
			context->GenerateMips(m_ShaderResourceView.Get());
		}
	}

	void Texture::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

