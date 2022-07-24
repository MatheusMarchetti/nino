#include "corepch.h"
#include "AssetLoader.h"

namespace nino
{
	bool AssetLoader::HasAssetBinary(uint64_t assetID)
	{
		return false;
	}

	DirectX::ScratchImage AssetLoader::LoadTexture(const std::string& filePath)
	{
		using namespace DirectX;

		ScratchImage loadedImage;

		std::filesystem::path texturePath = filePath;

		if (texturePath.extension() == ".dds")
		{
			ThrowOnError(LoadFromDDSFile(texturePath.c_str(), DDS_FLAGS_NONE, nullptr, loadedImage));
		}
		else if (texturePath.extension() == ".tga")
		{
			ThrowOnError(LoadFromTGAFile(texturePath.c_str(), nullptr, loadedImage));
		}
		else if (texturePath.extension() == ".hdr")
		{
			ThrowOnError(LoadFromHDRFile(texturePath.c_str(), nullptr, loadedImage));
		}
		else if (texturePath.extension() == ".bmp" || texturePath.extension() == ".jpg" || texturePath.extension() == ".png" || texturePath.extension() == ".tiff" || texturePath.extension() == ".gif")
		{
			ThrowOnError(LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_NONE, nullptr, loadedImage));
		}
		else
		{
			throw std::exception("Unsupported texture format!");
		}

		return loadedImage;
	}

	DirectX::ScratchImage AssetLoader::LoadTexture(uint64_t assetID)
	{
		return DirectX::ScratchImage();
	}

	void AssetLoader::CreateBinaryTexture(uint64_t assetID, DirectX::ScratchImage& image)
	{

	}
}
