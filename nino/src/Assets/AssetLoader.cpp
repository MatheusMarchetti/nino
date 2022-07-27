#include "corepch.h"
#include "AssetLoader.h"

#include "Core/Timestep.h"

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

		Timestep time;

		if (texturePath.extension() == ".dds")
		{
			time.Tick();
			ThrowOnError(LoadFromDDSFile(texturePath.c_str(), DDS_FLAGS_NONE, nullptr, loadedImage));
			time.Tick();
		}
		else if (texturePath.extension() == ".tga")
		{
			time.Tick();
			ThrowOnError(LoadFromTGAFile(texturePath.c_str(), nullptr, loadedImage));
			time.Tick();
		}
		else if (texturePath.extension() == ".hdr")
		{
			time.Tick();
			ThrowOnError(LoadFromHDRFile(texturePath.c_str(), nullptr, loadedImage));
			time.Tick();
		}
		else if (texturePath.extension() == ".bmp" || texturePath.extension() == ".jpg" || texturePath.extension() == ".png" || texturePath.extension() == ".tiff" || texturePath.extension() == ".gif")
		{
			time.Tick();
			ThrowOnError(LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_NONE, nullptr, loadedImage));
			time.Tick();
		}
		else
		{
			throw std::exception("Unsupported texture format!");
		}

		NINO_CORE_INFO("Loading of {} image of dimensions {} x {} took {} ms", texturePath.extension().string(), loadedImage.GetMetadata().width, loadedImage.GetMetadata().height, time);

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
