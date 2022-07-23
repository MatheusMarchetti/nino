#pragma once

#include "DirectXTex.h"

namespace nino
{
	class AssetLoader
	{
	public:
		static void LoadAssetBinary(uint64_t assetID);
		static DirectX::ScratchImage LoadTexture(const std::string& filePath);

		static void CreateBinaryTexture(uint64_t assetID, DirectX::ScratchImage& image);

	};
}