#include "corepch.h"
#include "AssetLoader.h"

namespace nino
{
	void AssetLoader::LoadAssetBinary(uint64_t assetID)
	{
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

	void AssetLoader::CreateBinaryTexture(uint64_t assetID, DirectX::ScratchImage& image)
	{
		using namespace DirectX;

		std::string fileName = std::to_string(assetID) + ".bin";

		std::ofstream fs(fileName, std::fstream::binary);
		

		for (size_t i = 0; i < image.GetImageCount(); i++)
		{
			std::string header = "Image " + std::to_string(i) + ":" + std::to_string(image.GetImages()[i].width) + "x" + std::to_string(image.GetImages()[i].height) + '\n';

			fs.write(header.c_str(), header.size());
			fs.write(reinterpret_cast<const char*>(image.GetImages()[i].pixels), image.GetImages()[i].rowPitch * image.GetImages()[i].height);
		}

		fs.close();

		NINO_CORE_WARN("File created!");
	}
}
