#pragma once

#include "DirectXTex.h"

namespace nino
{
	class AssetLoader
	{
	public:
		static bool HasAssetBinary(uint64_t assetID);
		static DirectX::ScratchImage LoadTexture(const std::string& filePath);
		static DirectX::ScratchImage LoadTexture(uint64_t assetID);

		static void CreateBinaryTexture(uint64_t assetID, DirectX::ScratchImage& image);

	};
}